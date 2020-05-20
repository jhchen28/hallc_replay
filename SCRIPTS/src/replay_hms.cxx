#include <chrono>
#include <iostream>
#include <locale>
#include <string>
using namespace std;

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
#include "THcHelicityScaler.h"
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

std::string coda_file_pattern(bool do_coin) {
  return fmt::format("{}_all_{{:05d}}.dat", do_coin ? "coin" : "hms");
}
std::string output_file_pattern(string_view path, string_view content, string_view extension,
                                string_view mode, const bool do_coin) {
  return fmt::format("{}/hms{}_{}_{}_{{:05d}}_{{}}.{}", path, do_coin ? "_coin" : "", content, mode,
                     extension);
}

int replay_hms(
    Int_t RunNumber = 2468, Int_t MaxEvent = -1, Int_t FirstEvent = 0,
    const std::string& mode      = "default",
    const std::string& odef_file = "DEF-files/HMS/PRODUCTION/hstackana_production.def",
    const std::string& cut_file  = "DEF-files/HMS/PRODUCTION/CUTS/hstackana_production_cuts.def",
    const bool         do_coin   = false) {
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
  pathList.push_back("./raw");
  pathList.push_back("./raw/../raw.copiedtotape");
  pathList.push_back("./cache");
  // 2. Output files
  const auto ROOTFileNamePattern =
      output_file_pattern("ROOTfiles", "replay_production", "root", mode, do_coin);

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

  // helicity scaler
  auto helscaler = new THcHelicityScaler("H", "Hall C helicity scalers");
  helscaler->SetROC(5);
  gHaEvtHandlers->Add(helscaler);

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
  // Set up the analyzer - we use the standard one,
  // but this could be an experiment-specific one as well.
  // The Analyzer controls the reading of the data, executes
  // tests/cuts, loops over Acpparatus's and PhysicsModules,
  // and executes the output routines.
  THcAnalyzer* analyzer = new THcAnalyzer;
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
  analyzer->SetOdefFile(odef_file.c_str());
  // Define cuts file
  analyzer->SetCutFile(cut_file.c_str());  // optional
  // File to record accounting information for cuts
  analyzer->SetSummaryFile(
      fmt::format(output_file_pattern("REPORT_OUTPUT/" + mode, "summary", "report", mode, do_coin),
                  RunNumber, MaxEvent)
          .c_str());
  // Start the actual analysis.
  analyzer->Process(run);
  // Create report file from template
  analyzer->PrintReport(
      "TEMPLATES/HMS/PRODUCTION/hstackana_production.template",
      fmt::format(output_file_pattern("REPORT_OUTPUT/" + mode, "replay_production", "report", mode,
                                      do_coin),
                  RunNumber, MaxEvent)
          .c_str());

  delete analyzer;

  return 0;
}
