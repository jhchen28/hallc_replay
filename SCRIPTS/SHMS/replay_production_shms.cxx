#include <fmt/core.h>
#include <fmt/ostream.h>
R__LOAD_LIBRARY(libfmt.so)

#include <chrono>
#include <iostream>

using namespace std;

#include "spdlog/spdlog.h"

R__LOAD_LIBRARY(libHallC.so)
#include "hcana/HallC_Data.h"

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
void replay_production_shms(Int_t RunNumber = 7160, Int_t MaxEvent = 50000) {
  // ===========================================================================
  // Setup logging
  spdlog::set_level(spdlog::level::warn);
  spdlog::flush_every(std::chrono::seconds(5));

  // ===========================================================================
  // Get RunNumber and MaxEvent if not provided.
  if (RunNumber == 0) {
    cout << "Enter a Run Number (-1 to exit): ";
    if (RunNumber <= 0)
      return;
  }
  // note: MaxEvent equal to -1 means all events
  if (MaxEvent == 0) {
    cout << "\nNumber of Events to analyze: ";
    cin >> MaxEvent;
    if (MaxEvent == 0) {
      cerr << "...Invalid entry\n";
      exit;
    }
  }

  // ===========================================================================
  // Create file name patterns.
  //
  // 1. Input files
  const char*     RunFileNamePattern = "shms_all_%05d.dat";
  vector<TString> pathList;
  pathList.push_back(".");
  pathList.push_back("./DATA/raw");
  pathList.push_back("./raw_coda");
  pathList.push_back("./raw");
  pathList.push_back("./raw.copiedtotape");
  pathList.push_back("./raw/../raw.copiedtotape");
  pathList.push_back("./cache");
  // 2. Output files
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

  // ===========================================================================
  // Experimental apparatus
  //
  // ---------------------------------------------------------------------------
  // A. SHMS setup
  //
  // Set up the equipment to be analyzed.
  THcHallCSpectrometer* SHMS = new THcHallCSpectrometer("P", "SHMS");
  SHMS->SetEvtType(1);
  SHMS->AddEvtType(4);
  SHMS->AddEvtType(5);
  SHMS->AddEvtType(6);
  SHMS->AddEvtType(7);
  gHaApps->Add(SHMS);
  // 1. Add Noble Gas Cherenkov to SHMS apparatus
  THcCherenkov* pngcer = new THcCherenkov("ngcer", "Noble Gas Cherenkov");
  SHMS->AddDetector(pngcer);
  // 2. Add drift chambers to SHMS apparatus
  THcDC* pdc = new THcDC("dc", "Drift Chambers");
  SHMS->AddDetector(pdc);
  // 3. Add hodoscope to SHMS apparatus
  THcHodoscope* phod = new THcHodoscope("hod", "Hodoscope");
  SHMS->AddDetector(phod);
  // 4. Add Heavy Gas Cherenkov to SHMS apparatus
  THcCherenkov* phgcer = new THcCherenkov("hgcer", "Heavy Gas Cherenkov");
  SHMS->AddDetector(phgcer);
  // 5. Add Aerogel Cherenkov to SHMS apparatus
  THcAerogel* paero = new THcAerogel("aero", "Aerogel");
  SHMS->AddDetector(paero);
  // 6. Add calorimeter to SHMS apparatus
  THcShower* pcal = new THcShower("cal", "Calorimeter");
  SHMS->AddDetector(pcal);
  // ---------------------------------------------------------------------------
  // B. Beamline
  //
  // Add rastered beam apparatus
  THaApparatus* pbeam = new THcRasteredBeam("P.rb", "Rastered Beamline");
  gHaApps->Add(pbeam);

  // ---------------------------------------------------------------------------
  // C. Trigger
  //
  // Add trigger detector to trigger apparatus
  THaApparatus* TRG = new THcTrigApp("T", "TRG");
  gHaApps->Add(TRG);
  // TODO Add helicity detector to trigger apparatus
  // THcHelicity* helicity = new THcHelicity("helicity","Helicity Detector");
  // TRG->AddDetector(helicity);

  // ===========================================================================
  // Phyics and derived quantities
  //
  // 1. Calculate reaction point
  THaReactionPoint* prp = new THaReactionPoint("P.react", "SHMS reaction point", "P", "P.rb");
  gHaPhysics->Add(prp);
  // 2. Calculate extended target corrections
  THcExtTarCor* pext =
      new THcExtTarCor("P.extcor", "HMS extended target corrections", "P", "P.react");
  gHaPhysics->Add(pext);
  // 3. Calculate golden track quantites
  THaGoldenTrack* pgtr = new THaGoldenTrack("P.gtr", "SHMS Golden Track", "P");
  gHaPhysics->Add(pgtr);
  // 4. Calculate the hodoscope efficiencies
  THcHodoEff* peff = new THcHodoEff("phodeff", "SHMS hodo efficiency", "P.hod");
  gHaPhysics->Add(peff);
  // 5. Single arm kinematics
  THcPrimaryKine* kin = new THcPrimaryKine("P.kin", "SHMS Single Arm Kinematics", "P", "P.rb");
  gHaPhysics->Add(kin);

  // ===========================================================================
  //  Global Objects & Event Handlers
  //
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

  // Add event handler for prestart event 125.
  THcConfigEvtHandler* ev125 = new THcConfigEvtHandler("HC", "Config Event type 125");
  gHaEvtHandlers->Add(ev125);
  // Add event handler for EPICS events
  THaEpicsEvtHandler* hcepics = new THaEpicsEvtHandler("epics", "HC EPICS event type 180");
  gHaEvtHandlers->Add(hcepics);

  // -----------------------------------------------------------
  // Analyzer
  // -----------------------------------------------------------
  //
  THcAnalyzer* analyzer = new THcAnalyzer;
  analyzer->SetCodaVersion(2);
  // analyzer->EnableBenchmarks(true);

  // A simple event class to be output to the resulting tree.
  // Creating your own descendant of THaEvent is one way of
  // defining and controlling the output.
  THaEvent* event = new THaEvent;

  // Define the run(s) that we want to analyze.
  // We just set up one, but this could be many.
  THcRun* run = new THcRun(pathList, Form(RunFileNamePattern, RunNumber));

  // Set to read in Hall C run database parameters
  run->SetRunParamClass("THcRunParameters");

  // Eventually need to learn to skip over, or properly analyze the pedestal events
  run->SetEventRange(1,
                     MaxEvent);  // Physics Event number, does not include scaler or control events.
  run->SetNscan(1);
  run->SetDataRequired(0x7);
  // run->Print();

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
  // analyzer->SetOdefFile("DEF-files/SHMS/PRODUCTION/pstackana_production_all.def");
  analyzer->SetOdefFile("DEF-files/SHMS/PRODUCTION/pstackana_production.def");
  // Define cuts file
  analyzer->SetCutFile("DEF-files/SHMS/PRODUCTION/CUTS/pstackana_production_cuts.def");  // optional
  // File to record accounting information for cuts
  analyzer->SetSummaryFile(Form("REPORT_OUTPUT/SHMS/PRODUCTION/summary_all_production_%d_%d.report",
                                RunNumber, MaxEvent));  // optional
  // Start the actual analysis.
  analyzer->Process(run);
  // Create report file from template
  analyzer->PrintReport(
      "TEMPLATES/SHMS/PRODUCTION/pstackana_production.template",
      Form("REPORT_OUTPUT/SHMS/PRODUCTION/replay_shms_all_production_%d_%d.report", RunNumber,
           MaxEvent));  // optional

  delete analyzer;
}
