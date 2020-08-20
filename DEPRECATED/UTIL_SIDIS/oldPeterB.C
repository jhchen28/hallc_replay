// Filename: PeterB.C
// Calculates cointime for 3 masses. Makes skim files with needed variables for good el. in HMS
// Needs runnumber, and if target is "h" or "d"
#include <stdio.h> 

void PeterB(Int_t runNumber, Int_t targ=1){

  // Skim file 
  TString fileNameSkim = "UTIL_SIDIS/Skimfiles/Skim" ;
  //  TString fileNameSkim = "Skimfiles/Skim" ;
  fileNameSkim += runNumber; 
  fileNameSkim += ".txt"; 
  FILE *f2 = fopen(fileNameSkim,"w");


  //read the input file from data
    TString fileNameD = "/net/cdaq/cdaql3data/cdaq/hallc-online/ROOTfiles/" ;
    //  TString fileNameD = "/net/cdaq/cdaql3data/cdaq/sidis-fall-18/ROOTfiles/" ;
  fileNameD += "coin_replay_production_"; //read the root file from data
  fileNameD += runNumber; //read the root file from data
  fileNameD += "_-1.root"; //read the root file from data
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




  Double_t HgtrX, HgtrTh, HgtrY, HgtrPh, hdelta, PgtrX, PgtrTh, PgtrY, PgtrPh, pdelta;
  Double_t HgtrBetaCalc, PgtrBetaCalc, evtType, PgtrP, HgtrP, PhodStatus, PhodStartTime, PhodfpHitsTime;
  Double_t cointime, HhodStatus, HhodStartTime, HhodfpHitsTime, paeronpe;
  Double_t pkinW, pEm, pPm, modPm, pbeta, hbeta, hcalepr, hcaletot, hcernpe, pcaletot, pcalepr, pcernpe, pcernpeng;
  Double_t TcoinpTRIG1_ROC1_tdcTimeRaw, TcoinpTRIG4_ROC1_tdcTimeRaw, TcoinpTRIG1_ROC2_tdcTimeRaw;
  Double_t TcoinhTRIG1_ROC1_tdcTimeRaw, TcoinhTRIG1_ROC2_tdcTimeRaw, TcoinhTRIG4_ROC1_tdcTimeRaw;
  Double_t TcoinhTRIG4_ROC2_tdcTimeRaw, TcoinpTRIG4_ROC2_tdcTimeRaw;
  Double_t  helmpsa,helnega,helposa ;
  Int_t  helmpsi,helnegi,helposi ;
  Int_t cntsepi=0;
  Int_t cntsep=0;
  Int_t cntsek=0;
  Int_t cntpos=0;

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
  tt->SetBranchAddress("P.ngcer.npeSum", &pcernpeng);          tt->SetBranchAddress("P.hgcer.npeSum", &pcernpe);          tt->SetBranchAddress("P.aero.npeSum", &paeronpe); 
  tt->SetBranchAddress("H.cal.eprtracknorm", &hcalepr);
  tt->SetBranchAddress("H.cal.etottracknorm", &hcaletot);  
  tt->SetBranchAddress("H.cer.npeSum", &hcernpe); 
  tt->SetBranchAddress(
   "T.coin.pHEL_MPS_adcMultiplicity", &helmpsa);
  tt->SetBranchAddress(
   "T.coin.pHEL_NEG_adcMultiplicity", &helnega);
  tt->SetBranchAddress(
   "T.coin.pHEL_POS_adcMultiplicity", &helposa);
                                                                                                            
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
  Double_t ctimepi, ctimeK, ctimep ;                          
                                                                                   
  Double_t SHMScorrCoinTimeROC1;                                                 
  Double_t SHMScorrCoinTimeROC2;                                                 
  Double_t HMScorrCoinTimeROC1;                                                  
  Double_t HMScorrCoinTimeROC2;   
     
  Bool_t epievent_cut, epevent_cut, ekevent_cut, positron_cut, event_cut, hpdelta_cut;
  //
  for (int kk=0; kk<nentriesD;  kk++){

    tt->GetEntry(kk);

    if (kk % 10000 == 0) cout << kk*100/nentriesD << "   % of data done" << 
PgtrP << endl;

    evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    // was -10, 10, -15, 20 thru 4178)
    if(hdelta > -15 && hdelta < 15 && 

       pdelta > -25 && pdelta < 40 &&
       hcaletot>0.6 && hcernpe>1) {

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

       if(ctimepi > -20. && ctimepi < 26) { 
	 helmpsi = 0 ;
	 if(helmpsa > 0) helmpsi = 1 ;
	 helnegi = 0 ;
	 if(helnega > 0) helnegi = 1 ;
	 helmpsi = 0 ;
	 if(helposa > 0) helposi = 1 ;
         fprintf(f2,
"%6.2f %5.2f %5.2f %1d %1d %1d %6.2f %7.4f %7.4f %6.2f %7.4f %7.4f %4.1f %4.2f %4.2f %4.1f %4.1f %4.1f %4.2f %4.2f\n",
	   ctimepi,ctimeK-ctimepi,ctimep-ctimepi,
	   helmpsa,helnega,helposa,hdelta,HgtrTh,
           HgtrPh,pdelta,PgtrTh,PgtrPh,hcernpe,
	   hcalepr,hcaletot,pcernpeng,
           paeronpe,pcernpe,pcalepr,pcaletot) ;
       } // cointime check
    } // delta, PID cut
  } // loop over events
  fclose(f2) ;
; 
  gROOT->SetBatch(kFALSE);

  gSystem->Exit(1) ; 
}

