#include "scandalizer/PostProcessors.h"
#include "scandalizer/ScriptHelpers.h"
#include "scandalizer/SpectrometerMonitor.h"
#include "monitor/DetectorDisplay.h"
#include "monitor/EventDisplays.h"
#include "monitor/ExperimentMonitor.h"

#include <chrono>
#include <iostream>
#include <locale>
#include <string>

// Logging
#include "spdlog/spdlog.h"

// Better formatting
#include <fmt/format.h>

// analyzer includes
#include "THaCut.h"
#include "THaGoldenTrack.h"
#include "THaReactionPoint.h"
#include "THcAerogel.h"
#include "THcAnalyzer.h"
#include "THcCherenkov.h"
#include "THcCoinTime.h"
#include "THcConfigEvtHandler.h"
#include "THcDC.h"
#include "THcDetectorMap.h"
#include "THcExtTarCor.h"
#include "THcGlobals.h"
#include "THcHallCSpectrometer.h"
#include "THcHelicity.h"
#include "THcHodoEff.h"
#include "THcHodoscope.h"
#include "THcParmList.h"
#include "THcPrimaryKine.h"
#include "THcRasteredBeam.h"
#include "THcRun.h"
#include "THcScalerEvtHandler.h"
#include "THcSecondaryKine.h"
#include "THcShower.h"
#include "THcTrigApp.h"
#include "THcTrigDet.h"

// reimplemented without logger -> need to fix -whit
class SimplePostProcess : public THaPostProcess {
public:
  using Function_t     = std::function<int(const THaEvData*)>;
  using InitFunction_t = std::function<int()>;

  Function_t          _event_lambda;
  InitFunction_t      _init_lambda;
  hcana::Scandalizer* _analyzer = nullptr;

public:
  SimplePostProcess(Function_t&& f)
      : THaPostProcess()
      , _event_lambda(std::forward<Function_t>(f))
      , _init_lambda([]() { return 0; }) {}

  SimplePostProcess(InitFunction_t&& initf, Function_t&& f)
      : THaPostProcess()
      , _event_lambda(std::forward<Function_t>(f))
      , _init_lambda(std::forward<InitFunction_t>(initf)) {}

  virtual ~SimplePostProcess() {}

  virtual Int_t Init(const TDatime&) { return _init_lambda(); }
  virtual Int_t Process(const THaEvData* evt, const THaRunBase*, Int_t code) {
    _event_lambda(evt);
    if (evt->GetEvNum() % 100 == 0) {
      gSystem->ProcessEvents();
    }
    return 0;
  }
  virtual Int_t Close() {
    std::cout << "close\n";
    return 0;
  }
};

std::string coda_file_pattern(bool do_coin) {
  return fmt::format("{}_all_{{:05d}}.dat", do_coin ? "coin" : "hms");
}
std::string output_file_pattern(string_view path, string_view content, string_view extension,
                                bool do_coin, bool do_all) {
  return fmt::format("{}/hms{}_{}{}_{{:05d}}_{{}}.{}", path, do_coin ? "_coin" : "", content,
                     do_all ? "_all" : "", extension);
}

int monitor_hms(Int_t RunNumber = 2368, Int_t MaxEvent = -1, Int_t FirstEvent = 0,
                const bool do_all = false, const bool do_coin = false) {
  using namespace std;
  // ===========================================================================
  // Setup logging
  spdlog::set_level(spdlog::level::warn);
  spdlog::flush_every(std::chrono::seconds(5));

  // ===========================================================================
  // Get RunNumber and MaxEvent if not provided.
  if (RunNumber == 0) {
    cout << "Enter a Run Number (-1 to exit): ";
    if (RunNumber <= 0)
      return -1;
  }
  // note: MaxEvent equal to -1 means all events
  if (MaxEvent == 0) {
    cout << "\nNumber of Events to analyze: ";
    cin >> MaxEvent;
    if (MaxEvent == 0) {
      cerr << "...Invalid entry\n";
      return -1;
    }
  }

  // ===========================================================================
  // Create file name patterns.
  //
  // 1. Input files
  const auto RunFileNamePattern = coda_file_pattern(do_coin);

  vector<TString> pathList;
  pathList.push_back(".");
  pathList.push_back("./DATA/raw");
  pathList.push_back("./raw_coda");
  pathList.push_back("./raw");
  pathList.push_back("./raw.copiedtotape");
  pathList.push_back("./raw/../raw.copiedtotape");
  pathList.push_back("./cache");
  // 2. Output files
  const auto ROOTFileNamePattern =
      output_file_pattern("ROOTfiles", "replay_production", "root", do_coin, do_all);

  // Load global parameters
  gHcParms->Define("gen_run_number", "Run Number", RunNumber);
  gHcParms->AddString("g_ctp_database_filename",
                      do_coin ? "DBASE/COIN/standard.database" : "DBASE/HMS/standard.database");
  gHcParms->Load(gHcParms->GetString("g_ctp_database_filename"), RunNumber);
  gHcParms->Load(gHcParms->GetString("g_ctp_parm_filename"));
  gHcParms->Load(gHcParms->GetString("g_ctp_kinematics_filename"), RunNumber);
  // Load parameters for HMS trigger configuration
  gHcParms->Load("PARAM/TRIG/thms.param");
  // Load fadc debug parameters
  gHcParms->Load("PARAM/HMS/GEN/h_fadc_debug.param");

  // Load the Hall C detector map
  gHcDetectorMap = new THcDetectorMap();
  gHcDetectorMap->Load("MAPS/HMS/DETEC/STACK/hms_stack.map");

  // ===========================================================================
  // Experimental apparatus
  //
  // ---------------------------------------------------------------------------
  // A. HMS setup
  //
  // Set up the equipment to be analyzed.
  THcHallCSpectrometer* HMS = new THcHallCSpectrometer("H", "HMS");
  if (do_coin) {
    HMS->SetEvtType(2);
    HMS->AddEvtType(4);
    HMS->AddEvtType(5);
    HMS->AddEvtType(6);
    HMS->AddEvtType(7);
  }
  gHaApps->Add(HMS);
  // 1. Add drift chambers to HMS apparatus
  THcDC* hdc = new THcDC("dc", "Drift Chambers");
  HMS->AddDetector(hdc);
  // 2. Add hodoscope to HMS apparatus
  THcHodoscope* hhod = new THcHodoscope("hod", "Hodoscope");
  HMS->AddDetector(hhod);
  // 3. Add Heavy Gas Cherenkov to HMS apparatus
  THcCherenkov* hcer = new THcCherenkov("cer", "Heavy Gas Cherenkov");
  HMS->AddDetector(hcer);
  // 6. Add calorimeter to HMS apparatus
  THcShower* hcal = new THcShower("cal", "Calorimeter");
  HMS->AddDetector(hcal);
  // ---------------------------------------------------------------------------
  // B. Beamline
  //
  // Add rastered beam apparatus
  THaApparatus* hbeam = new THcRasteredBeam("H.rb", "Rastered Beamline");
  gHaApps->Add(hbeam);

  // ---------------------------------------------------------------------------
  // C. Trigger
  //
  // Add trigger detector to trigger apparatus
  THaApparatus* TRG = new THcTrigApp("T", "TRG");
  gHaApps->Add(TRG);
  // Add trigger detector to trigger apparatus
  THcTrigDet* hms = new THcTrigDet("hms", "HMS Trigger Information");
  hms->SetSpectName("H");
  TRG->AddDetector(hms);
  THcHelicity* helicity = new THcHelicity("helicity", "Helicity Detector");
  TRG->AddDetector(helicity);

  // ===========================================================================
  // Phyics and derived quantities
  //
  // 1. Calculate reaction point
  THaReactionPoint* hrp = new THaReactionPoint("H.react", "HMS reaction point", "H", "H.rb");
  gHaPhysics->Add(hrp);
  // 2. Calculate extended target corrections
  THcExtTarCor* hext =
      new THcExtTarCor("H.extcor", "HMS extended target corrections", "H", "H.react");
  gHaPhysics->Add(hext);
  // 3. Calculate golden track quantites
  THaGoldenTrack* hgtr = new THaGoldenTrack("H.gtr", "HMS Golden Track", "H");
  gHaPhysics->Add(hgtr);
  // 4. Calculate the hodoscope efficiencies
  THcHodoEff* heff = new THcHodoEff("hhodeff", "HMS hodo efficiency", "H.hod");
  gHaPhysics->Add(heff);
  // 5. Single arm kinematics
  THcPrimaryKine* hkin = new THcPrimaryKine("H.kin", "HMS Single Arm Kinematics", "H", "H.rb");
  gHaPhysics->Add(hkin);

  // ===========================================================================
  //  Global Objects & Event Handlers
  //
  // Add event handler for scaler events
  THcScalerEvtHandler* hscaler = new THcScalerEvtHandler("H", "Hall C scaler event type 2");
  hscaler->AddEvtType(2);
  if (do_coin) {
    hscaler->AddEvtType(4);
    hscaler->AddEvtType(5);
    hscaler->AddEvtType(6);
    hscaler->AddEvtType(7);
  }
  hscaler->AddEvtType(129);
  hscaler->SetDelayedType(129);
  hscaler->SetUseFirstEvent(kTRUE);
  gHaEvtHandlers->Add(hscaler);

  // Add event handler for prestart event 125.
  THcConfigEvtHandler* hconfig = new THcConfigEvtHandler("hconfig", "Config Event type 125");
  gHaEvtHandlers->Add(hconfig);
  // Add event handler for EPICS events
  THaEpicsEvtHandler* hcepics = new THaEpicsEvtHandler(
      "epics", do_coin ? "HC EPICS event type 182" : "HC EPICS event type 181");
  gHaEvtHandlers->Add(hcepics);

  // -----------------------------------------------------------
  // Analyzer
  // -----------------------------------------------------------
  //
  THcAnalyzer* analyzer = new THcAnalyzer;
  //hcana::Scandalizer* analyzer = new hcana::Scandalizer;
  analyzer->SetCodaVersion(2);
  // analyzer->EnableBenchmarks(true);

  // A simple event class to be output to the resulting tree.
  // Creating your own descendant of THaEvent is one way of
  // defining and controlling the output.
  THaEvent* event = new THaEvent;

  // Define the run(s) that we want to analyze.
  // We just set up one, but this could be many.
  THcRun* run = new THcRun(pathList, fmt::format(RunFileNamePattern, RunNumber).c_str());

  // Set to read in Hall C run database parameters
  run->SetRunParamClass("THcRunParameters");

  // Eventually need to learn to skip over, or properly analyze the pedestal events
  run->SetEventRange(FirstEvent,
                     MaxEvent);  // Physics Event number, does not include scaler or control events.
  run->SetNscan(1);
  run->SetDataRequired(0x7);
  run->Print();

  // Define the analysis parameters
  const auto ROOTFileName = fmt::format(ROOTFileNamePattern, RunNumber, MaxEvent);
  analyzer->SetCountMode(2);  // 0 = counter is # of physics triggers
                              // 1 = counter is # of all decode reads
                              // 2 = counter is event number

  analyzer->SetEvent(event);
  // Set EPICS event type
  analyzer->SetEpicsEvtType(do_coin ? 182 : 181);
  // Define crate map
  analyzer->SetCrateMapFileName("MAPS/db_cratemap.dat");
  // Define output ROOT file
  analyzer->SetOutFile(ROOTFileName.c_str());
  // Define DEF-file+
  // analyzer->SetOdefFile("DEF-files/HMS/PRODUCTION/pstackana_production_all.def");
  analyzer->SetOdefFile(do_all ? "DEF-files/HMS/PRODUCTION/hstackana_production_all.def"
                               : "DEF-files/HMS/PRODUCTION/hstackana_production.def");
  // Define cuts file
  analyzer->SetCutFile("DEF-files/HMS/PRODUCTION/CUTS/hstackana_production_cuts.def");  // optional
  // File to record accounting information for cuts
  analyzer->SetSummaryFile(fmt::format(output_file_pattern("REPORT_OUTPUT/PRODUCTION", "summary",
                                                           "report", do_coin, do_all),
                                       RunNumber, MaxEvent)
                               .c_str());

  // --------------------------------------------------------------
  // post processors
  //
  // auto SHMS_scaler_yield_monitor = new
  // hallc::scandalizer::YieldMonitor(hscaler,"SHMS:EL_CLEAN","H.pEL_CLEAN.scaler","H.BCM2.scalerCharge");
  // analyzer->AddPostProcess(SHMS_scaler_yield_monitor);

  // hallc::PVList pv_list;
  // pv_list.AddPV("hcDAQMissingRefTime");
  // hallc::scandalizer::SimplePostProcess* daq_missing_ref_monitor =
  //    new hallc::scandalizer::SimplePostProcess(
  //        [&]() { return 0; },
  //        [&](const THaEvData* evt) {
  //          static int    counter = 0;
  //          static double total   = 0.2;
  //          total += hdc->fNTDCRef_miss + hdc->fNADCRef_miss;
  //          total += pdc->fNTDCRef_miss + pdc->fNADCRef_miss;
  //          total += hhod->fNTDCRef_miss + hhod->fNADCRef_miss;
  //          total += phod->fNTDCRef_miss + phod->fNADCRef_miss;
  //          total += hcer->fNTDCRef_miss + hcer->fNADCRef_miss;
  //          total += pngcer->fNTDCRef_miss + pngcer->fNADCRef_miss;
  //          total += phgcer->fNTDCRef_miss + phgcer->fNADCRef_miss;
  //          if ((evt->GetEvNum() > 1200) && (counter > 1000)) {
  //            counter = 0;
  //            pv_list.Put("hcDAQMissingRefTime", total);
  //          }
  //          counter++;
  //          return 0;
  //        });
  // analyzer->AddPostProcess(daq_missing_ref_monitor);

  auto ddisplay = new hallc::MonitoringDisplay();
  ddisplay->SetRootFolder(fmt::format("/online/hms/{}/",RunNumber));
  // ddisplay->_data._root_folder = "/detectors/";
  //hhodo_1x_good_adc_occ_pos","hhodo_1x_good_adc_occ_neg
  auto hod_plt1 = ddisplay->CreateDisplayPlot(
    "hod/", "HMS Hodoscope ADC Occ/Mult",
      [&](hallc::DisplayPlot& plt) {
        plt._plot_data._canvas = new TCanvas(plt.GetName().c_str(), plt.GetName().c_str());
        plt._plot_data._canvas->Divide(4, 2);
        // note that these two are part of the 50k online. They are pre defined, probably in a DEF file.
        //newpage  4 2
        //title HMS Hodoscope ADC Occ/Mult
        //macro 'UTIL/GEN/overlay2.C("hhodo_1x_good_adc_occ_pos","hhodo_1x_good_adc_occ_neg","1x+","1x-","1x Good ADC Occupancy")'
        //macro 'UTIL/GEN/overlay2.C("hhodo_1y_good_adc_occ_pos","hhodo_1y_good_adc_occ_neg","1y+","1y-","1y Good ADC Occupancy")'
        auto h_hod1 = (TH1F*)gROOT->FindObject("hhodo_1x_good_adc_occ_pos");
        auto h_hod2 = (TH1F*)gROOT->FindObject("hhodo_1x_good_adc_occ_neg");
        if (h_hod1 && h_hod2) {
          plt._plot_data._canvas->cd(1);
          plt._plot_data._hists1.push_back(h_hod1);
          plt._plot_data._hists1.push_back(h_hod2);
          gPad->SetLogy(true);
          h_hod1 ->SetTitle("1x+");
          h_hod2 ->SetTitle("1x-");
          h_hod1->Draw();
          h_hod2->SetLineColor(2);
          h_hod2->Draw("same");
          //plt._plot_data._hists1[0]->Draw();
          //plt._plot_data._hists1[1]->SetLineColor(2);
          //plt._plot_data._hists1[1]->Draw("same");
        } else { std::cout << "FAILED to find hists \n"; }
        //
        h_hod1 = (TH1F*)gROOT->FindObject("hhodo_1y_good_adc_occ_pos");
        h_hod2 = (TH1F*)gROOT->FindObject("hhodo_1y_good_adc_occ_neg");
        if (h_hod1 && h_hod2) {
          plt._plot_data._hists1.push_back(h_hod1);
          plt._plot_data._hists1.push_back(h_hod2);
          plt._plot_data._canvas->cd(2);
          gPad->SetLogy(true);
          h_hod1 ->SetTitle("1y+");
          h_hod2 ->SetTitle("1y-");
          h_hod1->Draw();
          h_hod2->SetLineColor(2);
          h_hod2->Draw("same");
        }
//macro 'UTIL/GEN/overlay2.C("hhodo_2x_good_adc_occ_pos","hhodo_2x_good_adc_occ_neg","2x+","2x-","2x Good ADC Occupancy")'
        h_hod1 = (TH1F*)gROOT->FindObject("hhodo_2x_good_adc_occ_pos");
        h_hod2 = (TH1F*)gROOT->FindObject("hhodo_2x_good_adc_occ_neg");
        if (h_hod1 && h_hod2) {
          plt._plot_data._hists1.push_back(h_hod1);
          plt._plot_data._hists1.push_back(h_hod2);
          plt._plot_data._canvas->cd(3);
          gPad->SetLogy(true);
          h_hod1 ->SetTitle("2x+");
          h_hod2 ->SetTitle("2x-");
          h_hod1->Draw();
          h_hod2->SetLineColor(2);
          h_hod2->Draw("same");
        }
//macro 'UTIL/GEN/overlay2.C("hhodo_2y_good_adc_occ_pos","hhodo_2y_good_adc_occ_neg","2y+","2y+","2y Good ADC Occupancy")'
        h_hod1 = (TH1F*)gROOT->FindObject("hhodo_2y_good_adc_occ_pos");
        h_hod2 = (TH1F*)gROOT->FindObject("hhodo_2y_good_adc_occ_neg");
        if (h_hod1 && h_hod2) {
          plt._plot_data._hists1.push_back(h_hod1);
          plt._plot_data._hists1.push_back(h_hod2);
          plt._plot_data._canvas->cd(4);
          gPad->SetLogy(true);
          h_hod1 ->SetTitle("2y+");
          h_hod2 ->SetTitle("2y-");
          h_hod1->Draw();
          h_hod2->SetLineColor(2);
          h_hod2->Draw("same");
        }
//macro 'UTIL/GEN/overlay2.C("hhodo_1x_good_adc_mult_pos","hhodo_1x_good_adc_mult_neg","1x+","1x-","1x Good ADC Multiplicity")'
        h_hod1 = (TH1F*)gROOT->FindObject("hhodo_1x_good_adc_mult_pos");
        h_hod2 = (TH1F*)gROOT->FindObject("hhodo_1x_good_adc_mult_neg");
        if (h_hod1 && h_hod2) {
          plt._plot_data._hists1.push_back(h_hod1);
          plt._plot_data._hists1.push_back(h_hod2);
          plt._plot_data._canvas->cd(5);
          gPad->SetLogy(true);
          h_hod1 ->SetTitle("1x+");
          h_hod2 ->SetTitle("1x-");
          h_hod1->Draw();
          h_hod2->SetLineColor(2);
          h_hod2->Draw("same");
        }
//macro 'UTIL/GEN/overlay2.C("hhodo_1y_good_adc_mult_pos","hhodo_1y_good_adc_mult_neg","1y+","1y-","1y Good ADC Multiplicity")'
        h_hod1 = (TH1F*)gROOT->FindObject("hhodo_1y_good_adc_mult_pos");
        h_hod2 = (TH1F*)gROOT->FindObject("hhodo_1y_good_adc_mult_neg");
        if (h_hod1 && h_hod2) {
          plt._plot_data._hists1.push_back(h_hod1);
          plt._plot_data._hists1.push_back(h_hod2);
          plt._plot_data._canvas->cd(6);
          gPad->SetLogy(true);
          h_hod1 ->SetTitle("1y+");
          h_hod2 ->SetTitle("1y-");
          h_hod1->Draw();
          h_hod2->SetLineColor(2);
          h_hod2->Draw("same");
        }
//macro 'UTIL/GEN/overlay2.C("hhodo_2x_good_adc_mult_pos","hhodo_2x_good_adc_mult_neg","2x+","2x-","2x Good ADC Multiplicity")'
        h_hod1 = (TH1F*)gROOT->FindObject("hhodo_2x_good_adc_mult_pos");
        h_hod2 = (TH1F*)gROOT->FindObject("hhodo_2x_good_adc_mult_neg");
        if (h_hod1 && h_hod2) {
          plt._plot_data._hists1.push_back(h_hod1);
          plt._plot_data._hists1.push_back(h_hod2);
          plt._plot_data._canvas->cd(7);
          gPad->SetLogy(true);
          h_hod1 ->SetTitle("2x+");
          h_hod2 ->SetTitle("2x-");
          h_hod1->Draw();
          h_hod2->SetLineColor(2);
          h_hod2->Draw("same");
        }
//macro 'UTIL/GEN/overlay2.C("hhodo_2y_good_adc_mult_pos","hhodo_2y_good_adc_mult_neg","2y+","2y-","2y Good ADC Multiplicity")'
        h_hod1 = (TH1F*)gROOT->FindObject("hhodo_2y_good_adc_mult_pos");
        h_hod2 = (TH1F*)gROOT->FindObject("hhodo_2y_good_adc_mult_neg");
        if (h_hod1 && h_hod2) {
          plt._plot_data._hists1.push_back(h_hod1);
          plt._plot_data._hists1.push_back(h_hod2);
          plt._plot_data._canvas->cd(8);
          gPad->SetLogy(true);
          h_hod1 ->SetTitle("2y+");
          h_hod2 ->SetTitle("2y-");
          h_hod1->Draw();
          h_hod2->SetLineColor(2);
          h_hod2->Draw("same");
          gPad->BuildLegend();
        }
        return 0;
      },
      [&](hallc::DisplayPlot& plt) {
        return 0;
      });

  auto plt1 = ddisplay->CreateDisplayPlot(
      "dc/", "HMS_DC_Occupancy",
      [&](hallc::DisplayPlot& plt) {
        plt._plot_data._canvas = new TCanvas(plt.GetName().c_str(), plt.GetName().c_str());
        plt._plot_data._hists1 = {new TH1F("h_dc1_occ", "h_dc1_occ", 30, 0, 30),
                                  new TH1F("h_dc2_occ", "h_dc2_occ", 30, 0, 30)};
        plt._plot_data._canvas->Divide(2, 1);
        plt._plot_data._canvas->cd(1);
        plt._plot_data._hists1[0]->Draw();
        plt._plot_data._canvas->cd(2);
        plt._plot_data._hists1[1]->SetLineColor(2);
        plt._plot_data._hists1[1]->Draw();
        return 0;
      },
      [&](hallc::DisplayPlot& plt) {
        int hmsDC1Planes_nhits = 0;
        int hmsDC2Planes_nhits = 0;
        for (int ip = 0; ip < 6; ip++) {
          hmsDC1Planes_nhits += hdc->GetPlane(ip)->GetNHits();
        }
        for (int ip = 6; ip < 12; ip++) {
          hmsDC2Planes_nhits += hdc->GetPlane(ip)->GetNHits();
        }
        plt._plot_data._hists1[0]->Fill(hmsDC1Planes_nhits);
        plt._plot_data._hists1[1]->Fill(hmsDC2Planes_nhits);
        return 0;
      });

  SimplePostProcess* hms_dc_occ_pproc =
      new SimplePostProcess(
          // init lambda function
          [&]() {
            ddisplay->InitAll(); // sends histograms to display server
            ddisplay->UpdateAll(); // sends histograms to display server
            return 0;
          },
          // event process lambda function
          [&](const THaEvData* evt) {
            ddisplay->Process();
            static int counter = 0;
            if ((evt->GetEvNum() > 500) && (counter > 500)) {
              counter = 0;
              ddisplay->UpdateAll(); // sends histograms to display server
            }
            counter++;
            return 0;
          });
  analyzer->AddPostProcess(hms_dc_occ_pproc);

  // Start the actual analysis.
  analyzer->Process(run);
  ddisplay->UpdateAll(); // sends histograms to display server
  // Create report file from template
  analyzer->PrintReport(
      "TEMPLATES/HMS/PRODUCTION/hstackana_production.template",
      fmt::format(output_file_pattern("REPORT_OUTPUT/PRODUCTION", "replay_production", "report",
                                      do_coin, do_all),
                  RunNumber, MaxEvent)
          .c_str());

  delete analyzer;

  return 0;
}
