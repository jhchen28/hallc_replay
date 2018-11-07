// Filename: PeterB.C
// Calculates cointime for 3 masses. Makes skim files with needed variables for good el. in HMS
// Needs runnumber, and if target is "h" or "d"
#include <stdio.h>  

void PeterB(Int_t runNumber, Int_t targ=1){

  // Skim file 
  TString fileNameSkim = "UTIL_SIDIS/Skimfiles//Skim" ;
  fileNameSkim += runNumber; 
  fileNameSkim += ".txt"; 
  FILE *f2 = fopen(fileNameSkim,"w");
  FILE *f4 = fopen("test.top","w");


  //read the input file from data
  // TString fileNameD = "/net/cdaq/cdaql3data/cdaq/hallc-online/ROOTfiles/" ;
  // TString fileNameD = "/w/hallc-scifs17exp/c-sidis18/bosted/hallc_replay_sidis18/ROOTfiles/" ;
   TString fileNameD = "/net/cdaq/cdaql3data/cdaq/sidis-fall-18/ROOTfiles/";
 fileNameD += "coin_replay_production_"; //read the root file from data
  fileNameD += runNumber; //read the root file from data
  fileNameD += "_-1.root"; //read the root file from data
  // fileNameD += "_20000.root"; //read the root file from data
  TFile *f1 = new TFile(fileNameD);
  TTree *tt = (TTree*)f1->Get("T");
  //get the relevant branch
  int nentriesD = tt->GetEntries();
  cout<<"Entries:\t"<<nentriesD<<endl;
  TString fileO;
  fileO += "HISTOGRAMS/COIN/ROOT/run_"; //read the root file from data
  fileO += runNumber; //read the root file from data
  fileO += "_hists_coin.root"; //read the root file from data

  //TFile *fout = new TFile(fileO,"RECREATE");

  gROOT->SetBatch(kTRUE);

  
  //make histograms:

  TH1D *h1_epi_PcointimeROC2    = new TH1D("SHMS ROC2 Corrected epi Coin Time","SHMS ROC2 Corrected epi Coin Time; cointime [ns]",       480, -24, 24); 
  TH1D *h1_epi_PcointimeROC2_R  = new TH1D("SHMS ROC2 Corrected epi Coin Time_R","SHMS ROC2 Corrected epi Coin Time_R; cointime [ns]",   480, -24, 24); 
  TH1D *h1_epi_PcointimeROC2_L  = new TH1D("SHMS ROC2 Corrected epi Coin Time_L","SHMS ROC2 Corrected epi Coin Time_L; cointime [ns]",   480, -24, 24); 
  TH1D *h1_epi_PcointimeROC2_C  = new TH1D("SHMS ROC2 Corrected epi Coin Time_C","SHMS ROC2 Corrected epi Coin Time_C; cointime [ns]",   480, -24, 24); 

  TH1D *h1_ep_PcointimeROC2    = new TH1D("SHMS ROC2 Corrected ep Coin Time","SHMS ROC2 Corrected ep Coin Time; cointime [ns]",       480, -24, 24); 
  TH1D *h1_ep_PcointimeROC2_R  = new TH1D("SHMS ROC2 Corrected ep Coin Time_R","SHMS ROC2 Corrected ep Coin Time_R; cointime [ns]",   480, -24, 24); 
  TH1D *h1_ep_PcointimeROC2_L  = new TH1D("SHMS ROC2 Corrected ep Coin Time_L","SHMS ROC2 Corrected ep Coin Time_L; cointime [ns]",   480, -24, 24); 
  TH1D *h1_ep_PcointimeROC2_C  = new TH1D("SHMS ROC2 Corrected ep Coin Time_C","SHMS ROC2 Corrected ep Coin Time_C; cointime [ns]",   480, -24, 24); 

  TH1D *h1_ek_PcointimeROC2    = new TH1D("SHMS ROC2 Corrected ek Coin Time","SHMS ROC2 Corrected ek Coin Time; cointime [ns]",       480, -24, 24); 
  TH1D *h1_ek_PcointimeROC2_R  = new TH1D("SHMS ROC2 Corrected ek Coin Time_R","SHMS ROC2 Corrected ek Coin Time_R; cointime [ns]",   480, -24, 24); 
  TH1D *h1_ek_PcointimeROC2_L  = new TH1D("SHMS ROC2 Corrected ek Coin Time_L","SHMS ROC2 Corrected ek Coin Time_L; cointime [ns]",   480, -24, 24); 
  TH1D *h1_ek_PcointimeROC2_C  = new TH1D("SHMS ROC2 Corrected ek Coin Time_C","SHMS ROC2 Corrected ek Coin Time_C; cointime [ns]",   480, -24, 24); 




  Double_t HgtrX, HgtrTh, HgtrPh, hdelta, PgtrX, PgtrTh, PgtrPh, pdelta;
  Double_t hdcx,hdcxp,hdcy,hdcyp,pdcx,pdcxp,pdcy,pdcyp;
  Double_t HgtrBetaCalc, PgtrBetaCalc, evtType, PgtrP, HgtrP, PhodStatus, PhodStartTime, PhodfpHitsTime;
  Double_t cointime, HhodStatus, HhodStartTime, HhodfpHitsTime, paeronpe;
  Double_t pkinW, pEm, pPm, modPm, pbeta, hbeta, hcalepr, hcaletot, hcernpe, pcaletot, pcalepr, pcernpe, pcernpeng;
  Double_t TcoinpTRIG1_ROC1_tdcTimeRaw, TcoinpTRIG4_ROC1_tdcTimeRaw, TcoinpTRIG1_ROC2_tdcTimeRaw;
  Double_t TcoinhTRIG1_ROC1_tdcTimeRaw, TcoinhTRIG1_ROC2_tdcTimeRaw, TcoinhTRIG4_ROC1_tdcTimeRaw;
  Double_t t11,t31,t41,t51,t61,t12,t32,t42,t52,t62,diff1,diff3,diff4,diff5,diff6 ;
  Double_t TcoinhTRIG4_ROC2_tdcTimeRaw, TcoinpTRIG4_ROC2_tdcTimeRaw;
  Double_t TcoinpTRIG3_ROC1_tdcTimeRaw, TcoinpTRIG3_ROC2_tdcTimeRaw;
  Double_t TcoinpTRIG5_ROC1_tdcTimeRaw, TcoinpTRIG5_ROC2_tdcTimeRaw;
  Double_t TcoinpTRIG6_ROC1_tdcTimeRaw, TcoinpTRIG6_ROC2_tdcTimeRaw,sum1,sum2,counters[10];
  Double_t  helmpsa,helnega,helposa,hgtry,pgtry,pcaltot,pgdsc,trkeff,trkeffer ;
  Int_t  helmpsi,helnegi,helposi,icc,chist[100],chistp[100],chistpt[100] ;
  Int_t  chistb[20][10],ibeta,ctpih[100],ctbig[100] ;
  Int_t cntsepi=0;
  Int_t cntsep=0;
  Int_t cntsek=0;
  Int_t cntpos=0;
  Int_t nevnt=0;
  for(icc=0 ; icc<100 ;  icc++) {
    chist[icc]=0 ;
    chistp[icc]=0 ;
    chistpt[icc]=0 ;
    ctpih[icc]=0 ;
    ctbig[icc]=0 ;
  }
  for(icc=0 ; icc<10 ;  icc++) {
    counters[icc]=0 ;
  }
  for(icc=0 ; icc<20 ;  icc++) {
    for(ibeta=0 ; ibeta<10 ;  ibeta++) {
      chistb[icc][ibeta]=0 ;
    }
  }

  tt->SetBranchAddress("P.dc.x_fp",  &pdcx); 
  tt->SetBranchAddress("P.dc.y_fp",  &pdcy); 
  tt->SetBranchAddress("P.dc.xp_fp", &pdcxp); 
  tt->SetBranchAddress("P.dc.yp_fp", &pdcyp); 
  tt->SetBranchAddress("H.dc.x_fp",  &hdcx); 
  tt->SetBranchAddress("H.dc.y_fp",  &hdcy); 
  tt->SetBranchAddress("H.dc.xp_fp", &hdcxp); 
  tt->SetBranchAddress("H.dc.yp_fp", &hdcyp); 

  tt->SetBranchAddress("P.gtr.y", &pgtry); 
  tt->SetBranchAddress("H.gtr.y", &hgtry); 
  tt->SetBranchAddress("P.gtr.x", &PgtrX); 
  tt->SetBranchAddress("H.gtr.x", &HgtrX); 
  tt->SetBranchAddress("P.gtr.p", &PgtrP); 
  tt->SetBranchAddress("H.gtr.p", &HgtrP); 
  tt->SetBranchAddress("P.gtr.beta", &pbeta);   
  tt->SetBranchAddress("H.gtr.beta", &hbeta); 
  tt->SetBranchAddress("H.gtr.dp", &hdelta);   
  tt->SetBranchAddress("P.gtr.dp", &pdelta);  
  tt->SetBranchAddress("H.gtr.th", &HgtrTh);   
  tt->SetBranchAddress("P.gtr.th", &PgtrTh);  
  tt->SetBranchAddress("H.gtr.ph", &HgtrPh);   
  tt->SetBranchAddress("P.gtr.ph", &PgtrPh);  
  tt->SetBranchAddress("P.cal.eprtracknorm", &pcalepr);
  tt->SetBranchAddress("P.cal.etottracknorm", &pcaletot); 
  tt->SetBranchAddress("P.cal.etotnorm", &pcaltot); 
  tt->SetBranchAddress("P.ngcer.npeSum", &pcernpeng);          
  tt->SetBranchAddress("P.hgcer.npeSum", &pcernpe);          
  tt->SetBranchAddress("P.aero.npeSum", &paeronpe); 
  tt->SetBranchAddress("H.cal.eprtracknorm", &hcalepr);
  tt->SetBranchAddress("H.cal.etottracknorm", &hcaletot);  
  tt->SetBranchAddress("H.cer.npeSum", &hcernpe); 
  tt->SetBranchAddress(
   "T.coin.pHEL_MPS_adcMultiplicity", &helmpsa);
  tt->SetBranchAddress(
   "T.coin.pHEL_NEG_adcMultiplicity", &helnega);
  tt->SetBranchAddress(
   "T.coin.pHEL_POS_adcMultiplicity", &helposa);
  tt->SetBranchAddress("P.hod.goodscinhit", &pgdsc);
                                                                                                            
  tt->SetBranchAddress("P.hod.starttime", &PhodStartTime);                                               
  tt->SetBranchAddress("P.hod.fpHitsTime", &PhodfpHitsTime);                                             
  tt->SetBranchAddress("H.hod.starttime", &HhodStartTime);                                               
  tt->SetBranchAddress("H.hod.fpHitsTime", &HhodfpHitsTime); 
  tt->SetBranchAddress("T.coin.pTRIG1_ROC1_tdcTimeRaw", &TcoinpTRIG1_ROC1_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG3_ROC1_tdcTimeRaw", &TcoinpTRIG3_ROC1_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG4_ROC1_tdcTimeRaw", &TcoinpTRIG4_ROC1_tdcTimeRaw);
  tt->SetBranchAddress("T.coin.pTRIG5_ROC1_tdcTimeRaw", &TcoinpTRIG5_ROC1_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG6_ROC1_tdcTimeRaw", &TcoinpTRIG6_ROC1_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG1_ROC2_tdcTimeRaw", &TcoinpTRIG1_ROC2_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG3_ROC2_tdcTimeRaw", &TcoinpTRIG3_ROC2_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG4_ROC2_tdcTimeRaw", &TcoinpTRIG4_ROC2_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG5_ROC2_tdcTimeRaw", &TcoinpTRIG5_ROC2_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG6_ROC2_tdcTimeRaw", &TcoinpTRIG6_ROC2_tdcTimeRaw);                   
                   
                                              
  TCut hpdelta;
  TCut epiCut;                                                                   



  hpdelta = "P.gtr.dp > -10 && P.gtr.dp < 20 && H.gtr.dp > -10 && H.gtr.dp < 10";
  epiCut = "P.aero.npeSum > 1.0 && P.hgcer.npeSum > 1.0 && P.cal.eprtracknorm < 0.2 && H.cer.npeSum > 1.0 && H.cal.etottracknorm > 0.6 && H.cal.etottracknorm < 2.0 && H.cal.eprtracknorm  > 0.2"; 

  

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

 
  Double_t HMSpartMass = 0.000510998; // electron mass in GeV/c^2
  Double_t SHMSpartMass;                  
  //  Double_t pOffset = 1.5; //9.5 + 10;  // in ns                     
  Double_t pOffset = 15.5   ; //9.5 + 10;  // in ns                                  
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
  Double_t ctimepi, ctimeK, ctimep,ctimeraw,tmp ;                          
                                                                                   
  Double_t SHMScorrCoinTimeROC1;                                                 
  Double_t SHMScorrCoinTimeROC2;                                                 
  Double_t HMScorrCoinTimeROC1;                                                  
  Double_t HMScorrCoinTimeROC2;   
  Double_t  ncoin=0;
  Double_t  ncoinwtrk=0;
     
  Bool_t epievent_cut, epevent_cut, ekevent_cut, positron_cut, event_cut, hpdelta_cut;
  //

  fprintf(f2,"%8.2f %8.2f\n",HhodoStartTimeMean, PhodoStartTimeMean) ;
  printf("%8.2f %8.2f\n",HhodoStartTimeMean, PhodoStartTimeMean) ;
  HhodoStartTimeMean = 30. ;
  PhodoStartTimeMean = 56. ;

  for (int kk=0; kk<nentriesD;  kk++){

    tt->GetEntry(kk);
    t11 = TcoinpTRIG1_ROC1_tdcTimeRaw ;   
    t31 = TcoinpTRIG3_ROC1_tdcTimeRaw ;
    t41 = TcoinpTRIG4_ROC1_tdcTimeRaw ;
    t51 = TcoinpTRIG5_ROC1_tdcTimeRaw ;  
    t61 = TcoinpTRIG6_ROC1_tdcTimeRaw ;
    t12 = TcoinpTRIG1_ROC2_tdcTimeRaw ;
    t32 = TcoinpTRIG3_ROC2_tdcTimeRaw ; 
    t42 = TcoinpTRIG4_ROC2_tdcTimeRaw ;
    t52 = TcoinpTRIG5_ROC2_tdcTimeRaw ;
    t62 = TcoinpTRIG6_ROC2_tdcTimeRaw ;                                           

    evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 

    counters[0] = counters[0]+1 ;


    if(hdelta > -15 && hdelta < 15) {
      counters[1] = counters[1]+1 ;
    }
    if(hdelta > -15 && hdelta < 15 && 

       pdelta > -25 && pdelta < 40) {
       //       hcaletot>0.6 && hcernpe>1) {
    counters[2] = counters[2]+1 ;

       SHMSpartMass = 0.1395704; // pion mass in GeV/c^2 
       DeltaHMSpathLength = 12.462*HgtrTh + 
         0.1138*HgtrTh*HgtrX - 
         0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 
         0.0000544*HgtrX*HgtrX - 
         116.52*HgtrPh*HgtrPh;               
       PgtrBetaCalc = PgtrP/
         sqrt(PgtrP*PgtrP + 
         SHMSpartMass*SHMSpartMass);        
       HgtrBetaCalc = HgtrP/
         sqrt(HgtrP*HgtrP + 
         HMSpartMass*HMSpartMass);          
       SHMScoinCorr = SHMScentralPathLen / 
         (speedOfLight*PgtrBetaCalc) + 
         (SHMSpathLength - SHMScentralPathLen) / 
         (speedOfLight*PgtrBetaCalc) + 
          (PhodoStartTimeMean - PhodfpHitsTime); 
       HMScoinCorr = HMScentralPathLen / 
         (speedOfLight*HgtrBetaCalc) + 
         DeltaHMSpathLength / 
         (speedOfLight*HgtrBetaCalc) + 
         (HhodoStartTimeMean - HhodfpHitsTime);      
       SHMScoinCorr = SHMScentralPathLen / 
         (speedOfLight*PgtrBetaCalc) + 
         (SHMSpathLength - SHMScentralPathLen) / 
         (speedOfLight*PgtrBetaCalc) + 
	 (PhodoStartTimeMean - PhodfpHitsTime); 
       HMScoinCorr = HMScentralPathLen / 
         (speedOfLight*HgtrBetaCalc) + 
         DeltaHMSpathLength / 
         (speedOfLight*HgtrBetaCalc) + 
         (HhodoStartTimeMean - HhodfpHitsTime);      
       SHMScorrCoinTimeROC1 = 
         (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - 
         SHMScoinCorr) - 
         (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - 
         HMScoinCorr) - pOffset; // 0.1 to convert to ns 
       SHMScorrCoinTimeROC2 = 
         (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - 
         SHMScoinCorr) - 
         (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - 
         HMScoinCorr) - pOffset; 
       ctimepi = SHMScorrCoinTimeROC2 ;
       //       printf(" %8.1f %8.1f %8.1f %8.1f %8.1f %8.1f %8.1f\n", ctimepi,TcoinpTRIG1_ROC2_tdcTimeRaw, SHMScoinCorr, TcoinpTRIG4_ROC2_tdcTimeRaw, HMScoinCorr, (PhodoStartTimeMean - PhodfpHitsTime), (HhodoStartTimeMean - HhodfpHitsTime)) ; 

       SHMScoinCorr = (PhodoStartTimeMean - PhodfpHitsTime); 
       HMScoinCorr = (HhodoStartTimeMean - HhodfpHitsTime);      
       ctimeraw = 72. + 
         (TcoinpTRIG1_ROC2_tdcTimeRaw*0.0997 - 
         SHMScoinCorr) - 
         (TcoinpTRIG4_ROC2_tdcTimeRaw*0.0997 - 
         HMScoinCorr) - pOffset; 

       SHMSpartMass = 0.93827231; // proton mass in GeV/c^2
       DeltaHMSpathLength = 12.462*HgtrTh + 
         0.1138*HgtrTh*HgtrX - 
         0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 
         0.0000544*HgtrX*HgtrX - 
         116.52*HgtrPh*HgtrPh;               
       PgtrBetaCalc = PgtrP/
         sqrt(PgtrP*PgtrP + 
         SHMSpartMass*SHMSpartMass);        
       HgtrBetaCalc = HgtrP/
         sqrt(HgtrP*HgtrP + 
         HMSpartMass*HMSpartMass);          
       SHMScoinCorr = SHMScentralPathLen / 
         (speedOfLight*PgtrBetaCalc) + 
         (SHMSpathLength - SHMScentralPathLen) / 
         (speedOfLight*PgtrBetaCalc) + 
         (PhodoStartTimeMean - PhodfpHitsTime); 
       HMScoinCorr = HMScentralPathLen / 
         (speedOfLight*HgtrBetaCalc) + 
         DeltaHMSpathLength / 
         (speedOfLight*HgtrBetaCalc) + 
         (HhodoStartTimeMean - HhodfpHitsTime);      
       SHMScoinCorr = SHMScentralPathLen / 
         (speedOfLight*PgtrBetaCalc) + 
         (SHMSpathLength - SHMScentralPathLen) / 
         (speedOfLight*PgtrBetaCalc) + 
         (PhodoStartTimeMean - PhodfpHitsTime); 
       HMScoinCorr = HMScentralPathLen / 
         (speedOfLight*HgtrBetaCalc) + 
         DeltaHMSpathLength / 
         (speedOfLight*HgtrBetaCalc) + 
         (HhodoStartTimeMean - HhodfpHitsTime);      
       SHMScorrCoinTimeROC1 = 
         (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - 
         SHMScoinCorr) - 
         (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - 
         HMScoinCorr) - pOffset; // 0.1 to convert to ns 
       SHMScorrCoinTimeROC2 = 
         (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - 
         SHMScoinCorr) - 
         (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - 
         HMScoinCorr) - pOffset; 
       ctimep = SHMScorrCoinTimeROC2 ;

       SHMSpartMass = 0.497648;// kaon mass in GeV/c^2
       DeltaHMSpathLength = 12.462*HgtrTh + 
         0.1138*HgtrTh*HgtrX - 
         0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 
         0.0000544*HgtrX*HgtrX - 
         116.52*HgtrPh*HgtrPh;               
       PgtrBetaCalc = PgtrP/
         sqrt(PgtrP*PgtrP + 
         SHMSpartMass*SHMSpartMass);        
       HgtrBetaCalc = HgtrP/
         sqrt(HgtrP*HgtrP + 
         HMSpartMass*HMSpartMass);          
       SHMScoinCorr = SHMScentralPathLen / 
         (speedOfLight*PgtrBetaCalc) + 
         (SHMSpathLength - SHMScentralPathLen) / 
         (speedOfLight*PgtrBetaCalc) + 
         (PhodoStartTimeMean - PhodfpHitsTime); 
       HMScoinCorr = HMScentralPathLen / 
         (speedOfLight*HgtrBetaCalc) + 
         DeltaHMSpathLength / 
         (speedOfLight*HgtrBetaCalc) + 
         (HhodoStartTimeMean - HhodfpHitsTime);      
       SHMScoinCorr = SHMScentralPathLen / 
         (speedOfLight*PgtrBetaCalc) + 
         (SHMSpathLength - SHMScentralPathLen) / 
         (speedOfLight*PgtrBetaCalc) + 
         (PhodoStartTimeMean - PhodfpHitsTime); 
       HMScoinCorr = HMScentralPathLen / 
         (speedOfLight*HgtrBetaCalc) + 
         DeltaHMSpathLength / 
         (speedOfLight*HgtrBetaCalc) + 
         (HhodoStartTimeMean - HhodfpHitsTime);      
       SHMScorrCoinTimeROC1 = 
         (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - 
         SHMScoinCorr) - 
         (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - 
         HMScoinCorr) - pOffset; // 0.1 to convert to ns 
       SHMScorrCoinTimeROC2 = 
         (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - 
         SHMScoinCorr) - 
         (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - 
         HMScoinCorr) - pOffset; 
       ctimeK = SHMScorrCoinTimeROC2 ;

       //       printf("%8.2f %8.2f\n",ctimepi,pbeta) ;
      if(hdelta > -10 && hdelta < 10. && 
          pdelta > -10 && pdelta < 20. && 
	 paeronpe>.4 && pcaltot>0.10 &&
         hcaletot>0.6 && hcernpe>2 &&
         ctimepi > -10. && ctimepi < 10) { 
	 tmp = (ctimepi + 10.)*5. ;
	 icc = (int) tmp;
	 ctpih[icc]++ ;
      }
      if(hdelta > -10 && hdelta < 10. && 
          pdelta > -10 && pdelta < 20. && 
	 paeronpe>.4 && pcaltot>0.10 &&
         hcaletot>0.6 && hcernpe>2 ) { 
	if(ctimepi > -1000 && ctimepi < 1000) tmp = (ctimepi + 1000.) / 20. ;
	if(ctimepi< -1000) tmp = 0. ;
	if(ctimepi>1000) tmp=99. ;
	 icc = (int) tmp;
	 ctbig[icc]++ ;
      }
       if(ctimepi > -2000. && ctimepi < 2600) { 
         counters[3] = counters[3]+1 ;
       }
       if(ctimepi > -20. && ctimepi < 26) { 
         counters[4] = counters[4]+1 ;
	 helmpsi = 0 ;
	 if(helmpsa > 0) helmpsi = 1 ;
	 helnegi = 0 ;
	 if(helnega > 0) helnegi = 1 ;
	 helmpsi = 0 ;
	 if(helposa > 0) helposi = 1 ;
         fprintf(f2,
"%6.2f %5.2f %5.2f %1d %1d %1d %6.2f %7.4f %7.4f %6.2f %7.4f %7.4f %4.1f %4.2f %4.2f %4.1f %4.1f %4.1f %4.2f %4.2f %5.1f %5.1f %6.3f %6.2f %6.3f %6.2f %6.3f %6.2f %6.3f %6.2f %6.3f\n",
	   ctimepi,ctimeK-ctimepi,ctimep-ctimepi,
		 (int)helmpsa,(int)helnega,(int)helposa,hdelta,HgtrTh,
           HgtrPh,pdelta,PgtrTh,PgtrPh,hcernpe,
	   hcalepr,hcaletot,pcernpeng,
		 paeronpe,pcernpe,pcalepr,pcaletot,hgtry,pgtry,pbeta,
                 hdcx,hdcxp,hdcy,hdcyp,pdcx,pdcxp,pdcy,pdcyp) ;
	 if (kk % 100 == 0) cout << kk*100/nentriesD << "   % of data done " << ctimepi << "  " << ctimepi-ctimeraw << "  " << pbeta << " " << hbeta << endl;
	 if(hcernpe>1 && hcaletot>0.7) {
	   counters[5] = counters[5]+1 ;
	   if(paeronpe > 2. && pcaletot > 0.5) {
	     counters[6] = counters[6]+1 ;
	   }
	 }
       } // cointime check
    } // delta, PID cut
    // get SHMS tracking efficiency cut on coin with good e
    // to do add cut on coin time
    // timing with no track info
       //       HMScoinCorr = HMScentralPathLen / 
       //  (speedOfLight*HgtrBetaCalc) + 
       //  DeltaHMSpathLength / 
       //  (speedOfLight*HgtrBetaCalc) + 
       //  (HhodoStartTimeMean - HhodfpHitsTime);      
      //printf("%7.1f %7.1f %7.1f %7.1f %7.1f %7.1f %7.1f %7.1f %7.1f %7.1f   \n",ctimeraw,hdelta,pgdsc,paeronpe,pcaltot,hcaletot,hcernpe) ;
    if(hdelta > -10 && hdelta < 10 && 
       pgdsc>0. && paeronpe>.4 && pcaltot>0.10 &&
       hcaletot>0.6 && hcernpe>2) {
      ncoin = ncoin + 1. ;
      if(pdelta>-100. && pdelta<100.) ncoinwtrk = ncoinwtrk + 1 ;
      //      printf("%10.1f %8.1f %8.2f %5.1f %5.2f \n",
      //        pdelta,ctimeraw,pgdsc,paeronpe,pcaltot);        
      icc = (int) ctimeraw -10 ;
      if(icc > -1 && icc < 100) chist[icc]++ ; 
      if(icc > -1 && icc < 100 && pdelta>-100. && pdelta<100.) chistp[icc]++ ;
      diff1 = t11 - t12 ;
      diff3 = t31 - t32 ;
      diff4 = t41 - t42 ;
      diff5 = t51 - t52 ;
      diff6 = t61 - t62 ;

      if(ctimepi > -4.0 && ctimepi < 0.0 && pbeta>0.95 && pbeta<1.05) {
	tmp = 100.*(pbeta-0.949) ;
	ibeta = (int) tmp ;
        tmp = (ctimepi + 4.) * 5. ;
	icc = (int) tmp ;
	if(icc > -1 && icc<20 && ibeta > -1 && ibeta<10) 
          chistb[icc][ibeta]++ ;
        nevnt++ ; 
        fprintf(f4,"%6d %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
           nevnt, ctimepi, 
		 SHMScoinCorr,HMScoinCorr, -0.1*diff1, hbeta, pbeta,  
           diff5 - diff1, diff6-diff1 ) ;
      } 
      tmp = (ctimepi + 5.) * 10. ;
      icc = (int) tmp ;
      if(icc > -1 && icc < 100 && pdelta>-100. && pdelta<100.) chistpt[icc]++ ;
    }
  } // loop over events

  fprintf(f2,"  0.00  0.00 0.00\n") ;
  fprintf(f2,"%8.0f %8.0f\n",ncoin,ncoinwtrk) ;
  for(icc=0 ; icc<100 ;  icc++) {
    printf("%5d %5d %6d %6d %6d %6d \n",icc,chist[icc],chistp[icc],
	   chistpt[icc],ctpih[icc],ctbig[icc]) ;
    trkeff = 0. ;
    trkeffer = 0. ; 
    if(chist[icc]>2 && chist[icc]>2) {
      trkeff = (float)chistp[icc] / (float)chist[icc] ;
     trkeffer = sqrt(1./chistp[icc] - 1./chist[icc]) ;
    }
    fprintf(f2," %5d %5d %6d %7.3f %7.3f\n",icc,chist[icc],chistp[icc],trkeff,trkeffer) ;
  }
  for(ibeta=0 ; ibeta<10 ;  ibeta++) {
    sum1 = 0. ;
    sum2 = 0. ; 
    for(icc=0 ; icc<20 ;  icc++) {
      fprintf(f4,"%5d %5d\n",icc,chistb[icc][ibeta]) ;
      sum1 = sum1 + chistb[icc][ibeta] ;
      sum2 = sum2 + chistb[icc][ibeta] * icc ;
    }
    fprintf(f4," hist") ;
    printf(" ibeta, petak= ,%5d %7.2f \n",ibeta,sum2/sum1) ;
  }
  for(icc=0 ; icc<10 ; icc++){
    printf(" %7.0f %7.3f\n",counters[icc],counters[icc]/counters[0]) ;
  }
  fclose(f2) ;
  fclose(f4) ;
; 
  gROOT
->SetBatch(kFALSE);

  gSystem->Exit(1) ; 
}

