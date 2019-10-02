#include <fmt/core.h>
#include <fmt/ostream.h>
R__LOAD_LIBRARY(libfmt.so)

#include <chrono>
#include <iostream>

using namespace std;

#include "spdlog/spdlog.h"

R__LOAD_LIBRARY(libHallC.so)
#include "hcana/HallC_Data.h"

#include "THcAnalyzer.h"
#include "THaCut.h"
#include "THcGlobals.h"
#include "THcHallCSpectrometer.h"
#include "THcDetectorMap.h"
#include "THcCherenkov.h"
#include "THcDC.h"
#include "THcHodoscope.h"
#include "THcParmList.h"
#include "THaGoldenTrack.h"
#include "THcHodoEff.h"
#include "THcScalerEvtHandler.h"
#include "THcShower.h"
#include "THaReactionPoint.h"
#include "THcExtTarCor.h"
#include "THcRasteredBeam.h"
#include "THcRun.h"
#include "THcCoinTime.h"
#include "THcConfigEvtHandler.h"
#include "THcTrigDet.h"
#include "THcTrigApp.h"
#include "THcSecondaryKine.h"
#include "THcAerogel.h"
#include "THcPrimaryKine.h"
void replay_production_shms(Int_t RunNumber = 7160, Int_t MaxEvent = 50000) {

  //hallc::helper::script_requires_hcana();
  //{
  //  std::string process_name = "";
  //  if (const char* env_p = std::getenv("_")) {
  //    process_name = env_p ;
  //  }
  //  std::cout << "running program : " << process_name << "\n";
  //  auto last_part = process_name.substr(process_name.size()-4,4);
  //  std::cout << last_part << "\n";
  //  if( last_part == "root" ) {
  //    std::cout << " You are running this script with root!\n";
  //    std::cout << " Use hcana instead. \n";
  //    std::quick_exit(1);
  //  }
  //}
  spdlog::set_level(spdlog::level::warn);
  spdlog::flush_every(std::chrono::seconds(5));


  // Get RunNumber and MaxEvent if not provided.
  if(RunNumber == 0) {
    cout << "Enter a Run Number (-1 to exit): ";
    if( RunNumber<=0 ) return;
  }
  if(MaxEvent == 0) {
    cout << "\nNumber of Events to analyze: ";
    cin >> MaxEvent;
    if(MaxEvent == 0) {
      cerr << "...Invalid entry\n";
      exit;
    }
  }

  // Create file name patterns.
  const char* RunFileNamePattern = "shms_all_%05d.dat";
  vector<TString> pathList;
  pathList.push_back(".");
  pathList.push_back("./DATA/raw");
  pathList.push_back("./raw_coda");
  pathList.push_back("./raw");
  pathList.push_back("./raw.copiedtotape");
  pathList.push_back("./raw/../raw.copiedtotape");
  pathList.push_back("./cache");

  //const char* RunFileNamePattern = "raw/coin_all_%05d.dat";
  const char* ROOTFileNamePattern = "ROOTfiles/shms_replay_production_%d_%d.root";

  // Load global parameters
  gHcParms->Define("gen_run_number", "Run Number", RunNumber);
  gHcParms->AddString("g_ctp_database_filename", "DBASE/SHMS/standard.database");
  gHcParms->Load(gHcParms->GetString("g_ctp_database_filename"), RunNumber);
  gHcParms->Load(gHcParms->GetString("g_ctp_parm_filename"));
  gHcParms->Load(gHcParms->GetString("g_ctp_kinematics_filename"), RunNumber);
  // Load parameters for SHMS trigger configuration
  gHcParms->Load("PARAM/TRIG/tshms.param");
  // Load fadc debug parameters
  gHcParms->Load("PARAM/SHMS/GEN/p_fadc_debug.param");

  // Load the Hall C detector map
  gHcDetectorMap = new THcDetectorMap();
  gHcDetectorMap->Load("MAPS/SHMS/DETEC/STACK/shms_stack.map");
  
  // ========
  //  SHMS 
  // ========

  // Set up the equipment to be analyzed.
  THcHallCSpectrometer* SHMS = new THcHallCSpectrometer("P", "SHMS");
  SHMS->SetEvtType(1);
  SHMS->AddEvtType(4);
  SHMS->AddEvtType(5);
  SHMS->AddEvtType(6);
  SHMS->AddEvtType(7);
  gHaApps->Add(SHMS);
  // Add Noble Gas Cherenkov to SHMS apparatus
  THcCherenkov* pngcer = new THcCherenkov("ngcer", "Noble Gas Cherenkov");
  SHMS->AddDetector(pngcer);
  // Add drift chambers to SHMS apparatus
  THcDC* pdc = new THcDC("dc", "Drift Chambers");
  SHMS->AddDetector(pdc);
  // Add hodoscope to SHMS apparatus
  THcHodoscope* phod = new THcHodoscope("hod", "Hodoscope");
  SHMS->AddDetector(phod);
  // Add Heavy Gas Cherenkov to SHMS apparatus
  THcCherenkov* phgcer = new THcCherenkov("hgcer", "Heavy Gas Cherenkov");
  SHMS->AddDetector(phgcer);
  // Add Aerogel Cherenkov to SHMS apparatus
  THcAerogel* paero = new THcAerogel("aero", "Aerogel");
  SHMS->AddDetector(paero);
  // Add calorimeter to SHMS apparatus
  THcShower* pcal = new THcShower("cal", "Calorimeter");
  SHMS->AddDetector(pcal);

  // Add rastered beam apparatus
  THaApparatus* pbeam = new THcRasteredBeam("P.rb", "Rastered Beamline");
  gHaApps->Add(pbeam);

  auto trkEff = new hcana::TrackingEfficiency("P.trackEfficiency", "SHMS tracking Efficiency", "P.hod");
  gHaPhysics->Add(trkEff);
  // Add physics modules
  // Calculate reaction point
  THaReactionPoint* prp = new THaReactionPoint("P.react", "SHMS reaction point", "P", "P.rb");
  gHaPhysics->Add(prp);
  // Calculate extended target corrections
  THcExtTarCor* pext = new THcExtTarCor("P.extcor", "HMS extended target corrections", "P", "P.react");
  gHaPhysics->Add(pext);
  // Calculate golden track quantites
  THaGoldenTrack* pgtr = new THaGoldenTrack("P.gtr", "SHMS Golden Track", "P");
  gHaPhysics->Add(pgtr);
  // Calculate the hodoscope efficiencies
  THcHodoEff* peff = new THcHodoEff("phodeff", "SHMS hodo efficiency", "P.hod");
  gHaPhysics->Add(peff);   

  // Add event handler for scaler events
  THcScalerEvtHandler* pscaler = new THcScalerEvtHandler("P", "Hall C scaler event type 1");
  pscaler->AddEvtType(1);
  pscaler->AddEvtType(4);
  pscaler->AddEvtType(5);
  pscaler->AddEvtType(6);
  pscaler->AddEvtType(7);
  pscaler->AddEvtType(129);
  pscaler->SetDelayedType(129);
  pscaler->SetUseFirstEvent(kTRUE);
  gHaEvtHandlers->Add(pscaler);


  // =================================
  //  Kinematics Modules
  // =================================

  // Add Physics Module to calculate primary (scattered electrons) beam kinematics
  //THcPrimaryKine* hkin_primary = new THcPrimaryKine("H.kin.primary", "HMS Single Arm Kinematics", "H", "H.rb");
  //gHaPhysics->Add(hkin_primary);
  // Add Physics Module to calculate secondary (scattered hadrons) beam kinematics
  //THcSecondaryKine* pkin_secondary = new THcSecondaryKine("P.kin.secondary", "SHMS Single Arm Kinematics", "P", "H.kin.primary");
  //gHaPhysics->Add(pkin_secondary);
  THcPrimaryKine* kin = new THcPrimaryKine("P.kin", "SHMS Single Arm Kinematics", "P", "P.rb");
  gHaPhysics->Add(kin);
  
  // =================================
  //  Global Objects & Event Handlers
  // =================================

  // Add trigger apparatus
  THaApparatus* TRG = new THcTrigApp("T", "TRG");
  gHaApps->Add(TRG);
  // Add trigger detector to trigger apparatus
  //THcTrigDet* coin = new THcTrigDet("coin", "Coincidence Trigger Information");
  // Suppress missing reference time warnings for these event types
  //coin->SetEvtType(1);
  //coin->AddEvtType(2);
  //TRG->AddDetector(coin); 

  // Add helicity detector to grigger apparatus
  THcHelicity* helicity = new THcHelicity("helicity","Helicity Detector");
  TRG->AddDetector(helicity);

  // Add event handler for prestart event 125.
  THcConfigEvtHandler* ev125 = new THcConfigEvtHandler("HC", "Config Event type 125");
  gHaEvtHandlers->Add(ev125);
  // Add event handler for EPICS events
  THaEpicsEvtHandler* hcepics = new THaEpicsEvtHandler("epics", "HC EPICS event type 180");
  gHaEvtHandlers->Add(hcepics);

  // Add coin physics module 
  //THcCoinTime* coinTime = new THcCoinTime("CTime", "Coincidende Time Determination", "P", "H", "T.coin");
  //gHaPhysics->Add(coinTime);
 
  // -----------------------------------------------------------
  // Scandalizer 
  // -----------------------------------------------------------
  //
  hcana::Scandalizer* analyzer = new hcana::Scandalizer;
  //analyzer->_skip_events = 100;
  analyzer->SetCodaVersion(2);
  //analyzer->EnableBenchmarks(true);

  // The following analyzes the first 2000 events (for pedestals, is required) 
  // then  repeatedly skips 3000 events and processes 1000.
  //auto pp0 = new hallc::scandalizer::SkipPeriodicAfterPedestal();
  //auto pp0 = new hallc::scandalizer::SkipAfterPedestal();
  //pp0->_analyzer = analyzer;

  //SimplePostProcess([&]() { return 0; },
  //                                                     [&](const THaEvData* evt) {
  //                                                       static int counter = 0;
  //                                                       if (evt->GetEvNum() > 2000) {
  //                                                         if (counter == 0) {
  //                                                           analyzer->_skip_events = 3000;
  //                                                           counter                = 1000;
  //                                                         } else {
  //                                                           counter--;
  //                                                         }
  //                                                       }
  //                                                       return 0;
  //                                                     });

  //hallc::scandalizer::SpectrometerMonitor * pp1a = new hallc::scandalizer::SpectrometerMonitor(hhod,hcer,hdc);
  //pp1a->_analyzer = analyzer;
  //pp1a->_spectrometer_name = "HMS";

  //hallc::scandalizer::SpectrometerMonitor * pp1 = new hallc::scandalizer::SpectrometerMonitor(phod,phgcer,pdc);
  //pp1->_analyzer = analyzer;

  //hallc::scandalizer::SimplePostProcess* pp1 = new hallc::scandalizer::SimplePostProcess(
  //  [&](){
  //    return 0;
  //  },
  //  [&](const THaEvData* evt){
  //    static int counter = 0;
  //    static double eff_num             = 0.0000001;
  //    static double eff_den             = 0.0;
  //    static int    n_num               = 0;
  //    static int    n_den               = 0;
  //    int           shmsDC1Planes_nhits = 0;
  //    int           shmsDC2Planes_nhits = 0;
  //    for (int ip = 0; ip < 6; ip++) {
  //      shmsDC1Planes_nhits += pdc->GetPlane(ip)->GetNHits();
  //    }
  //    for (int ip = 6; ip < 12; ip++) {
  //      shmsDC2Planes_nhits += pdc->GetPlane(ip)->GetNHits();
  //    }
  //    bool   shms_DC_too_many_hits = (shmsDC1Planes_nhits > 8) || (shmsDC2Planes_nhits > 8);
  //    double beta                  = phod->GetBetaNotrk();
  //    bool   good_beta             = beta > 0.4;
  //    bool   shms_good_hodoscope   = phod->fGoodScinHits;
  //    bool   good_ntracks          = (pdc->GetNTracks() > 0);
  //    bool   good_hgc              = phgcer->GetCerNPE() > 1;
  //    if ((good_beta && shms_good_hodoscope) && (!shms_DC_too_many_hits) && good_hgc) {
  //      eff_den = eff_den + 1.0;
  //      n_den++;
  //      if (good_ntracks) {
  //        eff_num = eff_num + 1.0;
  //        n_num++;
  //      }
  //    }
  //    if ((evt->GetEvNum() > 1200) && (counter > 1000)) {
  //      std::cout << " efficiency :  " << eff_num / eff_den << "\n";
  //      //std::cout << " Event : " << evt->GetEvNum() << "  ( " << evt->GetEvType() << ")\n";
  //      pv_list.Put("hcSHMSTrackingEff", eff_num/eff_den);
  //      pv_list.Put("hcSHMSTrackingEff:Unc",std::sqrt(double(n_num))/(n_num+n_den+0.0000001));
  //      pv_list.Put("hcSHMSTrackingEff.LOW",0.95);
  //      pv_list.Put("hcSHMSTrackingEff.LOLO",0.93);

  //      eff_num                = 0.000000001;
  //      eff_den                = 0.0;
  //      //analyzer->_skip_events = 300;
  //      counter = 0;
  //    }
  //    counter++;
  //    return 0; 
  //  });

  //analyzer->AddPostProcess(pp0);
  //analyzer->AddPostProcess(pp1);
  //analyzer->AddPostProcess(pp1a);

  // A simple event class to be output to the resulting tree.
  // Creating your own descendant of THaEvent is one way of
  // defining and controlling the output.
  THaEvent* event = new THaEvent;

  // Define the run(s) that we want to analyze.
  // We just set up one, but this could be many.
  THcRun* run = new THcRun( pathList, Form(RunFileNamePattern, RunNumber) );

  // Set to read in Hall C run database parameters
  run->SetRunParamClass("THcRunParameters");
  
  // Eventually need to learn to skip over, or properly analyze the pedestal events
  run->SetEventRange(1, MaxEvent); // Physics Event number, does not include scaler or control events.
  run->SetNscan(1);
  run->SetDataRequired(0x7);
  //run->Print();

  // Define the analysis parameters
  TString ROOTFileName = Form(ROOTFileNamePattern, RunNumber, MaxEvent);
  analyzer->SetCountMode(2);  // 0 = counter is # of physics triggers
                              // 1 = counter is # of all decode reads
                              // 2 = counter is event number

  analyzer->SetEvent(event);
  // Set EPICS event type
  analyzer->SetEpicsEvtType(180);
  // Define crate map
  analyzer->SetCrateMapFileName("MAPS/db_cratemap.dat");
  // Define output ROOT file
  analyzer->SetOutFile(ROOTFileName.Data());
  // Define DEF-file+
  //analyzer->SetOdefFile("DEF-files/SHMS/PRODUCTION/pstackana_production_all.def");
  analyzer->SetOdefFile("DEF-files/SHMS/PRODUCTION/pstackana_production.def");
  // Define cuts file
  analyzer->SetCutFile("DEF-files/SHMS/PRODUCTION/CUTS/pstackana_production_cuts.def");  // optional
  // File to record accounting information for cuts
  analyzer->SetSummaryFile(Form("REPORT_OUTPUT/SHMS/PRODUCTION/summary_all_production_%d_%d.report", RunNumber, MaxEvent));  // optional
  // Start the actual analysis.
  analyzer->Process(run);
  // Create report file from template
  analyzer->PrintReport("TEMPLATES/SHMS/PRODUCTION/pstackana_production.template",
  			Form("REPORT_OUTPUT/SHMS/PRODUCTION/replay_shms_all_production_%d_%d.report", RunNumber, MaxEvent));  // optional


  delete analyzer;
}
