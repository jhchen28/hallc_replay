// Filename: SIDIS_kine_compare.C
// Description: W, Q2, x , z_had, MM, pt & phi_had distributions are compared between data and SIMC 
// Needs number of runs, particle in SHMS, SIMC filename and normfac


//void SIDIS_kine_compare(Int_t runnumber, Int_t targ=1, Int_t pisign=1, Int_t kingrp=1, Int_t ptgrp=1){
void SIDIS_kine_compare_wds(Int_t numruns, Int_t runnumber,  Int_t targ, Int_t pisign, Int_t kingrp, Int_t ptgrp){
                     

  TString ROOT_FILE_PATH = "/net/cdaq/cdaql3data/cdaq/hallc-online/ROOTfiles/";
  TString SIMC_ROOT_FILE_PATH = "/net/cdaq/cdaql3data/cdaq/hallc-online/SIMC_ROOTfiles/";
  TString fileNameM = SIMC_ROOT_FILE_PATH;
  Int_t runnum1 = runnumber;
  Int_t runnum2 = runnumber + numruns-1;
  /////// Rootfiles for dummy runs ///
  Int_t numdummy = 2; // runmber of dummy runs
    Int_t dummyRun[2]={3500, 3501} ;  // run number of the dummy runs for kin1-10, 
  //Int_t dummyRun[2]={3620, 3621} ; // for kin2-10 3620, 3621 
  Double_t d_chr_ratio =-0.246*1.22*(74.3/15.8); // charge ratio of regular to dummy runs, for kin 1-10 LH2 = 122 mC, LD2 = 74.3 mC, dummy=15.8 mC, minus because it is to be subtracted. 0.246 = ratio of (LD2 entrance+exit)/(dummy entrance+exit thicknesses) and 1.22 = ratio of product of livetime and HMS and SHMS tracking effs (dummy/LD2).
  //  Double_t d_chr_ratio =-0.33*(159.6/74.1);
  // for kin 2-10 LH2 = 159.6 mC, LD2 = 163.1 mC and dummy = 74.1 mC

  if (targ == 1) {
      fileNameM += "sidis_h_";
  }
  else if (targ == 2) {
      fileNameM += "sidis_";
  }
  if (pisign == 1){
   fileNameM += "pip"; //read the root file from SIMC
  }
  else if (pisign == 2){
   fileNameM += "pim"; //read the root file from SIMC
  }
  fileNameM +=kingrp;
  fileNameM += "_pt";
  fileNameM +=ptgrp;
  fileNameM +=".root";

  TFile *f2 = new TFile(fileNameM);
  TTree *ts = (TTree*)f2->Get("h666");
  //get the relevant branch
  int nentriesM = ts->GetEntries();
  cout<<"Simc Entries:\t"<<nentriesM<<endl;
  

  gROOT->SetBatch(kTRUE);
  gROOT->ForceStyle(); 
  //  gStyle->SetPalette(kOcean);
  gStyle->SetOptStat(2);

  Double_t pionMass = 0.1395701835;    //Gev/c^2    
  Double_t z_had, E_pi, diff_p, ptd, MissMass2d, pKinW_2, pkinW;                         
   

  
  //make histograms:
  TH1D *h1_epi_PcointimeROC2    = new TH1D("SHMS ROC2 Corrected epi Coin Time","SHMS ROC2 Corrected epi Coin Time; cointime [ns]",       480, -24, 24); 
  TH1D *h1_epi_PcointimeROC2_C  = new TH1D("SHMS ROC2 Corrected epi Coin Time_C","SHMS ROC2 Corrected epi Coin Time_C; cointime [ns]",   480, -24, 24); 
  TH1D *h1_epi_PcointimeROC2_B  = new TH1D("SHMS ROC2 Corrected epi Coin Time_B","SHMS ROC2 Corrected epi Coin Time_B; cointime [ns]",   480, -24, 24); 
  TH1D *h1_depi_PcointimeROC2    = new TH1D("SHMS ROC2 Corrected depi Coin Time","SHMS ROC2 Corrected depi Coin Time; cointime [ns]",       480, -24, 24); 
  TH1D *h1_depi_PcointimeROC2_C  = new TH1D("SHMS ROC2 Corrected depi Coin Time_C","SHMS ROC2 Corrected depi Coin Time_C; cointime [ns]",   480, -24, 24); 
  TH1D *h1_depi_PcointimeROC2_B  = new TH1D("SHMS ROC2 Corrected depi Coin Time_B","SHMS ROC2 Corrected depi Coin Time_B; cointime [ns]",   480, -24, 24); 


  TH1D *h_Q2d = new TH1D("h_Q2d","Q2 (GeV2)",100,1,6);  //Q2
  TH1D *h_Q2s = new TH1D("h_Q2s","Q2 (GeV2)",100,1,6);
  TH1D *h_Q2a = new TH1D("h_Q2a","Q2 (GeV2)",100,1,6); // for accidentals
  TH1D *h_Q2ds = new TH1D("h_Q2ds","Q2 (GeV2)",100,1,6); // for dummy subtraction
  TH1D *h_Q2da = new TH1D("h_Q2da","Q2 (GeV2)",100,1,6); // for accidentals

  TH1D *h_W2d = new TH1D("h_W2d","W2 (GeV2)", 150, 5, 11.0);// W2
  TH1D *h_W2s = new TH1D("h_W2s","W2 (GeV2)", 150, 5, 11.0);
  TH1D *h_W2a = new TH1D("h_W2a","W2 (GeV2)", 150, 5, 11.0);
  TH1D *h_W2ds = new TH1D("h_W2ds","W2 (GeV2)", 150, 5, 11.0);
  TH1D *h_W2da = new TH1D("h_W2da","W2 (GeV2)", 150, 5, 11.0);

  TH1D *h_xd = new TH1D("h_xd","X_bj", 100,0.1, 0.6);  
  TH1D *h_xs = new TH1D("h_xs","X_bj", 100,0.1, 0.6);  
  TH1D *h_xa = new TH1D("h_xa","X_bj", 100,0.1, 0.6);  
  TH1D *h_xds = new TH1D("h_xds","X_bj", 100,0.1, 0.6); 
  TH1D *h_xda = new TH1D("h_xda","X_bj", 100,0.1, 0.6);  

  if (ptgrp == 1 || ptgrp == 2){
   TH1D *h_zd = new TH1D("h_zd","z_had", 100,0.7, 1.1); 
   TH1D *h_zs = new TH1D("h_zs","z_had", 100,0.7, 1.1); 
   TH1D *h_za = new TH1D("h_za","z_had", 100,0.7, 1.1); 
   TH1D *h_zds = new TH1D("h_zds","z_had", 100,0.7, 1.1); 
   TH1D *h_zda = new TH1D("h_zda","z_had", 100,0.7, 1.1); 
  }
  else if (ptgrp == 3 || ptgrp == 4){
   TH1D *h_zd = new TH1D("h_zd","z_had", 100,0.45, 0.85); 
   TH1D *h_zs = new TH1D("h_zs","z_had", 100,0.45, 0.85); 
   TH1D *h_za = new TH1D("h_za","z_had", 100,0.45, 0.85); 
   TH1D *h_zds = new TH1D("h_zds","z_had", 100,0.45, 0.85); 
   TH1D *h_zda = new TH1D("h_zda","z_had", 100,0.45, 0.85); 
  }
  else if (ptgrp > 4 ){
   TH1D *h_zd = new TH1D("h_zd","z_had", 100,0.3, 0.7);  
   TH1D *h_zs = new TH1D("h_zs","z_had", 100,0.3, 0.7);  
   TH1D *h_za = new TH1D("h_za","z_had", 100,0.3, 0.7);  
   TH1D *h_zds = new TH1D("h_zds","z_had", 100,0.3, 0.7);  
   TH1D *h_zda = new TH1D("h_zda","z_had", 100,0.3, 0.7);  
  }
  TH1D *h_ptd = new TH1D("h_ptd","pt (GeV)", 75,-0.25, 1.00);   
  TH1D *h_pts = new TH1D("h_pts","pt (GeV)", 75,-0.25, 1.00);
  TH1D *h_pta = new TH1D("h_pta","pt (GeV)", 75,-0.25, 1.00);
  TH1D *h_ptds = new TH1D("h_ptds","pt (GeV)", 75,-0.25, 1.00);
  TH1D *h_ptda = new TH1D("h_ptda","pt (GeV)", 75,-0.25, 1.00);

  TH1D *h_MissMass2d = new TH1D("h_MissMass2d", "MissMass2 (GeV2)", 150, 1.5, 8.5);
  TH1D *h_MissMass2s = new TH1D("h_MissMass2s", "MissMass2 (GeV2)", 150, 1.5, 8.5); 
  TH1D *h_MissMass2a = new TH1D("h_MissMass2a", "MissMass2 (GeV2)", 150, 1.5, 8.5); 
  TH1D *h_MissMass2ds = new TH1D("h_MissMass2ds", "MissMass2 (GeV2)", 150, 1.5, 8.5); 
  TH1D *h_MissMass2da = new TH1D("h_MissMass2da", "MissMass2 (GeV2)", 150, 1.5, 8.5); 

  TH1D *h_phipqd = new TH1D ("h_phipqd", "Phi_hadron (Rad)", 100, -4, 7);
  TH1D *h_phipqs = new TH1D ("h_phipqs", "Phi_hadron (Rad)", 100, -4, 7);

  TH1D *s_hdelta = new TH1D("s_hdelta","HMS DELTA (%)",100,-15,15); 
  TH1D *s_hxptar = new TH1D("s_hxptar","HMS XPTAR (rad)",100,-0.15,0.15);
  TH1D *s_hyptar = new TH1D("s_hyptar","HMS YPTAR (rad)",100,-0.1,0.1);
  TH1D *s_hytar = new TH1D("s_hytar","HMS YTAR (cm)",100,-5.0,5.0);
  TH1D *s_pdelta = new TH1D("s_pdelta","SHMS DELTA (%)",100,-25,30);   
  TH1D *s_pxptar = new TH1D("s_sxptar","SHMS XPTAR (rad)",100,-0.1,0.1);
  TH1D *s_pyptar = new TH1D("s_syptar","SHMS YPTAR (rad)",100,-0.08,0.08);
  TH1D *s_pytar = new TH1D("s_sytar","SHMS YTAR (cm)",100,-5,5);  

  TH1D *h_hdelta = new TH1D("h_hdelta","HMS DELTA (%)",100,-15,15);      
  TH1D *h_hxptar = new TH1D("h_hxptar","HMS XPTAR (rad)",100,-0.15,0.15);
  TH1D *h_hyptar = new TH1D("h_hyptar","HMS YPTAR (rad)",100,-0.1,0.1);
  TH1D *h_hytar = new TH1D("h_hytar","HMS YTAR (cm)",100,-5.0,5.0);    

  TH1D *h_pdelta = new TH1D("h_pdelta","SHMS DELTA (%)",100,-25,30);          
  TH1D *h_pxptar = new TH1D("h_sxptar","SHMS XPTAR (rad)",100,-0.1,0.1);
  TH1D *h_pyptar = new TH1D("h_syptar","SHMS YPTAR (rad)",100,-0.08,0.08);
  TH1D *h_pytar = new TH1D("h_pytar","SHMS YTAR (cm)",100,-5,5); 

  TH1D *h_hdeltaa = new TH1D("h_hdeltaa","HMS DELTA (%)",100,-15,15);      
  TH1D *h_hxptara = new TH1D("h_hxptara","HMS XPTAR (rad)",100,-0.15,0.15);
  TH1D *h_hyptara = new TH1D("h_hyptara","HMS YPTAR (rad)",100,-0.1,0.1);
  TH1D *h_hytara = new TH1D("h_hytara","HMS YTAR (cm)",100,-5.0,5.0);    

  TH1D *h_pdeltaa = new TH1D("h_pdeltaa","SHMS DELTA (%)",100,-25,30); 
  TH1D *h_pxptara = new TH1D("h_sxptara","SHMS XPTAR (rad)",100,-0.1,0.1);
  TH1D *h_pyptara = new TH1D("h_syptara","SHMS YPTAR (rad)",100,-0.08,0.08);
  TH1D *h_pytara = new TH1D("h_pytara","SHMS YTAR (cm)",100,-5,5); 

  TH1D *h_hdeltads = new TH1D("h_hdeltads","HMS DELTA (%)",100,-15,15);      
  TH1D *h_hxptards = new TH1D("h_hxptards","HMS XPTAR (rad)",100,-0.15,0.15);
  TH1D *h_hyptards = new TH1D("h_hyptards","HMS YPTAR (rad)",100,-0.1,0.1);
  TH1D *h_hytards = new TH1D("h_hytards","HMS YTAR (cm)",100,-5.0,5.0);    

  TH1D *h_pdeltads = new TH1D("h_pdeltads","SHMS DELTA (%)",100,-25,30); 
  TH1D *h_pxptards = new TH1D("h_sxptards","SHMS XPTAR (rad)",100,-0.1,0.1);
  TH1D *h_pyptards = new TH1D("h_syptards","SHMS YPTAR (rad)",100,-0.08,0.08);
  TH1D *h_pytards = new TH1D("h_pytards","SHMS YTAR (cm)",100,-5,5); 

  TH1D *h_hdeltada = new TH1D("h_hdeltada","HMS DELTA (%)",100,-15,15);      
  TH1D *h_hxptarda = new TH1D("h_hxptarda","HMS XPTAR (rad)",100,-0.15,0.15);
  TH1D *h_hyptarda = new TH1D("h_hyptarda","HMS YPTAR (rad)",100,-0.1,0.1);
  TH1D *h_hytarda = new TH1D("h_hytarda","HMS YTAR (cm)",100,-5.0,5.0);    

  TH1D *h_pdeltada = new TH1D("h_pdeltada","SHMS DELTA (%)",100,-25,30); 
  TH1D *h_pxptarda = new TH1D("h_sxptarda","SHMS XPTAR (rad)",100,-0.1,0.1);
  TH1D *h_pyptarda = new TH1D("h_syptarda","SHMS YPTAR (rad)",100,-0.08,0.08);
  TH1D *h_pytarda = new TH1D("h_pytarda","SHMS YTAR (cm)",100,-5,5); 


  TChain *tt = new TChain("T");
  
  for (int j=0;j<numruns;j++){

    cout << "opening run number: " <<  runnumber <<"\n";
    //read the input file from data
    TString fileNameD = ROOT_FILE_PATH;
    fileNameD += "coin_replay_production_"; //read the root file from data
    fileNameD += runnumber; //read the root file from data
    fileNameD += "_-1.root"; //read the root file from data

    tt->Add(fileNameD);
    runnumber++;
  }
  //  TFile *f1 = new TFile(fileNameD);
  //  TTree *tt = (TTree*)f1->Get("T");
  //get the relevant branch
  int nentriesD = tt->GetEntries();
  cout<<"Entries:\t"<<nentriesD<<endl;

  Double_t HgtrX, HgtrTh, HgtrY, HgtrPh, hdelta, PgtrX, PgtrTh, PgtrY, PgtrPh, pdelta;// Ep, Epp, p_e, p_p, defEm;
  Double_t HgtrBetaCalc, PgtrBetaCalc, evtType, PgtrP, HgtrP, PhodStatus, PhodStartTime, PhodfpHitsTime;
  Double_t cointime, HhodStatus, HhodStartTime, HhodfpHitsTime, SHMSpartMass, HMSpartMass;
  Double_t pkinomega, pkinQ2, pkinxbj, pEm, pPm, pbeta, hbeta, hcalepr, hcaletot, hcernpe, pcaletot, pcalepr, pcernpe, paeronpe;
  Double_t pkinPmiss, pkinq, pkinthetaq, pkinphx_cm, pkinph_xq; //Added
  Double_t TcoinpTRIG1_ROC1_tdcTimeRaw, TcoinpTRIG4_ROC1_tdcTimeRaw, TcoinpTRIG1_ROC2_tdcTimeRaw;
  Double_t TcoinhTRIG1_ROC1_tdcTimeRaw, TcoinhTRIG1_ROC2_tdcTimeRaw, TcoinhTRIG4_ROC1_tdcTimeRaw;
  Double_t TcoinhTRIG4_ROC2_tdcTimeRaw, TcoinpTRIG4_ROC2_tdcTimeRaw;
  Double_t Preactz,Hreactz;


  // Foal plane coords
  Double_t PdcXfp;
  Double_t PdcXpfp;
  Double_t PdcYfp;
  Double_t PdcYpfp;
  Double_t HdcXfp;
  Double_t HdcXpfp;
  Double_t HdcYfp;
  Double_t HdcYpfp;


  Int_t cntsepi=0;


  tt->SetBranchAddress("P.gtr.p", &PgtrP); 
  tt->SetBranchAddress("H.gtr.p", &HgtrP); 
  tt->SetBranchAddress("P.gtr.beta", &pbeta);                                                                                 
  tt->SetBranchAddress("H.gtr.beta", &hbeta); 
  tt->SetBranchAddress("H.gtr.dp", &hdelta);                                                                                
  tt->SetBranchAddress("P.gtr.dp", &pdelta);                                                                                
                                                                                                                            
  tt->SetBranchAddress("P.cal.eprtracknorm", &pcalepr);                                                                    
  tt->SetBranchAddress("P.cal.etottracknorm", &pcaletot);                                                                  
  tt->SetBranchAddress("P.hgcer.npeSum", &pcernpe);
  tt->SetBranchAddress("P.aero.npeSum", &paeronpe);                                                 
                                                                                                             
  tt->SetBranchAddress("H.cal.eprtracknorm", &hcalepr);                                  
  tt->SetBranchAddress("H.cal.etottracknorm", &hcaletot);                                          
  tt->SetBranchAddress("H.cer.npeSum", &hcernpe); 


  tt->SetBranchAddress("P.hod.starttime", &PhodStartTime);                                               
  tt->SetBranchAddress("P.hod.fpHitsTime", &PhodfpHitsTime);                                             
  tt->SetBranchAddress("H.hod.starttime", &HhodStartTime);                                               
  tt->SetBranchAddress("H.hod.fpHitsTime", &HhodfpHitsTime); 
  tt->SetBranchAddress("T.coin.pTRIG1_ROC1_tdcTimeRaw", &TcoinpTRIG1_ROC1_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG4_ROC1_tdcTimeRaw", &TcoinpTRIG4_ROC1_tdcTimeRaw);
  tt->SetBranchAddress("T.coin.pTRIG1_ROC2_tdcTimeRaw", &TcoinpTRIG1_ROC2_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG4_ROC2_tdcTimeRaw", &TcoinpTRIG4_ROC2_tdcTimeRaw);                   

  TCut hpdelta;
  TCut epiCut;                                                                   



  hpdelta = "P.gtr.dp > -10 && P.gtr.dp < 20 && H.gtr.dp > -10 && H.gtr.dp < 10";
  epiCut = "P.aero.npeSum > 1.0 && P.cal.eprtracknorm < 0.2 && H.cer.npeSum > 1.0 && H.cal.etottracknorm > 0.6 && H.cal.etottracknorm < 2.0 && H.cal.eprtracknorm  > 0.2"; 

  

  TCanvas *canvas1 = new TCanvas("canvas1","canvas1");                           
  tt->Draw("P.hod.starttime >> SHMShodoStartTime", epiCut && hpdelta );  
  TH1D *h1PhodoStartTime = (TH1D*)gDirectory->Get("SHMShodoStartTime");
  h1PhodoStartTime->GetXaxis()->SetTitle("SHMS hodo start time [ns]");           
  Double_t PhodoStartTimeMean = h1PhodoStartTime->GetMean();  



                   
  TCanvas *canvas2 = new TCanvas("canvas2","canvas2");                           
  tt->Draw("H.hod.starttime >> HMShodoStartTime", epiCut && hpdelta );  
  TH1D *h1HhodoStartTime = (TH1D*)gDirectory->Get("HMShodoStartTime");           
  h1HhodoStartTime->GetXaxis()->SetTitle("HMS hodo start time [ns]");            
  Double_t HhodoStartTimeMean = h1HhodoStartTime->GetMean();                     



  tt->SetBranchAddress("H.kin.primary.W", &pkinW);
  tt->SetBranchAddress("H.kin.primary.omega", &pkinomega);
  tt->SetBranchAddress("H.kin.primary.Q2", &pkinQ2);
  tt->SetBranchAddress("H.kin.primary.x_bj", &pkinxbj); 

  tt->SetBranchAddress("P.kin.secondary.th_xq", &pkinthetaq);        
      
  tt->SetBranchAddress("H.kin.primary.q3m", &pkinq);              

  tt->SetBranchAddress("P.kin.secondary.ph_xq",&pkinph_xq);  //"Azimuth of Detected particle with scattering plane (rad)"
  if (targ == 1) tt->SetBranchAddress("P.kin.secondary.emiss", &pEm);   //missing energy     
  tt->SetBranchAddress("P.kin.secondary.pmiss", &pPm);    //missing momentum
  tt->SetBranchAddress("H.gtr.x", &HgtrX);                                                               
  tt->SetBranchAddress("H.gtr.th", &HgtrTh);                                                             
  tt->SetBranchAddress("H.gtr.y", &HgtrY);                                                               
  tt->SetBranchAddress("H.gtr.ph", &HgtrPh);                                                             

  tt->SetBranchAddress("P.gtr.x", &PgtrX);                                                               
  tt->SetBranchAddress("P.gtr.th", &PgtrTh);                                                             
  tt->SetBranchAddress("P.gtr.y", &PgtrY);                                                               
  tt->SetBranchAddress("P.gtr.ph", &PgtrPh);                                                             

    
  tt->SetBranchAddress("H.dc.x_fp", &HdcXfp);
  tt->SetBranchAddress("H.dc.xp_fp", &HdcXpfp);
  tt->SetBranchAddress("H.dc.y_fp", &HdcYfp);
  tt->SetBranchAddress("H.dc.yp_fp", &HdcYpfp);    

  tt->SetBranchAddress("P.dc.x_fp", &PdcXfp);
  tt->SetBranchAddress("P.dc.xp_fp", &PdcXpfp);
  tt->SetBranchAddress("P.dc.y_fp", &PdcYfp);
  tt->SetBranchAddress("P.dc.yp_fp", &PdcYpfp);    


  Double_t pOffset = 1.5; //9.5 + 10;  // in ns                                  
  Double_t hOffset = 335;                                                        
  Double_t speedOfLight = 29.9792; // in cm/ns                                   
  Double_t SHMScentralPathLen = 18.1*100;  // SHMS Target to focal plane path length converted to cm  
  Double_t SHMSpathLength = 18.1*100;      // For now assume that it's same as SHMScentralPathLen  
  Double_t HMScentralPathLen = 22*100;     // HMS Target to focal plane path length converted to cm
  Double_t DeltaHMSpathLength;             // For now assume that it's same as HMScentralPathLen 
  Double_t SHMScoinCorr = 0.0;                                                   
  Double_t HMScoinCorr = 0.0;                                                    
  Double_t SHMSrawCoinTimeROC1;                                                  
  Double_t SHMSrawCoinTimeROC2;                                                  
  Double_t HMSrawCoinTimeROC1;                                                   
  Double_t HMSrawCoinTimeROC2;                                                   
                                                                                   
  Double_t SHMScorrCoinTimeROC1;                                                 
  Double_t SHMScorrCoinTimeROC2;                                                 
  Double_t HMScorrCoinTimeROC1;                                                  
  Double_t HMScorrCoinTimeROC2;   
                                                                                                   
   
      SHMSpartMass = 0.1395701835; // pion mass in GeV/c^2                 
      HMSpartMass = 0.000510998; // electron mass in GeV/c^2  
    

      Bool_t epievent_cut, event_cut, hpdelta_cut, ctime_cut, acc_cut, accL_cut, accR_cut; 
  //

  for (int kk=0; kk<nentriesD;  kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
    evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    hpdelta_cut = hdelta > -10 && hdelta < 10 && pdelta > -10 && pdelta < 20 ;
    epievent_cut = paeronpe > 0. && pcalepr < 0.2 && pcaletot < 0.5 && hcaletot > 0.6 && hcaletot < 2.0 && hpdelta_cut  && hcernpe > 0.;
    event_cut = epievent_cut;


    if (epievent_cut) {
     SHMSpartMass = 0.1395704; // pion mass in GeV/c^2 

	//  ---------------cointime calculation ------------------
	DeltaHMSpathLength = 12.462*HgtrTh + 0.1138*HgtrTh*HgtrX - 0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 0.0000544*HgtrX*HgtrX - 116.52*HgtrPh*HgtrPh;               
	PgtrBetaCalc = PgtrP/sqrt(PgtrP*PgtrP + SHMSpartMass*SHMSpartMass);        
	HgtrBetaCalc = HgtrP/sqrt(HgtrP*HgtrP + HMSpartMass*HMSpartMass);          

	SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*PgtrBetaCalc) + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
	HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / (speedOfLight*HgtrBetaCalc) + (HhodoStartTimeMean - HhodfpHitsTime);      

	SHMScorrCoinTimeROC1 = (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; // 0.1 to convert to ns 

	SHMScorrCoinTimeROC2 = (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; 
	h1_epi_PcointimeROC2->Fill(SHMScorrCoinTimeROC2);    
            
	//-------------------------------------------------------------------------
      }
  }
  Int_t goodcoins=0;
  Int_t accidentals=0;
  Int_t bin_max = h1_epi_PcointimeROC2->GetMaximumBin();
  Double_t max_value = h1_epi_PcointimeROC2->GetBinCenter(bin_max);

  Int_t sh1=max_value + 3*4 -1.5;
  Int_t sh2=max_value + 4*4 +2.;
  Int_t sh3=max_value - 1*4 +1.5;
  Int_t sh4=max_value - 3*4 -1.5;
  Double_t acc_scale_fac = -3.0/((sh2-sh1)+(sh3-sh4));

  for (int kk=0; kk<nentriesD;  kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
    evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    hpdelta_cut = hdelta > -10 && hdelta < 10 && pdelta > -10 && pdelta < 20 ;
    epievent_cut = paeronpe > 0. && pcalepr < 0.2 && pcaletot < 0.5 && hcaletot > 0.6 && hcaletot < 2.0 && hpdelta_cut  && hcernpe > 0.;
    event_cut = epievent_cut;


    if (epievent_cut) {
     SHMSpartMass = 0.1395704; // pion mass in GeV/c^2 
        cntsepi++;

	//  ---------------cointime calculation ------------------
	DeltaHMSpathLength = 12.462*HgtrTh + 0.1138*HgtrTh*HgtrX - 0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 0.0000544*HgtrX*HgtrX - 116.52*HgtrPh*HgtrPh;               
	PgtrBetaCalc = PgtrP/sqrt(PgtrP*PgtrP + SHMSpartMass*SHMSpartMass);        
	HgtrBetaCalc = HgtrP/sqrt(HgtrP*HgtrP + HMSpartMass*HMSpartMass);          

	SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*PgtrBetaCalc) + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
	HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / (speedOfLight*HgtrBetaCalc) + (HhodoStartTimeMean - HhodfpHitsTime);      

	SHMScorrCoinTimeROC1 = (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; // 0.1 to convert to ns 

	SHMScorrCoinTimeROC2 = (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; 

        ctime_cut = SHMScorrCoinTimeROC2 > max_value - 1.5 && SHMScorrCoinTimeROC2 < max_value + 1.5;
        accR_cut = SHMScorrCoinTimeROC2 > sh1 && SHMScorrCoinTimeROC2 < sh2 ;
        accL_cut = SHMScorrCoinTimeROC2 > sh4 && SHMScorrCoinTimeROC2 < sh3 ;
        acc_cut = accR_cut || accL_cut ; 

        if (ctime_cut) {
	  h_Q2d->Fill(pkinQ2);
	  pKinW_2= pkinW*pkinW;
	  h_W2d->Fill(pKinW_2);
	  h_xd->Fill(pkinxbj);
	  E_pi = sqrt(pow(pionMass,2) + pow(PgtrP,2));
	  z_had = E_pi/pkinomega;
	  h_zd->Fill(z_had);
	  ptd = (PgtrP)*sin(pkinthetaq);
          h_ptd->Fill(ptd);  
          if (targ == 2) pEm = pkinomega + 0.9389187 - E_pi ;
          MissMass2d = pEm*pEm - pPm*pPm;
	  h_MissMass2d->Fill(MissMass2d); 
	  h_hdelta->Fill(hdelta);  
          h_hxptar->Fill(HgtrTh);
          h_hyptar->Fill(HgtrPh);
	  h_hytar->Fill(HgtrY); 
	  h_pdelta->Fill(pdelta); 
          h_pxptar->Fill(PgtrTh);
          h_pyptar->Fill(PgtrPh);
	  h_pytar->Fill(PgtrY); 
	  h1_epi_PcointimeROC2_C->Fill(SHMScorrCoinTimeROC2); 
          goodcoins++;   
	}
        else if (acc_cut){
	  h_Q2a->Fill(pkinQ2);
	  pKinW_2= pkinW*pkinW;
	  h_W2a->Fill(pKinW_2);
	  h_xa->Fill(pkinxbj);
	  E_pi = sqrt(pow(pionMass,2) + pow(PgtrP,2));
	  z_had = E_pi/pkinomega;
	  h_za->Fill(z_had);
	  ptd = (PgtrP)*sin(pkinthetaq);
          h_pta->Fill(ptd);  
          if (targ == 2) pEm = pkinomega + 0.9389187 - E_pi ;
          MissMass2d = pEm*pEm - pPm*pPm;
	  h_MissMass2a->Fill(MissMass2d); 
	  h_hdeltaa->Fill(hdelta);  
          h_hxptara->Fill(HgtrTh);
          h_hyptara->Fill(HgtrPh);
	  h_hytara->Fill(HgtrY);
	  h_pdeltaa->Fill(pdelta); 
          h_pxptara->Fill(PgtrTh);
          h_pyptara->Fill(PgtrPh);
	  h_pytara->Fill(PgtrY); 
	  h1_epi_PcointimeROC2_B->Fill(SHMScorrCoinTimeROC2); 
          accidentals++;   
        }
    }
  }  
  cout << "good coincidences =  " << goodcoins << "   and accidentals =  " << accidentals << endl; 
  cout << "acc scale fac = " << acc_scale_fac << endl;
  TH1D *h_Q2c = new TH1D(*h_Q2d); // for clean events
  //  if (!(h_Q2c->GetSumw2N() > 0))   h_Q2c->Sumw2(kTRUE);
  h_Q2c->Add(h_Q2a,acc_scale_fac);

  TH1D *h_W2c = new TH1D(*h_W2d);
  //  if (!(h_W2c->GetSumw2N() > 0))   h_W2c->Sumw2(kTRUE);
  h_W2c->Add(h_W2a,acc_scale_fac);

  TH1D *h_xc = new TH1D(*h_xd);                                       
  //  if (!(h_xc->GetSumw2N() > 0))   h_xc->Sumw2(kTRUE);
  h_xc->Add(h_xa,acc_scale_fac);

  TH1D *h_zc = new TH1D(*h_zd);    
  //  if (!(h_zc->GetSumw2N() > 0))   h_zc->Sumw2(kTRUE);
  h_zc->Add(h_za,acc_scale_fac);

  TH1D *h_ptc = new TH1D(*h_ptd); 
  //  if (!(h_ptc->GetSumw2N() > 0))   h_ptc->Sumw2(kTRUE);
  h_ptc->Add(h_pta,acc_scale_fac);

  TH1D *h_MissMass2c = new TH1D(*h_MissMass2d);
  //  if (!(h_MissMass2c->GetSumw2N() > 0))   h_MissMass2c->Sumw2(kTRUE);
  h_MissMass2c->Add(h_MissMass2a,acc_scale_fac);
  
  TH1D *h_hdeltac = new TH1D(*h_hdelta); // for clean events
  //  if (!(h_hdeltac->GetSumw2N() > 0)) h_hdeltac->Sumw2(kTRUE);
  h_hdeltac->Add(h_hdeltaa,acc_scale_fac);

  TH1D *h_hxptarc = new TH1D(*h_hxptar); // for clean events
  //  if (!(h_hxptarc->GetSumw2N() > 0)) h_hxptarc->Sumw2(kTRUE);
  h_hxptarc->Add(h_hxptara,acc_scale_fac);

  TH1D *h_hyptarc = new TH1D(*h_hyptar); // for clean events
  //  if (!(h_hyptarc->GetSumw2N() > 0)) h_hyptarc->Sumw2(kTRUE);
  h_hyptarc->Add(h_hyptara,acc_scale_fac);

  TH1D *h_hytarc = new TH1D(*h_hytar); // for clean events
  //  if (!(h_hytarc->GetSumw2N() > 0)) h_hytarc->Sumw2(kTRUE);
  h_hytarc->Add(h_hytara,acc_scale_fac);


  TH1D *h_pdeltac = new TH1D(*h_pdelta); // for clean events
  //  if (!(h_pdeltac->GetSumw2N() > 0)) h_pdeltac->Sumw2(kTRUE);
  h_pdeltac->Add(h_pdeltaa,acc_scale_fac);

  TH1D *h_pxptarc = new TH1D(*h_pxptar); // for clean events
  //  if (!(h_pxptarc->GetSumw2N() > 0)) h_pxptarc->Sumw2(kTRUE);
  h_pxptarc->Add(h_pxptara,acc_scale_fac);

  TH1D *h_pyptarc = new TH1D(*h_pyptar); // for clean events
  //  if (!(h_pyptarc->GetSumw2N() > 0)) h_pyptarc->Sumw2(kTRUE);
  h_pyptarc->Add(h_pyptara,acc_scale_fac);
    
  TH1D *h_pytarc = new TH1D(*h_pytar); // for clean events
  //  if (!(h_pytarc->GetSumw2N() > 0)) h_pytarc->Sumw2(kTRUE);
  h_pytarc->Add(h_pytara,acc_scale_fac);


  h_Q2d->GetXaxis()->SetTitle("Q2 (GeV2)");
  h_xd->GetXaxis()->SetTitle("x_bj");
  h_W2d->GetXaxis()->SetTitle("W2 (GeV2)");
  h_zd->GetXaxis()->SetTitle("z_had");
  h_ptd->GetXaxis()->SetTitle("pt(GeV)");  
  h_MissMass2d->GetXaxis()->SetTitle("MissMass2 (GeV2)");  
  h_phipqd->GetXaxis()->SetTitle("Phi_hadron (Rad)");

  cout << cntsepi <<" events passed all cuts" << endl;

  Double_t dsz_had, dsE_pi, dsdiff_p, dsptd, dsMissMass2d, dspKinW_2, dspkinW;
  Bool_t dsepievent_cut, dsevent_cut, dshpdelta_cut, dsctime_cut, dacc_cut, daccL_cut, daccR_cut; 

  /////// opening Rootfiles for dummy runs ///

  TChain *td = new TChain("T");
  
  for (int jd=0;jd<numdummy;jd++){

    cout << "opening dummy run number: " << dummyRun[jd]  <<"\n";
    //read the input file from data
    TString fileNameDS = ROOT_FILE_PATH;
    fileNameDS += "coin_replay_production_"; //read the root file from data
    fileNameDS += dummyRun[jd]; //read the root file from data
    fileNameDS += "_-1.root"; //read the root file from data

    td->Add(fileNameDS);
  }

  int nentriesDS = td->GetEntries();
  cout<<"Entries from dummy runs:\t"<<nentriesDS<<endl;


  //////  for Dummy subtraction  /////

  Double_t dsHgtrX, dsHgtrTh, dsHgtrY, dsHgtrPh, dshdelta, dsPgtrX, dsPgtrTh, dsPgtrY, dsPgtrPh, dspdelta;
  Double_t dsHgtrBetaCalc, dsPgtrBetaCalc, dsevtType;
  Double_t dsPgtrP, dsHgtrP, dsPhodStatus, dsPhodStartTime, dsPhodfpHitsTime;
  Double_t dscointime, dsHhodStatus, dsHhodStartTime, dsHhodfpHitsTime;
  Double_t dspkinomega, dspkinQ2, dspkinxbj, dspEm, dspPm, dspbeta, dshbeta, dshcalepr, dshcaletot, dshcernpe, dspcaletot, dspcalepr, dspcernpe, dspaeronpe;
  Double_t dspkinPmiss, dspkinq, dspkinthetaq, dspkinphx_cm, dspkinph_xq; 
  Double_t dsTcoinpTRIG1_ROC1_tdcTimeRaw, dsTcoinpTRIG4_ROC1_tdcTimeRaw, dsTcoinpTRIG1_ROC2_tdcTimeRaw;
  Double_t dsTcoinhTRIG1_ROC1_tdcTimeRaw, dsTcoinhTRIG1_ROC2_tdcTimeRaw, dsTcoinhTRIG4_ROC1_tdcTimeRaw;
  Double_t dsTcoinhTRIG4_ROC2_tdcTimeRaw, dsTcoinpTRIG4_ROC2_tdcTimeRaw;
  Double_t dsPreactz,Hreactz;
  Double_t dsDeltaHMSpathLength;

  Double_t dsSHMScoinCorr = 0.0;                                                   
  Double_t dsHMScoinCorr = 0.0;                                                    
  Double_t dsSHMSrawCoinTimeROC1;                                                  
  Double_t dsSHMSrawCoinTimeROC2;                                                  
  Double_t dsHMSrawCoinTimeROC1;                                                   
  Double_t dsHMSrawCoinTimeROC2;                                                   
                                                                                   
  Double_t dsSHMScorrCoinTimeROC1;                                                 
  Double_t dsSHMScorrCoinTimeROC2;                                                 
  Double_t dsHMScorrCoinTimeROC1;                                                  
  Double_t dsHMScorrCoinTimeROC2;   


  // Foal plane coords
  Double_t dsPdcXfp;
  Double_t dsPdcXpfp;
  Double_t dsPdcYfp;
  Double_t dsPdcYpfp;
  Double_t dsHdcXfp;
  Double_t dsHdcXpfp;
  Double_t dsHdcYfp;
  Double_t dsHdcYpfp;




  td->SetBranchAddress("P.gtr.p", &dsPgtrP); 
  td->SetBranchAddress("H.gtr.p", &dsHgtrP); 
  td->SetBranchAddress("P.gtr.beta", &dspbeta);                                         
                                        
  td->SetBranchAddress("H.gtr.beta", &dshbeta); 
  td->SetBranchAddress("H.gtr.dp", &dshdelta);                                                       
                         
  td->SetBranchAddress("P.gtr.dp", &dspdelta);                                                      
 
                         
  td->SetBranchAddress("P.cal.eprtracknorm", &dspcalepr);                      
                                              
  td->SetBranchAddress("P.cal.etottracknorm", &dspcaletot);           
                                                       
  td->SetBranchAddress("P.hgcer.npeSum", &dspcernpe);
  td->SetBranchAddress("P.aero.npeSum", &dspaeronpe);                                                 
                                                               
  td->SetBranchAddress("H.cal.eprtracknorm", &dshcalepr);                                  
  td->SetBranchAddress("H.cal.etottracknorm", &dshcaletot);                                          
  td->SetBranchAddress("H.cer.npeSum", &dshcernpe); 


  td->SetBranchAddress("P.hod.starttime", &dsPhodStartTime);                                      
  td->SetBranchAddress("P.hod.fpHitsTime", &dsPhodfpHitsTime);                        
                     
  td->SetBranchAddress("H.hod.starttime", &dsHhodStartTime);                      
                         
  td->SetBranchAddress("H.hod.fpHitsTime", &dsHhodfpHitsTime); 
  td->SetBranchAddress("T.coin.pTRIG1_ROC1_tdcTimeRaw", &dsTcoinpTRIG1_ROC1_tdcTimeRaw);   
                
  td->SetBranchAddress("T.coin.pTRIG4_ROC1_tdcTimeRaw", &dsTcoinpTRIG4_ROC1_tdcTimeRaw);
  td->SetBranchAddress("T.coin.pTRIG1_ROC2_tdcTimeRaw", &dsTcoinpTRIG1_ROC2_tdcTimeRaw);     
              
  td->SetBranchAddress("T.coin.pTRIG4_ROC2_tdcTimeRaw", &dsTcoinpTRIG4_ROC2_tdcTimeRaw);         
          

  TCut dshpdelta;
  TCut dsepiCut;                                                                   



  dshpdelta = "P.gtr.dp > -10 && P.gtr.dp < 20 && H.gtr.dp > -10 && H.gtr.dp < 10";
  dsepiCut = "P.aero.npeSum > 1.0 && P.cal.eprtracknorm < 0.2 && H.cer.npeSum > 1.0 && H.cal.etottracknorm > 0.6 && H.cal.etottracknorm < 2.0 && H.cal.eprtracknorm  > 0.2"; 



  TCanvas *canvas3 = new TCanvas("canvas3","canvas3");                           
  td->Draw("P.hod.starttime >> SHMShodoStartTime", dsepiCut && dshpdelta );  
  TH1D *h1dsPhodoStartTime = (TH1D*)gDirectory->Get("SHMShodoStartTime");
  h1dsPhodoStartTime->GetXaxis()->SetTitle("SHMS hodo start time [ns]");           
  Double_t dsPhodoStartTimeMean = h1dsPhodoStartTime->GetMean();  

                   
  TCanvas *canvas4 = new TCanvas("canvas4","canvas4");                           
  td->Draw("H.hod.starttime >> HMShodoStartTime", dsepiCut && dshpdelta );  
  TH1D *h1dsHhodoStartTime = (TH1D*)gDirectory->Get("HMShodoStartTime");           
  h1HhodoStartTime->GetXaxis()->SetTitle("HMS hodo start time [ns]");            
  Double_t dsHhodoStartTimeMean = h1dsHhodoStartTime->GetMean();                     



  td->SetBranchAddress("H.kin.primary.W", &dspkinW);
  td->SetBranchAddress("H.kin.primary.omega", &dspkinomega);
  td->SetBranchAddress("H.kin.primary.Q2", &dspkinQ2);
  td->SetBranchAddress("H.kin.primary.x_bj", &dspkinxbj); 

  td->SetBranchAddress("P.kin.secondary.th_xq", &dspkinthetaq);        
      
  td->SetBranchAddress("H.kin.primary.q3m", &dspkinq);              

  td->SetBranchAddress("P.kin.secondary.ph_xq",&dspkinph_xq);  //"Azimuth of Detected particle with scattering plane (rad)"
  if (targ == 1) tt->SetBranchAddress("P.kin.secondary.emiss", &dspEm);   //missing energy     
  td->SetBranchAddress("P.kin.secondary.pmiss", &dspPm);    //missing momentum
  td->SetBranchAddress("H.gtr.x", &dsHgtrX); 
                                                              
  td->SetBranchAddress("H.gtr.th", &dsHgtrTh);                                                    
        
  td->SetBranchAddress("H.gtr.y", &dsHgtrY);                                                      
         
  td->SetBranchAddress("H.gtr.ph", &dsHgtrPh);                                                  
           

  td->SetBranchAddress("P.gtr.x", &dsPgtrX);                                                    
           
  td->SetBranchAddress("P.gtr.th", &dsPgtrTh);                                                  
           
  td->SetBranchAddress("P.gtr.y", &dsPgtrY);                                               
                
  td->SetBranchAddress("P.gtr.ph", &dsPgtrPh);                                                  
           

    
  td->SetBranchAddress("H.dc.x_fp", &dsHdcXfp);
  td->SetBranchAddress("H.dc.xp_fp", &dsHdcXpfp);
  td->SetBranchAddress("H.dc.y_fp", &dsHdcYfp);
  td->SetBranchAddress("H.dc.yp_fp", &dsHdcYpfp);    

  td->SetBranchAddress("P.dc.x_fp", &dsPdcXfp);
  td->SetBranchAddress("P.dc.xp_fp", &dsPdcXpfp);
  td->SetBranchAddress("P.dc.y_fp", &dsPdcYfp);
  td->SetBranchAddress("P.dc.yp_fp", &dsPdcYpfp);    


  // Now the calculations for dummy subtraction

  for (int kd=0; kd<nentriesDS;  kd++){
    td->GetEntry(kd);
    if (kd % 50000 == 0) cout << kd*100/nentriesD << "   % of data done" << endl;
    dsevtType = td->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    dshpdelta_cut = dshdelta > -10 && dshdelta < 10 && dspdelta > -10 && dspdelta < 20 ;
    dsepievent_cut = dspaeronpe > 0. && dspcalepr < 0.2 && dspcaletot < 0.5 && dshcaletot > 0.6 && dshcaletot < 2.0 && dshpdelta_cut  && dshcernpe > 0.;
    dsevent_cut = dsepievent_cut;


    if (dsepievent_cut) {
     SHMSpartMass = 0.1395704; // pion mass in GeV/c^2 

	//  ---------------cointime calculation ------------------
	dsDeltaHMSpathLength = 12.462*dsHgtrTh + 0.1138*dsHgtrTh*dsHgtrX - 0.0154*dsHgtrX - 72.292*dsHgtrTh*dsHgtrTh - 0.0000544*dsHgtrX*dsHgtrX - 116.52*dsHgtrPh*dsHgtrPh;               
	dsPgtrBetaCalc = dsPgtrP/sqrt(dsPgtrP*dsPgtrP + SHMSpartMass*SHMSpartMass);        
	dsHgtrBetaCalc = dsHgtrP/sqrt(dsHgtrP*dsHgtrP + HMSpartMass*HMSpartMass);          

	dsSHMScoinCorr = SHMScentralPathLen / (speedOfLight*dsPgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*dsPgtrBetaCalc) + (dsPhodoStartTimeMean - dsPhodfpHitsTime);                                                                   
	dsHMScoinCorr = HMScentralPathLen / (speedOfLight*dsHgtrBetaCalc) + dsDeltaHMSpathLength / (speedOfLight*dsHgtrBetaCalc) + (dsHhodoStartTimeMean - dsHhodfpHitsTime);      


	dsSHMScorrCoinTimeROC1 = (dsTcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - dsSHMScoinCorr) - (dsTcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - dsHMScoinCorr) - pOffset; // 0.1 to convert to ns 

	dsSHMScorrCoinTimeROC2 = (dsTcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - dsSHMScoinCorr) - (dsTcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - dsHMScoinCorr) - pOffset; 
	h1_depi_PcointimeROC2->Fill(dsSHMScorrCoinTimeROC2);    
            
	//-------------------------------------------------------------------------
      }
  }
  Int_t dummies=0;
  Int_t adummies=0;
  Int_t dbin_max = h1_depi_PcointimeROC2->GetMaximumBin();
  Double_t dmax_value = h1_depi_PcointimeROC2->GetBinCenter(dbin_max);
  Int_t dsh1=dmax_value + 3*4 -1.5;
  Int_t dsh2=dmax_value + 4*4 +2.;
  Int_t dsh3=dmax_value - 1*4 +1.5;
  Int_t dsh4=dmax_value - 3*4 -1.5;
  Double_t dacc_scale_fac = -3.0/((dsh2-dsh1)+(dsh3-dsh4));


  dsSHMScoinCorr = 0.0;                                                   
  dsHMScoinCorr = 0.0;                                                    

  for (int kc=0; kc<nentriesDS;  kc++){
    td->GetEntry(kc);
    if (kc % 50000 == 0) cout << kc*100/nentriesDS << "   % of data done" << endl;
    dsevtType = td->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    dshpdelta_cut = dshdelta > -10 && dshdelta < 10 && dspdelta > -10 && dspdelta < 20 ;
    dsepievent_cut = dspaeronpe > 0. && dspcalepr < 0.2 && dspcaletot < 0.5 && dshcaletot > 0.6 && dshcaletot < 2.0 && dshpdelta_cut  && dshcernpe > 0.;
    dsevent_cut = dsepievent_cut;


    if (dsepievent_cut) {
     SHMSpartMass = 0.1395704; // pion mass in GeV/c^2 

	//  ---------------cointime calculation ------------------
	dsDeltaHMSpathLength = 12.462*dsHgtrTh + 0.1138*dsHgtrTh*dsHgtrX - 0.0154*dsHgtrX - 72.292*dsHgtrTh*dsHgtrTh - 0.0000544*dsHgtrX*dsHgtrX - 116.52*dsHgtrPh*dsHgtrPh;               
	dsPgtrBetaCalc = dsPgtrP/sqrt(dsPgtrP*dsPgtrP + SHMSpartMass*SHMSpartMass);        
	dsHgtrBetaCalc = dsHgtrP/sqrt(dsHgtrP*dsHgtrP + HMSpartMass*HMSpartMass);          

	dsSHMScoinCorr = SHMScentralPathLen / (speedOfLight*dsPgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*dsPgtrBetaCalc) + (dsPhodoStartTimeMean - dsPhodfpHitsTime);                                                                   
	dsHMScoinCorr = HMScentralPathLen / (speedOfLight*dsHgtrBetaCalc) + dsDeltaHMSpathLength / (speedOfLight*dsHgtrBetaCalc) + (dsHhodoStartTimeMean - dsHhodfpHitsTime);      

	dsSHMScorrCoinTimeROC1 = (dsTcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - dsSHMScoinCorr) - (dsTcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - dsHMScoinCorr) - pOffset; // 0.1 to convert to ns 

	dsSHMScorrCoinTimeROC2 = (dsTcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - dsSHMScoinCorr) - (dsTcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - dsHMScoinCorr) - pOffset; 

        dsctime_cut = dsSHMScorrCoinTimeROC2 > dmax_value - 1.5 && dsSHMScorrCoinTimeROC2 < dmax_value + 1.5;
        daccR_cut = dsSHMScorrCoinTimeROC2 > dsh1 && dsSHMScorrCoinTimeROC2 < dsh2 ;
        daccL_cut = dsSHMScorrCoinTimeROC2 > dsh4 && dsSHMScorrCoinTimeROC2 < dsh3 ;
        dacc_cut = daccR_cut || daccL_cut ; 

        if (dsctime_cut) {
	  h_Q2ds->Fill(dspkinQ2);
	  dspKinW_2= dspkinW*dspkinW;
	  h_W2ds->Fill(dspKinW_2);
	  h_xds->Fill(dspkinxbj);
	  dsE_pi = sqrt(pow(pionMass,2) + pow(dsPgtrP,2));
	  dsz_had = dsE_pi/dspkinomega;
	  h_zds->Fill(dsz_had);
	  dsptd = (dsPgtrP)*sin(dspkinthetaq);
          h_ptds->Fill(dsptd);  
          if (targ == 2) dspEm = dspkinomega + 0.9389187 - dsE_pi ;
          dsMissMass2d = dspEm*dspEm - dspPm*dspPm;
	  h_MissMass2ds->Fill(dsMissMass2d); 
	  h_hdeltads->Fill(dshdelta);  
          h_hxptards->Fill(dsHgtrTh);
          h_hyptards->Fill(dsHgtrPh);
	  h_hytards->Fill(dsHgtrY); 
	  h_pdeltads->Fill(dspdelta); 
          h_pxptards->Fill(dsPgtrTh);
          h_pyptards->Fill(dsPgtrPh);
	  h_pytards->Fill(dsPgtrY); 
	  h1_depi_PcointimeROC2_C->Fill(dsSHMScorrCoinTimeROC2);
          dummies++;    
	}
        else if (dacc_cut){
	  h_Q2da->Fill(dspkinQ2);
	  dspKinW_2= dspkinW*dspkinW;
	  h_W2da->Fill(dspKinW_2);
	  h_xda->Fill(dspkinxbj);
	  dsE_pi = sqrt(pow(pionMass,2) + pow(dsPgtrP,2));
	  dsz_had = dsE_pi/dspkinomega;
	  h_zda->Fill(dsz_had);
	  dsptd = (dsPgtrP)*sin(dspkinthetaq);
          h_ptda->Fill(dsptd);  
          if (targ == 2) dspEm = dspkinomega + 0.9389187 - dsE_pi ;
          dsMissMass2d = dspEm*dspEm - dspPm*dspPm;
	  h_MissMass2da->Fill(dsMissMass2d); 
	  h_hdeltada->Fill(dshdelta);  
          h_hxptarda->Fill(dsHgtrTh);
          h_hyptarda->Fill(dsHgtrPh);
	  h_hytarda->Fill(dsHgtrY);
	  h_pdeltada->Fill(dspdelta); 
          h_pxptarda->Fill(dsPgtrTh);
          h_pyptarda->Fill(PgtrPh);
	  h_pytarda->Fill(dsPgtrY); 
	  h1_depi_PcointimeROC2_B->Fill(dsSHMScorrCoinTimeROC2); 
          adummies++;   
        }
    }
  }  
  cout << " number of good dummy events = " << dummies << "  and number of acc dummies = " << adummies <<endl;
  cout << "dacc scale fac = " << dacc_scale_fac << endl;
  cout << "ratio =  " << d_chr_ratio << endl;

  TH1D *h_Q2dc = new TH1D(*h_Q2c); // for clean events
  h_Q2dc->Add(h_Q2da,dacc_scale_fac);

  TH1D *h_W2dc = new TH1D(*h_W2c);
  h_W2dc->Add(h_W2da,dacc_scale_fac);

  TH1D *h_xdc = new TH1D(*h_xc);                                       
  h_xdc->Add(h_xda,dacc_scale_fac);

  TH1D *h_zdc = new TH1D(*h_zc);    
  h_zdc->Add(h_zda,dacc_scale_fac);

  TH1D *h_ptdc = new TH1D(*h_ptc); 
  h_ptdc->Add(h_ptda,dacc_scale_fac);

  TH1D *h_MissMass2dc = new TH1D(*h_MissMass2c);
  h_MissMass2dc->Add(h_MissMass2da,dacc_scale_fac);
  
  TH1D *h_hdeltadc = new TH1D(*h_hdeltac); // for clean events
  h_hdeltadc->Add(h_hdeltada,dacc_scale_fac);

  TH1D *h_hxptardc = new TH1D(*h_hxptarc); // for clean events
  h_hxptardc->Add(h_hxptarda,dacc_scale_fac);

  TH1D *h_hyptardc = new TH1D(*h_hyptarc); // for clean events
  h_hyptardc->Add(h_hyptarda,dacc_scale_fac);

  TH1D *h_hytardc = new TH1D(*h_hytarc); // for clean events
  h_hytardc->Add(h_hytarda,dacc_scale_fac);


  TH1D *h_pdeltadc = new TH1D(*h_pdeltac); // for clean events
  h_pdeltadc->Add(h_pdeltada,dacc_scale_fac);

  TH1D *h_pxptardc = new TH1D(*h_pxptarc); // for clean events
  h_pxptardc->Add(h_pxptarda,dacc_scale_fac);

  TH1D *h_pyptardc = new TH1D(*h_pyptarc); // for clean events
  h_pyptardc->Add(h_pyptarda,dacc_scale_fac);
    
  TH1D *h_pytardc = new TH1D(*h_pytarc); // for clean events
  h_pytardc->Add(h_pytarda,dacc_scale_fac);

  ///////final dummy subtracted histograms
  TH1D *h_Q2df = new TH1D(*h_Q2dc); // for dummy subtracted events
    h_Q2df->Add(h_Q2ds,d_chr_ratio);

  TH1D *h_W2df = new TH1D(*h_W2dc);
  //  if (!(h_W2df->GetSumw2N() > 0)) h_W2df->Sumw2(kTRUE);
  h_W2df->Add(h_W2ds,d_chr_ratio);

  TH1D *h_xdf = new TH1D(*h_xdc);                                       
  //  if (!(h_xdf->GetSumw2N() > 0)) h_xdf->Sumw2(kTRUE);
  h_xdf->Add(h_xds,d_chr_ratio);

  TH1D *h_zdf = new TH1D(*h_zdc);    
  //  if (!(h_zdf->GetSumw2N() > 0)) h_zdf->Sumw2(kTRUE);
  h_zdf->Add(h_zds,d_chr_ratio);

  TH1D *h_ptdf = new TH1D(*h_ptdc); 
  // if (!(h_ptdf->GetSumw2N() > 0)) h_ptdf->Sumw2(kTRUE);
  h_ptdf->Add(h_ptds,d_chr_ratio);

  TH1D *h_MissMass2df = new TH1D(*h_MissMass2dc);
  //  if (!(h_MissMass2df->GetSumw2N() > 0)) h_MissMass2df->Sumw2(kTRUE);
  h_MissMass2df->Add(h_MissMass2ds,d_chr_ratio);
  
  TH1D *h_hdeltadf = new TH1D(*h_hdeltadc); // for clean events
  //  if (!(h_hdeltadf->GetSumw2N() > 0)) h_hdeltadf->Sumw2(kTRUE);
  h_hdeltadf->Add(h_hdeltads,d_chr_ratio);

  TH1D *h_hxptardf = new TH1D(*h_hxptardc); // for clean events
  //  if (!(h_hxptardf->GetSumw2N() > 0)) h_hxptardf->Sumw2(kTRUE);
  h_hxptardf->Add(h_hxptards,d_chr_ratio);

  TH1D *h_hyptardf = new TH1D(*h_hyptardc); // for clean events
  //  if (!(h_hyptardf->GetSumw2N() > 0)) h_hyptardf->Sumw2(kTRUE);
  h_hyptardf->Add(h_hyptards,d_chr_ratio);

  TH1D *h_hytardf = new TH1D(*h_hytardc); // for clean events
  //  if (!(h_hytardf->GetSumw2N() > 0)) h_hytardf->Sumw2(kTRUE);
  h_hytardf->Add(h_hytards,d_chr_ratio);


  TH1D *h_pdeltadf = new TH1D(*h_pdeltadc); // for clean events
  //  if (!(h_pdeltadf->GetSumw2N() > 0)) h_pdeltadf->Sumw2(kTRUE);
  h_pdeltadf->Add(h_pdeltads,d_chr_ratio);

  TH1D *h_pxptardf = new TH1D(*h_pxptardc); // for clean events
  //  if (!(h_pxptardf->GetSumw2N() > 0)) h_pxptardf->Sumw2(kTRUE);
  h_pxptardf->Add(h_pxptards,d_chr_ratio);

  TH1D *h_pyptardf = new TH1D(*h_pyptardc); // for clean events
  //  if (!(h_pyptardf->GetSumw2N() > 0)) h_pyptardf->Sumw2(kTRUE);
  h_pyptardf->Add(h_pyptards,d_chr_ratio);
    
  TH1D *h_pytardf = new TH1D(*h_pytardc); // for clean events
  //  if (!(h_pytardf->GetSumw2N() > 0)) h_pytardf->Sumw2(kTRUE);
  h_pytardf->Add(h_pytards,d_chr_ratio);




  /////////////////////////////////////////////////////////
  ///////////SIMULATION STARTS HERE///////////////////////
  ///////////////////////////////////////////////////////

  double spdelta, sQ2, sxbj, szhad, wt, sdelta, sfac, sMissMass2, normwt, sPm, sEm, shxptar,shyptar,spxptar,spyptar, shytar, spytar;
  double sthetapq, spt, sMissMass, sEm, sPm, q_q, n_u ,E_pion,sWpr, p_pi, sphipq, p_0, sW, sW2;  
 

  for (int kk=0; kk<nentriesM; kk++){
    ts->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesM << "   % of simc done" << endl;

    sdelta = ts->GetLeaf("hsdelta")->GetValue();
    spdelta = ts->GetLeaf("ssdelta")->GetValue();
    shxptar = ts->GetLeaf("hsxptar")->GetValue();
    shyptar = ts->GetLeaf("hsyptar")->GetValue();
    spxptar = ts->GetLeaf("ssxptar")->GetValue();
    spyptar = ts->GetLeaf("ssyptar")->GetValue();
    shytar = ts->GetLeaf("hsytar")->GetValue();
    spytar = ts->GetLeaf("ssytar")->GetValue();
    
    sQ2 = ts->GetLeaf("Q2")->GetValue();
    sxbj = ts->GetLeaf("xbj")->GetValue();
    szhad = ts->GetLeaf("z")->GetValue();


     
    if (targ == 2) {
     sMissMass = ts->GetLeaf("missmass")->GetValue(); 
     sMissMass2 = sMissMass*sMissMass;
    }
    else if (targ == 1){
     sPm =ts->GetLeaf("Pm")->GetValue();
     sEm =ts->GetLeaf("Em")->GetValue();
     sMissMass2  = (-sPm*sPm +sEm*sEm);
    }


    sthetapq = ts->GetLeaf("thetapq")->GetValue();
    q_q = ts->GetLeaf("q")->GetValue();  //for q
    n_u = ts->GetLeaf("nu")->GetValue(); //for nu
    p_0 = ts->GetLeaf("ppi")->GetValue();//p_0 = ppi in simc
    p_pi = (1+ spdelta/100)*p_0;         //spdelta 
    E_pion = sqrt(pionMass*pionMass + p_pi*p_pi);
    sphipq = ts->GetLeaf("phipq")->GetValue();
    spt = p_pi*sin(sthetapq);
    sW = ts->GetLeaf("W")->GetValue();
    sW2=sW*sW;
    wt = ts->GetLeaf("Weight")->GetValue();
    normwt = wt;
    if (sdelta < 10 && sdelta > -10 && spdelta > -10 && spdelta < 20 )
      {
	h_Q2s->Fill(sQ2,normwt);
	h_xs->Fill(sxbj,normwt);
	h_W2s->Fill(sW2,normwt);
	h_zs->Fill(szhad,normwt);
	h_pts->Fill(spt,normwt);  
	h_MissMass2s->Fill(sMissMass2,normwt);  
	h_phipqs->Fill(sphipq, normwt);
	s_hdelta->Fill(sdelta,normwt);  
        s_hxptar->Fill(shxptar,normwt);
        s_hyptar->Fill(shyptar,normwt);
        s_hytar->Fill(shytar,normwt);
	s_pdelta->Fill(spdelta,normwt); 
        s_pxptar->Fill(spxptar,normwt);
        s_pyptar->Fill(spyptar,normwt);
        s_pytar->Fill(spytar,normwt);
      }
  }
  h_Q2s->GetXaxis()->SetTitle("Q2 (GeV2)");
  h_xs->GetXaxis()->SetTitle("x_bj");
  h_W2s->GetXaxis()->SetTitle("W2 (GeV2)");
  h_zs->GetXaxis()->SetTitle("z_had");
  h_pts->GetXaxis()->SetTitle("pt(GeV)");  
  h_MissMass2s->GetXaxis()->SetTitle("MissMass2 (GeV2)");
  h_phipqs->GetXaxis()->SetTitle("Phi_hadron (Rad)");

  gROOT->SetBatch(kFALSE);
  gStyle->SetOptStat(3);


  TCanvas *cc1 = new TCanvas ("cc1","cc1",500,500);
  cc1->Divide(1,1);
  cc1->cd(1);
  h1_epi_PcointimeROC2->SetLineColor(kBlack);
  h1_epi_PcointimeROC2->Draw();
  h1_epi_PcointimeROC2_C->SetFillColor(kBlue);
  h1_epi_PcointimeROC2_C->SetMarkerColor(kBlue);
  h1_epi_PcointimeROC2_C->SetLineColor(kBlue);
  h1_epi_PcointimeROC2_C->Draw("SAME");
  h1_epi_PcointimeROC2_B->SetMarkerColor(kRed);
  h1_epi_PcointimeROC2_B->SetLineColor(kRed);
  h1_epi_PcointimeROC2_B->Draw("SAME");

  TCanvas *cd1 = new TCanvas ("cd1","cd1",500,500);
  cd1->Divide(1,1);
  cd1->cd(1);
  h1_depi_PcointimeROC2->SetLineColor(kBlack);
  h1_depi_PcointimeROC2->Draw();
  h1_depi_PcointimeROC2_C->SetFillColor(kBlue);
  h1_depi_PcointimeROC2_C->SetMarkerColor(kBlue);
  h1_depi_PcointimeROC2_C->SetLineColor(kBlue);
  h1_depi_PcointimeROC2_C->Draw("SAME");
  h1_depi_PcointimeROC2_B->SetMarkerColor(kRed);
  h1_depi_PcointimeROC2_B->SetLineColor(kRed);
  h1_depi_PcointimeROC2_B->Draw("SAME");
  
  //TCanvas *cc3 = new TCanvas ("cc3","cc3",800,800);
  //cc3->Divide(4,2);
  //cc3->cd(1);
  //h_hdeltads->SetLineColor(kBlue);            
  //h_hdeltads->DrawNormalized("p e1");
  //cc3->cd(2);  
  //h_hxptards->SetLineColor(kBlue);
  //h_hxptards->DrawNormalized("p e1"); 
  //cc3->cd(3);
  //h_hyptards->SetLineColor(kBlue);
  //h_hyptards->DrawNormalized("p e1");
  //cc3->cd(4);
  //h_hytards->SetLineColor(kBlue);
  //h_hytards->DrawNormalized("p e1");
  //cc3->cd(5); 
  //h_pdeltads->SetLineColor(kBlue);
  //h_pdeltads->DrawNormalized("p e1");
  //cc3->cd(6);
  //h_pxptards->SetLineColor(kBlue);
  //h_pxptards->DrawNormalized("p e1");
  //cc3->cd(7);
  //h_pyptards->SetLineColor(kBlue);
  //h_pyptards->DrawNormalized("p e1");
  //cc3->cd(8);
  //h_pytards->SetLineColor(kBlue);
  //h_pytards->DrawNormalized("p e1");


  //if (numruns > 1 ){
  // cc3->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_%d_SIDIS_spec_compare_dummy.pdf",runnum1,runnum2));
  // }
  //else {
  //cc3->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_SIDIS_spec_compare_dummy.pdf",runnum1));
  //}


  TCanvas *cc2 = new TCanvas ("cc2","cc2",800,800);
  cc2->Divide(4,2);
  cc2->cd(1);
  h_hdeltadf->SetLineColor(kBlue);            
  h_hdeltadf->DrawNormalized("p e1");
  s_hdelta->SetLineColor(kRed);
  s_hdelta->DrawNormalized("same");
  cc2->cd(2);  
  h_hxptardf->SetLineColor(kBlue);
  h_hxptardf->DrawNormalized("p e1"); 
  s_hxptar->SetLineColor(kRed);
  s_hxptar->DrawNormalized("same"); 
  cc2->cd(3);
  h_hyptardf->SetLineColor(kBlue);
  h_hyptardf->DrawNormalized("p e1");
  s_hyptar->SetLineColor(kRed);
  s_hyptar->DrawNormalized("same");  
  cc2->cd(4);
  h_hytardf->SetLineColor(kBlue);
  h_hytardf->DrawNormalized("p e1");
  s_hytar->SetLineColor(kRed); 
  s_hytar->DrawNormalized("same");  
  cc2->cd(5); 
  h_pdeltadf->SetLineColor(kBlue);
  h_pdeltadf->DrawNormalized("p e1");
  s_pdelta->SetLineColor(kRed);
  s_pdelta->DrawNormalized("same"); 
  cc2->cd(6);
  h_pxptardf->SetLineColor(kBlue);
  h_pxptardf->DrawNormalized("p e1");
  s_pxptar->SetLineColor(kRed);
  s_pxptar->DrawNormalized("same");
  cc2->cd(7);
  h_pyptardf->SetLineColor(kBlue);
  h_pyptardf->DrawNormalized("p e1");
  s_pyptar->SetLineColor(kRed);
  s_pyptar->DrawNormalized("same"); 
  cc2->cd(8);
  h_pytardf->SetLineColor(kBlue);
  h_pytardf->DrawNormalized("p e1");
  s_pytar->SetLineColor(kRed);
  s_pytar->DrawNormalized("same");


  if (numruns > 1 ){
   cc2->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_%d_SIDIS_spec_compare_dummy_subtracted.pdf",runnum1,runnum2));
  }
  else {
cc2->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_SIDIS_spec_compare_dummy_subtracted.pdf",runnum1));
  }


  gStyle->SetOptStat(3);

  TCanvas *cc = new TCanvas ("cc","cc",800,800);

  cc->SetGrid();
  cc->SetLeftMargin(0.2);
  cc->SetRightMargin(0.2);
  gStyle->SetOptStat(1101);

  cc->Divide(3,2);

  cc->cd(1);
  h_W2df->SetFillColor(kBlue);
  h_W2df->SetMarkerColor(kBlue);
  h_W2df->SetLineColor(kBlue);
  h_W2df->DrawNormalized("p e1");
  h_W2s->SetMarkerColor(kRed);
  h_W2s->SetLineColor(kRed);
  h_W2s->DrawNormalized("SAME");


  cc->cd(2);
  h_Q2df->SetFillColor(kBlue);
  h_Q2df->SetMarkerColor(kBlue);
  h_Q2df->SetLineColor(kBlue);
  h_Q2df->DrawNormalized("p e1");
  h_Q2s->SetFillStyle(1001);
  h_Q2s->SetLineColor(kRed);
  h_Q2s->SetMarkerColor(kRed);
  h_Q2s->DrawNormalized("same");



  cc->cd(3);
  h_xdf->SetFillColor(kBlue);
  h_xdf->SetMarkerColor(kBlue);
  h_xdf->SetLineColor(kBlue);
  h_xdf->DrawNormalized("p e1");
  h_xs->SetFillStyle(1001);
  h_xs->SetMarkerColor(kRed);
  h_xs->SetLineColor(kRed);
  h_xs->DrawNormalized("same");


  
  cc->cd(4);
  h_zdf->SetFillColor(kBlue);
  h_zdf->SetMarkerColor(kBlue);
  h_zdf->SetLineColor(kBlue);
  h_zdf->DrawNormalized("p e1");
  h_zs->SetFillStyle(1001);
  h_zs->SetMarkerColor(kRed);
  h_zs->SetLineColor(kRed);
  h_zs->DrawNormalized("same");

  
  cc->cd(5);
  h_ptdf->SetFillColor(kBlue);
  h_ptdf->SetMarkerColor(kBlue);
  h_ptdf->SetLineColor(kBlue);
  h_ptdf->DrawNormalized("p e1");
  h_pts->SetFillStyle(1001);
  h_pts->SetMarkerColor(kRed);
  h_pts->SetLineColor(kRed);
  h_pts->DrawNormalized("same");


  cc->cd(6);
  h_MissMass2df->SetFillColor(kBlue);
  h_MissMass2df->SetMarkerColor(kBlue);
  h_MissMass2df->SetLineColor(kBlue);
  h_MissMass2df->DrawNormalized("p e1");
  h_MissMass2s->SetFillStyle(1001);
  h_MissMass2s->SetMarkerColor(kRed);
  h_MissMass2s->SetLineColor(kRed);
  h_MissMass2s->DrawNormalized("same");



  if (numruns > 1) {
   cc->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_%d_SIDIS_kine_compare_dummy_subtracted.pdf",runnum1,runnum2));
  }
  else {
   cc->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_SIDIS_kine_compare_dummy_subtracted.pdf",runnum1));
  }


  //  gSystem->Exit(1) ; 
}
