// Filename: Bean_counter_all.C
// Description: delta, xptar,yptar, ytar distributions for SHMS and HMS and W, Em, Pm and Cointime distributions 
// are plotted and the number of good counts are printed out to add to the count of good events.
// Needs runnumber, and if target is "h" or "d"

void Bean_counter_all(Int_t runNumber, Int_t shms_polarity=1,Int_t last_event=-1){
  //read the input file from data
  TString fileNameD = "/net/cdaq/cdaql3data/cdaq/hallc-online/ROOTfiles/" ;
  fileNameD += "coin_replay_production_"; //read the root file from data
  fileNameD += runNumber; //read the root file from data
  fileNameD += Form("_%d.root",last_event); //read the root file from data
  TFile *f1 = new TFile(fileNameD);
  TTree *tt = (TTree*)f1->Get("T");
  TH2F* dcref_vs_hodoref;
  dcref_vs_hodoref=(TH2F*)f1->Get("dcref_vs_hodoref");
  TH2F* shms_dcref_vs_hodoref;
  shms_dcref_vs_hodoref=(TH2F*)f1->Get("shms_dcref_vs_hodoref");
  TH2F* raster_frx_hms_shms;
  raster_frx_hms_shms=(TH2F*)f1->Get("raster_frx_hms_shms");
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
  TH1D *h1_epi_PcointimeROC2_noPID    = new TH1D("SHMS ROC2 Corrected epi Coin Time NO PID","SHMS ROC2 Corrected epi Coin Time (no PID cut) ; cointime [ns]",       480, -24, 24); 
  TH1D *h1_epi_PcointimeROC2_R  = new TH1D("SHMS ROC2 Corrected epi Coin Time_R","SHMS ROC2 Corrected epi Coin Time_R; cointime [ns]",   480, -24, 24); 
  TH1D *h1_epi_PcointimeROC2_L  = new TH1D("SHMS ROC2 Corrected epi Coin Time_L","SHMS ROC2 Corrected epi Coin Time_L; cointime [ns]",   480, -24, 24); 
  TH1D *h1_epi_PcointimeROC2_C  = new TH1D("SHMS ROC2 Corrected epi Coin Time_C","SHMS ROC2 Corrected epi Coin Time_C; cointime [ns]",   480, -24, 24); 

  TH1D *h1_ep_PcointimeROC2    = new TH1D("SHMS ROC2 Corrected ep Coin Time","SHMS ROC2 Corrected ep Coin Time; cointime [ns]",       480, -24, 24); 
  TH1D *h1_ep_PcointimeROC2_noPID    = new TH1D("SHMS ROC2 Corrected ep Coin Time NO PID","SHMS ROC2 Corrected ep Coin Time (no PID cut) ; cointime [ns]",       480, -24, 24); 
  TH1D *h1_ep_PcointimeROC2_R  = new TH1D("SHMS ROC2 Corrected ep Coin Time_R","SHMS ROC2 Corrected ep Coin Time_R; cointime [ns]",   480, -24, 24); 
  TH1D *h1_ep_PcointimeROC2_L  = new TH1D("SHMS ROC2 Corrected ep Coin Time_L","SHMS ROC2 Corrected ep Coin Time_L; cointime [ns]",   480, -24, 24); 
  TH1D *h1_ep_PcointimeROC2_C  = new TH1D("SHMS ROC2 Corrected ep Coin Time_C","SHMS ROC2 Corrected ep Coin Time_C; cointime [ns]",   480, -24, 24); 

  TH1D *h1_ek_PcointimeROC2    = new TH1D("SHMS ROC2 Corrected ek Coin Time","SHMS ROC2 Corrected ek Coin Time; cointime [ns]",       480, -24, 24); 
  TH1D *h1_ek_PcointimeROC2_noPID    = new TH1D("SHMS ROC2 Corrected ek Coin Time NO PID","SHMS ROC2 Corrected ek Coin Time (no PID cut); cointime [ns]",       480, -24, 24); 
  TH1D *h1_ek_PcointimeROC2_R  = new TH1D("SHMS ROC2 Corrected ek Coin Time_R","SHMS ROC2 Corrected ek Coin Time_R; cointime [ns]",   480, -24, 24); 
  TH1D *h1_ek_PcointimeROC2_L  = new TH1D("SHMS ROC2 Corrected ek Coin Time_L","SHMS ROC2 Corrected ek Coin Time_L; cointime [ns]",   480, -24, 24); 
  TH1D *h1_ek_PcointimeROC2_C  = new TH1D("SHMS ROC2 Corrected ek Coin Time_C","SHMS ROC2 Corrected ek Coin Time_C; cointime [ns]",   480, -24, 24); 

  TH2D *h_aerosum_ecaltot_pi = new TH2D("h_aerosum_ecaltot_pi","SHMS Aero vs Etot (Cut ct_pi); Etot; Aero ",300,0.,1.5,80,0,20);
  TH2D *h_aerosum_ecaltot_p =  new TH2D("h_aerosum_ecaltot_p","SHMS Aero vs Etot (Cut ct_p); Etot; Aero ",300,0.,1.5,80,0,20);
  TH2D *h_aerosum_hgcer_p =  new TH2D("h_aerosum_hgcer_p","SHMS Aero vs HGCer (Cut ct_p); HG Cer; Aero ",80,0,20,80,0,20);
  TH2D *h_aerosum_ecaltot_k =  new TH2D("h_aerosum_ecaltot_k","SHMS Aero vs Etot (Cut ct_k); Etot; Aero ",300,0.,1.5,80,0,20);

  TH2D *h_aerosum_cointime_pi = new TH2D("h_aerosum_cointime_pi","No PID cut ; PIon Cointime (ns) ; Aero ",480, -24, 24,80,0,20);
  TH2D *h_cersum_cointime_pi = new TH2D("h_cersum_cointime_pi","No PID cut ; PIon Cointime (ns) ; HG Cer ",480, -24, 24,80,0,20);

  TH1D *h_Pprshower_tot = new TH1D("h_Pprshower_tot","; Preshower energy ; Counts",200,0.,1.);
  TH1D *h_Paero = new TH1D("h_Paero","; Aero NPE sum ; Counts",60,0.,15.);

  Double_t HgtrX, HgtrTh, HgtrY, HgtrPh, hdelta, PgtrX, PgtrTh, PgtrY, PgtrPh, pdelta;
  Double_t HgtrBetaCalc, PgtrBetaCalc, evtType, PgtrP, HgtrP, PhodStatus, PhodStartTime, PhodfpHitsTime;
  Double_t PgtrBetaCalcPi,PgtrBetaCalcK,PgtrBetaCalcP;
  Double_t cointime, HhodStatus, HhodStartTime, HhodfpHitsTime, paeronpe;
  Double_t pkinW, pEm, pPm, modPm, pbeta, hbeta, hcalepr, hcaletot, hcernpe;
  Double_t  pcaletot, pcalepr, pcernpe, pngcernpe;
  Double_t TcoinpTRIG1_ROC1_tdcTimeRaw, TcoinpTRIG4_ROC1_tdcTimeRaw, TcoinpTRIG1_ROC2_tdcTimeRaw;
  Double_t TcoinhTRIG1_ROC1_tdcTimeRaw, TcoinhTRIG1_ROC2_tdcTimeRaw, TcoinhTRIG4_ROC1_tdcTimeRaw;
  Double_t TcoinhTRIG4_ROC2_tdcTimeRaw, TcoinpTRIG4_ROC2_tdcTimeRaw;

  Int_t cntsepi=0;
  Int_t cntsep=0;
  Int_t cntsek=0;
  Int_t cntpos=0;

  tt->SetBranchAddress("P.gtr.p", &PgtrP); 
  tt->SetBranchAddress("H.gtr.p", &HgtrP); 
  tt->SetBranchAddress("H.gtr.th", &HgtrTh); 
  tt->SetBranchAddress("P.gtr.th", &PgtrTh); 
  tt->SetBranchAddress("P.gtr.beta", &pbeta);   
  tt->SetBranchAddress("H.gtr.beta", &hbeta); 
  tt->SetBranchAddress("H.gtr.dp", &hdelta);   
  tt->SetBranchAddress("P.gtr.dp", &pdelta);  
  tt->SetBranchAddress("P.cal.eprtracknorm", &pcalepr);
  tt->SetBranchAddress("P.cal.etottracknorm", &pcaletot); 
  tt->SetBranchAddress("P.hgcer.npeSum", &pcernpe);                                                 
  tt->SetBranchAddress("P.ngcer.npeSum", &pngcernpe);                                                 
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
  Double_t pOffset = -3.34; //9.5 + 10;  // in ns                                  
  Double_t hOffset = 335;                                                        
  Double_t speedOfLight = 29.9792; // in cm/ns                                   
  Double_t SHMScentralPathLen = 18.1*100;  // SHMS Target to focal plane path length converted to cm  
  Double_t SHMSpathLength = 18.1*100;      // For now assume that it's same as SHMScentralPathLen  
  Double_t HMScentralPathLen = 22*100;     // HMS Target to focal plane path length converted to cm
  Double_t DeltaHMSpathLength;  
  Double_t DeltaSHMSpathLength;  
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
     
  Bool_t epievent_cut, epevent_cut, ekevent_cut, positron_cut, event_cut, hpdelta_cut;
  //
  Double_t p_cent=0;
  for (int kk=0; kk<nentriesD;  kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
    evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    hpdelta_cut = hdelta > -10 && hdelta < 10 && pdelta > -10 && pdelta < 20 ;
    Double_t hms_elec = hcernpe > 1.0 && hcaletot > 0.6 && hcaletot < 2.0 && hcalepr > 0.2;


//event_cut = epievent_cut || epevent_cut || ekevent_cut ;
    event_cut = hpdelta_cut &&  hms_elec;

    if (!event_cut) continue;
    if (p_cent ==0) p_cent=PgtrP/(1+pdelta/100.);

    epievent_cut =hms_elec && hpdelta_cut; // default 
    if (shms_polarity == 1) {
      epievent_cut = pcalepr*PgtrP < 0.3 && pcalepr*PgtrP > 0.02 &&pcernpe <0.5 &&  paeronpe >= 2. && hms_elec && hpdelta_cut ;
          if (PgtrP>2.65) epievent_cut =pcalepr*PgtrP < 0.3 && pcalepr*PgtrP > 0.02 &&  pcernpe >=0.5 && paeronpe > 2. && hms_elec && hpdelta_cut ;
    }
    if (shms_polarity == 2) {
          epievent_cut =  pcalepr*PgtrP < 0.3 && pcalepr*PgtrP > 0.02 && pcernpe ==0 && hms_elec && hpdelta_cut ;
          if (PgtrP>=2.65 && PgtrP<4.2) epievent_cut =  pcalepr*PgtrP < 0.3 && pcalepr*PgtrP > 0.02 && pngcernpe ==0 && hms_elec && hpdelta_cut ; 
          if (PgtrP>=4.2) epievent_cut = pcalepr*PgtrP < 0.3 && pcalepr*PgtrP > 0.02 && pcaletot > 0.0 && pcaletot < 0.6 && hms_elec && hpdelta_cut ; 
    }
    //    epievent_cut = paeronpe > 0. && pcernpe > 0. && hcaletot > 0.6 && pcaletot > 0.6 && hpdelta_cut ;
    epevent_cut  =  pcalepr*PgtrP < 0.3 && pcalepr*PgtrP > 0.02 && paeronpe < 2.0  && hms_elec  && hpdelta_cut ;
    if (PgtrP>3.0) ekevent_cut  =  pcalepr*PgtrP < 0.3 && pcalepr*PgtrP > 0.02 && pcernpe <0.5 && paeronpe >= 2.0 && hms_elec   && hpdelta_cut ;
    if (PgtrP<=3.0) ekevent_cut  =  pcalepr*PgtrP < 0.3 && pcalepr*PgtrP > 0.02 && pcernpe ==0 && paeronpe == 0.0 && hms_elec   && hpdelta_cut ;
    positron_cut = paeronpe > 1.0 && pcernpe > 1.0 && pcalepr > 0.2 && hpdelta_cut && hms_elec ;
    if (positron_cut) cntpos++;

    if (epievent_cut) {
     SHMSpartMass = 0.1395704; // pion mass in GeV/c^2 
        cntsepi++;
    }    
    else if (epevent_cut) {
     SHMSpartMass = 0.93827231; // proton mass in GeV/c^2
        cntsep++;
    }            
    else if (ekevent_cut) {
     SHMSpartMass = 0.497648;// kaon mass in GeV/c^2
        cntsek++;
    } 

    PgtrBetaCalcP = PgtrP/sqrt(PgtrP*PgtrP + ( 0.93827231* 0.93827231));        
    PgtrBetaCalcK = PgtrP/sqrt(PgtrP*PgtrP + (0.497648*0.497648));        
    PgtrBetaCalcPi = PgtrP/sqrt(PgtrP*PgtrP + (0.1395704*0.1395704));        

        DeltaHMSpathLength = 12.462*HgtrTh + 0.1138*HgtrTh*HgtrX - 0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 0.0000544*HgtrX*HgtrX - 116.52*HgtrPh*HgtrPh;    
	PgtrBetaCalc = PgtrP/sqrt(PgtrP*PgtrP + SHMSpartMass*SHMSpartMass);        
	HgtrBetaCalc = HgtrP/sqrt(HgtrP*HgtrP + HMSpartMass*HMSpartMass);          
        DeltaSHMSpathLength= +0.11*atan2(PgtrTh,1)*1000;
	SHMScoinCorr =(SHMScentralPathLen+ DeltaSHMSpathLength)/speedOfLight*( 1./PgtrBetaCalc-1) - PhodfpHitsTime;  
	//	HMScoinCorr = -(HMScentralPathLen + DeltaHMSpathLength)/speedOfLight  + (HMScentralPathLen + DeltaHMSpathLength) / (speedOfLight*HgtrBetaCalc)  - HhodfpHitsTime;      
	HMScoinCorr = (HMScentralPathLen + DeltaHMSpathLength)/speedOfLight*(1./HgtrBetaCalc-1)  - HhodfpHitsTime;      
	SHMScorrCoinTimeROC1 = (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; // 0.1 to convert to ns 

	SHMScorrCoinTimeROC2 = (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; 

    Double_t SHMScoinCorrP = (SHMScentralPathLen+ DeltaSHMSpathLength)/speedOfLight*( 1./PgtrBetaCalcP-1) - PhodfpHitsTime; 
    Double_t coinP= (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorrP) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset;
    Double_t SHMScoinCorrK =   (SHMScentralPathLen+ DeltaSHMSpathLength)/speedOfLight*( 1./PgtrBetaCalcK-1) - PhodfpHitsTime; 
    Double_t coinK= (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorrK) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset;
    Double_t SHMScoinCorrPi =  (SHMScentralPathLen+ DeltaSHMSpathLength)/speedOfLight*( 1./PgtrBetaCalcPi-1) - PhodfpHitsTime; 
    Double_t coinPi= (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorrPi) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset;


	h1_epi_PcointimeROC2_noPID->Fill(coinPi);    
	h1_ep_PcointimeROC2_noPID->Fill(coinP);    
	h1_ek_PcointimeROC2_noPID->Fill(coinK);    
        h_aerosum_cointime_pi->Fill(coinPi,paeronpe);
        if (paeronpe > 2.0) h_cersum_cointime_pi->Fill(coinPi,pcernpe);
    h_aerosum_ecaltot_p->Fill(pcaletot,paeronpe);
    h_aerosum_hgcer_p->Fill(pcernpe,paeronpe);
    h_aerosum_ecaltot_k->Fill(pcaletot,paeronpe);
    h_aerosum_ecaltot_pi->Fill(pcaletot,paeronpe);
    h_Pprshower_tot->Fill(pcalepr*PgtrP);
    h_Paero->Fill(paeronpe);
    if (epievent_cut)
      {
	h1_epi_PcointimeROC2->Fill(coinPi);    
	h1_epi_PcointimeROC2_R->Fill(coinPi);
	h1_epi_PcointimeROC2_L->Fill(coinPi);
	h1_epi_PcointimeROC2_C->Fill(coinPi);
     }
    if (epevent_cut)
     {
	h1_ep_PcointimeROC2->Fill(coinP);    
	h1_ep_PcointimeROC2_R->Fill(coinP);
	h1_ep_PcointimeROC2_L->Fill(coinP);
	h1_ep_PcointimeROC2_C->Fill(coinP);
     }
    if (ekevent_cut)
     { 
                        
	h1_ek_PcointimeROC2->Fill(coinK);    
	h1_ek_PcointimeROC2_R->Fill(coinK);
	h1_ek_PcointimeROC2_L->Fill(coinK);
	h1_ek_PcointimeROC2_C->Fill(coinK);
     }     
  }

  cout<<" number of positrons in SHMS = " << cntpos << endl; 
  gROOT->SetBatch(kFALSE);

  Double_t max_cnt;
Double_t area1,area2,area3,av_acc,good_cnts;
  Double_t vel_p = speedOfLight*p_cent/sqrt(p_cent*p_cent+(.93827)*(.932827));
  Double_t vel_k = speedOfLight*p_cent/sqrt(p_cent*p_cent+(.496)*(.496));
  Double_t vel_pi = speedOfLight*p_cent/sqrt(p_cent*p_cent+(.13957)*(.13957));
  Double_t vel_p10m = speedOfLight*p_cent*.9/sqrt(p_cent*p_cent*.9*.9+(.93827)*(.932827));
  Double_t vel_p30p = speedOfLight*p_cent*1.2/sqrt(p_cent*p_cent*1.2*1.2+(.93827)*(.932827));
  Double_t good_coin_peak_time = 0.0 ; // from pi peak in run 3719
//  cout << "pcent = " << p_cent << " tdiff = " << tdiff << " " << 2200./vel_p << " " <<  2200./vel_pi << endl;
 

  Int_t bin_max;
  Double_t max_value;
if (shms_polarity == 1)  {
TCanvas *can3 = new TCanvas ("can3",Form("coin ep Run: %d",runNumber),700,1000);
  //*******************************************************************************
  can3->SetWindowPosition(500,100);
  can3->Divide(1,2); 
  can3->cd(1); 

  bin_max = h1_ep_PcointimeROC2->GetMaximumBin();
  max_value = h1_ep_PcointimeROC2->GetBinCenter(bin_max);

  Int_t bin_low =  h1_ep_PcointimeROC2->FindBin(max_value - 1.5);
  Int_t bin_hi = h1_ep_PcointimeROC2->FindBin(max_value + 1.5);
  h1_ep_PcointimeROC2_C->GetXaxis()->SetRange(bin_low,bin_hi);

  Int_t bin1 = h1_ep_PcointimeROC2->FindBin(max_value + 6);
  Int_t bin2 = h1_ep_PcointimeROC2->FindBin(max_value + 18);                                                           
  h1_ep_PcointimeROC2_R->GetXaxis()->SetRange(bin1,bin2);

  Int_t bin3 = h1_ep_PcointimeROC2->FindBin(max_value - 3);
  Int_t bin4 = h1_ep_PcointimeROC2->FindBin(max_value - 15);
  h1_ep_PcointimeROC2_L->GetXaxis()->SetRange(bin4,bin3);

  h1_ep_PcointimeROC2->SetLineColor(kBlack);
  h1_ep_PcointimeROC2_R->SetLineColor(kRed);
  h1_ep_PcointimeROC2_L->SetLineColor(kRed);
  h1_ep_PcointimeROC2_C->SetLineColor(kBlue);

  h1_ep_PcointimeROC2->SetLineWidth(2);
  h1_ep_PcointimeROC2->Draw();
  h1_ep_PcointimeROC2_R->Draw("same");
  h1_ep_PcointimeROC2_L->Draw("same");
  h1_ep_PcointimeROC2_C->Draw("same");


  can3->cd(2); 
  h1_ep_PcointimeROC2_noPID->Draw();
  max_cnt=h1_ep_PcointimeROC2_noPID->GetMaximum();
  TLine *clinep[8];
  for (Int_t ip=0;ip<9;ip++) {
    clinep[ip]= new TLine(good_coin_peak_time+(ip-4)*4,0.,good_coin_peak_time+(ip-4)*4,max_cnt/2.); 
    clinep[ip]->Draw();
    clinep[ip]->SetLineColor(2);
    clinep[ip]->SetLineWidth(2);
    if ((ip-4)==0) clinep[ip]->SetLineColor(3);
  }
  // Draw expected accidental proton peak for p_cent
  Double_t tdiffpk = SHMScentralPathLen*(1./vel_k-1./vel_p);
  TLine *pline2 = new TLine(good_coin_peak_time+tdiffpk,0.,good_coin_peak_time+tdiffpk,max_cnt/2.);
  pline2->Draw();
  pline2->SetLineColor(6);
  Double_t tdiff10mpk = SHMScentralPathLen*(1./vel_k-1./vel_p10m);
  Double_t tdiff30ppk = SHMScentralPathLen*(1./vel_k-1./vel_p30p);
  TLine *parrow2 = new TArrow(good_coin_peak_time+tdiff10mpk,max_cnt/4.,good_coin_peak_time+tdiff30ppk,max_cnt/4.,.005,"<>");
  parrow2->Draw();
  parrow2->SetLineColor(6);
  Double_t tkdiffppi = SHMScentralPathLen*(1./vel_pi-1./vel_p);
  TLine *kline1 = new TLine(good_coin_peak_time+tkdiffppi,0.,good_coin_peak_time+tkdiffppi,max_cnt/2.);
  kline1->Draw();
  kline1->SetLineColor(4);
  kline1->SetLineWidth(2);
  Double_t tdiff10mppi = SHMScentralPathLen*(1./vel_pi-1./vel_p10m);
  Double_t tdiff30pppi = SHMScentralPathLen*(1./vel_pi-1./vel_p30p);
  TLine *parrowpi = new TArrow(good_coin_peak_time+tdiff10mppi,max_cnt/3.,good_coin_peak_time+tdiff30pppi,max_cnt/3.,.005,"<>");
  parrowpi->Draw();
  parrowpi->SetLineColor(4);
  can3->cd(1);
  max_cnt=h1_ep_PcointimeROC2->GetMaximum();
 pline2 = new TLine(good_coin_peak_time+tdiffpk,0.,good_coin_peak_time+tdiffpk,max_cnt/2.);
   pline2->Draw();
  pline2->SetLineColor(6);
kline1 = new TLine(good_coin_peak_time+tkdiffppi,0.,good_coin_peak_time+tkdiffppi,max_cnt/2.);
 kline1->Draw();
  kline1->SetLineColor(4);
  kline1->SetLineWidth(2);
  parrowpi = new TArrow(good_coin_peak_time+tdiff10mppi,max_cnt/3.,good_coin_peak_time+tdiff30pppi,max_cnt/3.,.005,"<>");
  parrowpi->Draw();
  parrowpi->SetLineColor(4);
  pline2->Draw();
  parrow2= new TArrow(good_coin_peak_time+tdiff10mpk,max_cnt/4.,good_coin_peak_time+tdiff30ppk,max_cnt/4.,.005,"<>");
  parrow2->Draw();
  parrow2->SetLineColor(6);
  
  can3->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_ep_cointime.pdf",runNumber));
 

  area1 =h1_ep_PcointimeROC2->Integral(bin_low, bin_hi);
  cout<<"The integral of mainpeak for ep events= "<<area1<<endl;


  area2 =h1_ep_PcointimeROC2->Integral(bin4, bin3);
  cout<<"The integral of left Red Portion for ep events= "<<area2<<endl;
  //  cout<<bin_low<< "   \t"<<bin_hi<<endl;


  area3 =h1_ep_PcointimeROC2->Integral(bin1, bin2);
   cout<<"The integral of right Red Portion ep events = "<<area3<<endl;
  // cout<<bin_low<< "   \t"<<bin_hi<<endl;



  av_acc = (area2 + area3)/24 ;
  good_cnts = area1 - av_acc*3;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;
  cout << "NOTE:>>>>  " << good_cnts <<"  <<<<<  GOOD ep EVENTS have passed all cuts (update the white board and excel sheet)" << endl;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;


TCanvas *can4 = new TCanvas ("can4",Form("coin eK Run: %d",runNumber),700,1000);
  //*******************************************************************************
  can4->SetWindowPosition(100,100);
  can4->Divide(1,2); 
  can4->cd(1); 

  bin_max = h1_ek_PcointimeROC2->GetMaximumBin();
  max_value = h1_ek_PcointimeROC2->GetBinCenter(bin_max);
  max_value=0;
  bin_low =  h1_ek_PcointimeROC2->FindBin(max_value - 1.5);
  bin_hi = h1_ek_PcointimeROC2->FindBin(max_value + 1.5);
  h1_ek_PcointimeROC2_C->GetXaxis()->SetRange(bin_low,bin_hi);

  bin1 = h1_ek_PcointimeROC2->FindBin(max_value + 6);
  bin2 = h1_ek_PcointimeROC2->FindBin(max_value + 18);                                                           
  h1_ek_PcointimeROC2_R->GetXaxis()->SetRange(bin1,bin2);

  bin3 = h1_ek_PcointimeROC2->FindBin(max_value - 3);
  bin4 = h1_ek_PcointimeROC2->FindBin(max_value - 15);
  h1_ek_PcointimeROC2_L->GetXaxis()->SetRange(bin4,bin3);

  h1_ek_PcointimeROC2->SetLineColor(kBlack);
  h1_ek_PcointimeROC2_R->SetLineColor(kRed);
  h1_ek_PcointimeROC2_L->SetLineColor(kRed);
  h1_ek_PcointimeROC2_C->SetLineColor(kBlue);

  h1_ek_PcointimeROC2->SetLineWidth(2);
  h1_ek_PcointimeROC2->Draw();
  h1_ek_PcointimeROC2_R->Draw("same");
  h1_ek_PcointimeROC2_L->Draw("same");
  h1_ek_PcointimeROC2_C->Draw("same");
  can4->cd(2); 

  h1_ek_PcointimeROC2_noPID->Draw();
  max_cnt=h1_ek_PcointimeROC2_noPID->GetMaximum();
  TLine *clinek[8];
  for (Int_t ip=0;ip<9;ip++) {
    clinek[ip]= new TLine(good_coin_peak_time+(ip-4)*4,0.,good_coin_peak_time+(ip-4)*4,max_cnt/2.); 
    clinek[ip]->Draw();
    clinek[ip]->SetLineColor(2);
    clinek[ip]->SetLineWidth(2);
    if ((ip-4)==0) clinek[ip]->SetLineColor(6);
  }
  // Draw expected accidental proton peak for p_cent
  Double_t tdiffkp = SHMScentralPathLen*(1./vel_p-1./vel_k);
  TLine *pline = new TLine(good_coin_peak_time+tdiffkp,0.,good_coin_peak_time+tdiffkp,max_cnt/2.);
  pline->Draw();
  pline->SetLineColor(3);
  Double_t tdiff10mkp = SHMScentralPathLen*(1./vel_p10m-1./vel_k);
  Double_t tdiff30pkp = SHMScentralPathLen*(1./vel_p30p-1./vel_k);
  TLine *parrow = new TArrow(good_coin_peak_time+tdiff10mkp,max_cnt/4.,good_coin_peak_time+tdiff30pkp,max_cnt/4.,.005,"<>");
  parrow->Draw();
  parrow->SetLineColor(3);
  Double_t tkdiffkpi = SHMScentralPathLen*(1./vel_pi-1./vel_k);
  TLine *kline = new TLine(good_coin_peak_time+tkdiffkpi,0.,good_coin_peak_time+tkdiffkpi,max_cnt/2.);
  kline->Draw();
  kline->SetLineColor(4);
  can4->cd(1);
  max_cnt=h1_ek_PcointimeROC2->GetMaximum();
  kline = new TLine(good_coin_peak_time+tkdiffkpi,0.,good_coin_peak_time+tkdiffkpi,max_cnt/2.);
  kline->Draw();
 kline->SetLineColor(4);
  parrow = new TArrow(good_coin_peak_time+tdiff10mkp,max_cnt/4.,good_coin_peak_time+tdiff30pkp,max_cnt/4.,.005,"<>");
  parrow->Draw();
  parrow->SetLineColor(3);
  pline = new TLine(good_coin_peak_time+tdiffkp,0.,good_coin_peak_time+tdiffkp,max_cnt/2.);
  pline->Draw();
  pline->SetLineColor(3);
  can4->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_ek_cointime.pdf",runNumber));
   

  area1 =h1_ek_PcointimeROC2->Integral(bin_low, bin_hi);
  cout<<"The integral of mainpeak for ek= "<<area1<<endl;
  


  area2 =h1_ek_PcointimeROC2->Integral(bin4, bin3);
  cout<<"The integral of left Red Portion for ek = "<<area2<<endl;
  //  cout<<bin_low<< "   \t"<<bin_hi<<endl;


   area3 =h1_ek_PcointimeROC2->Integral(bin1, bin2);
   cout<<"The integral of right Red Portion ek = "<<area3<<endl;
  // cout<<bin_low<< "   \t"<<bin_hi<<endl;



  av_acc = (area2 + area3)/24 ;
  good_cnts = area1 - av_acc*3;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;
  cout << "NOTE:>>>>  " << good_cnts <<"  <<<<<  GOOD ek EVENTS have passed all cuts (update the white board and excel sheet)" << endl;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;
  //
 } // pos shms polarity
//

  TCanvas *can2 = new TCanvas ("can2",Form("Coin pi Run: %d",runNumber),700,1000);
can2->Divide(1,2);
  //*******************************************************************************
  can2->SetWindowPosition(100,1000);
  can2->cd(1); 

  bin_max = h1_epi_PcointimeROC2->GetMaximumBin();
  max_value = h1_epi_PcointimeROC2->GetBinCenter(bin_max);

  bin_low =  h1_epi_PcointimeROC2->FindBin(max_value - 1.5);
  bin_hi = h1_epi_PcointimeROC2->FindBin(max_value + 1.5);
  h1_epi_PcointimeROC2_C->GetXaxis()->SetRange(bin_low,bin_hi);

  bin1 = h1_epi_PcointimeROC2->FindBin(max_value + 6);
  bin2 = h1_epi_PcointimeROC2->FindBin(max_value + 18);                                                           
  h1_epi_PcointimeROC2_R->GetXaxis()->SetRange(bin1,bin2);

  bin3 = h1_epi_PcointimeROC2->FindBin(max_value - 3);
  bin4 = h1_epi_PcointimeROC2->FindBin(max_value - 15);
  h1_epi_PcointimeROC2_L->GetXaxis()->SetRange(bin4,bin3);

  h1_epi_PcointimeROC2->SetLineColor(kBlack);
  h1_epi_PcointimeROC2_R->SetLineColor(kRed);
  h1_epi_PcointimeROC2_L->SetLineColor(kRed);
  h1_epi_PcointimeROC2_C->SetLineColor(kBlue);

  h1_epi_PcointimeROC2->SetLineWidth(2);
  h1_epi_PcointimeROC2->Draw();
  h1_epi_PcointimeROC2_R->Draw("same");
  h1_epi_PcointimeROC2_L->Draw("same");
  h1_epi_PcointimeROC2_C->Draw("same");

  TLine *cline1[8];
  Double_t max_cnt1=h1_epi_PcointimeROC2->GetMaximum();
  for (Int_t ip=0;ip<9;ip++) {
    cline1[ip]= new TLine(max_value+(ip-4)*4,0.,max_value+(ip-4)*4,max_cnt1/2.); 
    cline1[ip]->Draw();
    cline1[ip]->SetLineColor(2);
    cline1[ip]->SetLineWidth(2);
    if ((ip-4)==0) cline1[ip]->SetLineColor(4);
  }

  can2->cd(2); 
  h1_epi_PcointimeROC2_noPID->Draw();
if (shms_polarity == 1)  {
  max_cnt=h1_epi_PcointimeROC2_noPID->GetMaximum();
  TLine *cline[8];
  for (Int_t ip=0;ip<9;ip++) {
    cline[ip]= new TLine(good_coin_peak_time+(ip-4)*4,0.,good_coin_peak_time+(ip-4)*4,max_cnt/2.); 
    cline[ip]->Draw();
    cline[ip]->SetLineColor(2);
    cline[ip]->SetLineWidth(2);
    if ((ip-4)==0) cline[ip]->SetLineColor(4);
  }
  // Draw expected accidental proton peak for p_cent
  Double_t tdiff = SHMScentralPathLen*(1./vel_p-1./vel_pi);
  TLine *pline1 = new TLine(good_coin_peak_time+tdiff,0.,good_coin_peak_time+tdiff,max_cnt/2.);
  pline1->Draw();
  pline1->SetLineColor(3);
  Double_t tdiff10m = SHMScentralPathLen*(1./vel_p10m-1./vel_pi);
  Double_t tdiff30p = SHMScentralPathLen*(1./vel_p30p-1./vel_pi);
  TLine *parrow = new TArrow(good_coin_peak_time+tdiff10m,max_cnt/4.,good_coin_peak_time+tdiff30p,max_cnt/4.,.005,"<>");
  parrow->Draw();
  parrow->SetLineColor(3);
  Double_t tkdiff = SHMScentralPathLen*(1./vel_k-1./vel_pi);
  TLine *kline2 = new TLine(good_coin_peak_time+tkdiff,0.,good_coin_peak_time+tkdiff,max_cnt/2.);
  kline2->Draw();
  kline2->SetLineColor(6);
 }
//
can2->cd(1);
if (shms_polarity == 1)  {
  max_cnt=h1_epi_PcointimeROC2->GetMaximum();
kline2 = new TLine(good_coin_peak_time+tkdiff,0.,good_coin_peak_time+tkdiff,max_cnt/2.);
  kline2->Draw();
  kline2->SetLineColor(6);
  TPaveText *ktext = new TPaveText(-24.,max_cnt*.8,-12., max_cnt*.95);
  ktext->AddText(" Kaon cyan vert line");
  ktext->AddText(" Pion blue vert line");
  ktext->AddText(" Proton green vert line");
  ktext->Draw();
 parrow = new TArrow(good_coin_peak_time+tdiff10m,max_cnt/4.,good_coin_peak_time+tdiff30p,max_cnt/4.,.005,"<>");
  parrow->Draw();
  parrow->SetLineColor(3);
  pline1 = new TLine(good_coin_peak_time+tdiff,0.,good_coin_peak_time+tdiff,max_cnt/2.);
  pline1->Draw();
  pline1->SetLineColor(3);
 }
//

  can2->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_epi_cointime.pdf",runNumber));
   
  area1 =h1_epi_PcointimeROC2->Integral(bin_low, bin_hi);
  cout<<"The integral of mainpeak for epi= "<<area1<<endl;
  cout<<bin_low<< "   \t"<<bin_hi<<endl;


  area2 =h1_epi_PcointimeROC2->Integral(bin4, bin3);
  cout<<"The integral of left Red Portion for epi = "<<area2<<endl;
  cout<<bin4<< "   \t"<<bin3<<endl;


  area3 =h1_epi_PcointimeROC2->Integral(bin1, bin2);
   cout<<"The integral of right Red Portion epi = "<<area3<<endl;
  cout<<bin1<< "   \t"<<bin2<<endl;



  av_acc = (area2 + area3)/24 ;
  good_cnts = area1 - av_acc*3;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;
  cout << "NOTE:>>>>  " << good_cnts <<"  <<<<<  GOOD epi EVENTS have passed all cuts (update the white board and excel sheet)" << endl;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;

    TCanvas *can2d = new TCanvas ("can2d",Form("ROC correlation plots Run: %d",runNumber),800,1000);
  can2d->SetWindowPosition(500,500);
  can2d->Divide(2,2);
  can2d->cd(1);
  gStyle->SetOptStat(0);
  if (dcref_vs_hodoref) {
  dcref_vs_hodoref->Draw("colz");
  } else {
    cout << " no HMS reference time histo" << endl;
  }
  can2d->cd(2);
  gStyle->SetOptStat(0);
  if (shms_dcref_vs_hodoref) {
  shms_dcref_vs_hodoref->Draw("colz");
  } else {
    cout << " no SHMS reference time histo" << endl;
  }
  can2d->cd(3);
  gStyle->SetOptStat(0);
  if (raster_frx_hms_shms) {
  raster_frx_hms_shms->Draw("colz");
  } else {
    cout << " no SHMS reference time histo" << endl;
  }

}
