// Filename: Bean_counter.C
// Description: delta, xptar,yptar, ytar distributions for SHMS and HMS and W, Em, Pm and Cointime distributions 
// are plotted and the number of good counts are printed out to add to the count of good events.
// Needs runnumber, and if target is "h" or "d"

void Bean_counter(Int_t runNumber, Int_t targ=1,Int_t ev=-1){

  //read the input file from data
  TString fileNameD = "/net/cdaq/cdaql3data/cdaq/sidis-fall-18/ROOTfiles/";
  fileNameD += "coin_replay_production_"; //read the root file from data
  fileNameD += runNumber; //read the root file from data
  fileNameD += Form("_%d.root",ev); //read the root file from data
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
  TH1D *h1_epi_PcointimeROC2_R  = new TH1D("SHMS ROC2 Corrected epi Coin Time_R","SHMS ROC2 Corrected epi Coin Time_R; cointime [ns]",   480, -24, 24); 
  TH1D *h1_epi_PcointimeROC2_L  = new TH1D("SHMS ROC2 Corrected epi Coin Time_L","SHMS ROC2 Corrected epi Coin Time_L; cointime [ns]",   480, -24, 24); 
  TH1D *h1_epi_PcointimeROC2_C  = new TH1D("SHMS ROC2 Corrected epi Coin Time_C","SHMS ROC2 Corrected epi Coin Time_C; cointime [ns]",   480, -24, 24); 
  TH2D *h_reactz_diff_cointtime = new TH2D("h_reactz_diff_cointtime",";SHMS Reactz - HMS reactz (cm) ; SHMS ROC2 Corrected epi Coin Time",200,-5,5,480, -24, 24);

  Double_t HgtrX, HgtrTh, HgtrY, HgtrPh, hdelta, PgtrX, PgtrTh, PgtrY, PgtrPh, pdelta;
  Double_t HgtrBetaCalc, PgtrBetaCalc, evtType, PgtrP, HgtrP, PhodStatus, PhodStartTime, PhodfpHitsTime;
  Double_t cointime, HhodStatus, HhodStartTime, HhodfpHitsTime, paeronpe;
  Double_t pkinW, pEm, pPm, modPm, pbeta, hbeta, hcalepr, hcaletot, hcernpe, pcaletot, pcalepr, pcernpe;
  Double_t TcoinpTRIG1_ROC1_tdcTimeRaw, TcoinpTRIG4_ROC1_tdcTimeRaw, TcoinpTRIG1_ROC2_tdcTimeRaw;
  Double_t TcoinhTRIG1_ROC1_tdcTimeRaw, TcoinhTRIG1_ROC2_tdcTimeRaw, TcoinhTRIG4_ROC1_tdcTimeRaw;
  Double_t TcoinhTRIG4_ROC2_tdcTimeRaw, TcoinpTRIG4_ROC2_tdcTimeRaw;
  Double_t Preactz,Hreactz;

  Int_t cntsepi=0;
  Int_t cntsep=0;
  Int_t cntsek=0;
  Int_t cntpos=0;

  tt->SetBranchAddress("P.react.z", &Preactz); 
  tt->SetBranchAddress("H.react.z", &Hreactz); 
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
  TCut epiCut, epcut;                                                                   



  hpdelta = "P.gtr.dp > -10 && P.gtr.dp < 20 && H.gtr.dp > -10 && H.gtr.dp < 10";
  epiCut = "P.aero.npeSum > -1.0 && P.cal.eprtracknorm < 0.2 && H.cer.npeSum > 1.0 && H.cal.etottracknorm > 0.6 && H.cal.etottracknorm < 2.0 && H.cal.eprtracknorm  > 0.2"; 
  //  epCut = "P.aero.npeSum < 2.0 && P.hgcer.npeSum < 2.0 && P.cal.eprtracknorm < 0.2 && H.cer.npeSum > 1.0 && H.cal.etottracknorm > 0.6 && H.cal.etottracknorm < 2.0 && H.cal.eprtracknorm  > 0.2"; 

  
  TCanvas *canvas1 = new TCanvas("canvas1","canvas1");                           
  //  tt->Draw("P.hod.starttime >> SHMShodoStartTime", epiCut && hpdelta );  
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
  Double_t SHMSpartMass = 0.139 ;                  
  Double_t pOffset = 1.5 + 17. ; //9.5 + 10;  // in ns                                  
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
  Double_t HMScorrCoinTimeROC2, ctimepi;   
     
  Bool_t epievent_cut, positron_cut, event_cut, hpdelta_cut; //epevent_cut, ekevent_cut, 
  //
  Double_t p_cent=0;
  for (int kk=0; kk<nentriesD;  kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
    evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    hpdelta_cut = hdelta > -10 && hdelta < 10 && pdelta > -10 && pdelta < 20 ;
    //    epievent_cut = paeronpe > 0. && pcernpe > 0. && hcaletot > 0.6 && hcaletot < 2.0 && hpdelta_cut ;
    epievent_cut = paeronpe > 3. && pcaletot >.2 && hcaletot > 0.6 && hcaletot < 2.0 && hpdelta_cut  && hcernpe > 0.;
    positron_cut = paeronpe > 1.0 && pcernpe > 1.0 && pcalepr > 0.2 && hpdelta_cut ;


    event_cut = epievent_cut;// || epevent_cut || ekevent_cut ;

    if (positron_cut) cntpos++;

    p_cent = 3.44 ;
    if (epievent_cut) {
        if (p_cent ==0) p_cent=PgtrP/(1+pdelta/100.);
     SHMSpartMass = 0.1395704; // pion mass in GeV/c^2 
        cntsepi++;
        


	DeltaHMSpathLength = 12.462*HgtrTh + 0.1138*HgtrTh*HgtrX - 0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 0.0000544*HgtrX*HgtrX - 116.52*HgtrPh*HgtrPh;               
	PgtrBetaCalc = PgtrP/sqrt(PgtrP*PgtrP + SHMSpartMass*SHMSpartMass);        
	HgtrBetaCalc = HgtrP/sqrt(HgtrP*HgtrP + HMSpartMass*HMSpartMass);          

	SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*PgtrBetaCalc) + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
	HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / (speedOfLight*HgtrBetaCalc) + (HhodoStartTimeMean - HhodfpHitsTime);      

	SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*PgtrBetaCalc) + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
	HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / (speedOfLight*HgtrBetaCalc) + (HhodoStartTimeMean - HhodfpHitsTime);      

	SHMScorrCoinTimeROC1 = (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; // 0.1 to convert to ns 

	SHMScorrCoinTimeROC2 = (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; 

       SHMScorrCoinTimeROC2 = 
         (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - 
         SHMScoinCorr) - 
         (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - 
         HMScoinCorr) - pOffset; 

       ctimepi = SHMScorrCoinTimeROC2 ;

       //       printf("%7.2f %7.2f %7.2f %7.2f %7.2f\n",ctimepi,SHMScorrCoinTimeROC1, TcoinpTRIG1_ROC2_tdcTimeRaw*0.1,SHMScoinCorr,TcoinpTRIG4_ROC2_tdcTimeRaw*0.1, HMScoinCorr, pOffset) ;

        h_reactz_diff_cointtime->Fill(Preactz+Hreactz,SHMScorrCoinTimeROC2);
	h1_epi_PcointimeROC2->Fill(SHMScorrCoinTimeROC2);    
	h1_epi_PcointimeROC2_R->Fill(SHMScorrCoinTimeROC2);
	h1_epi_PcointimeROC2_L->Fill(SHMScorrCoinTimeROC2);
	h1_epi_PcointimeROC2_C->Fill(SHMScorrCoinTimeROC2);
     }
	
  }

  gROOT->SetBatch(kFALSE);

  TCanvas *can2 = new TCanvas ("can2",Form("can2 Run: %d",runNumber));

  Int_t bin_max = h1_epi_PcointimeROC2->GetMaximumBin();
  Double_t max_value = h1_epi_PcointimeROC2->GetBinCenter(bin_max);

  Int_t bin_low =  h1_epi_PcointimeROC2->FindBin(max_value - 1.5);
  Int_t bin_hi = h1_epi_PcointimeROC2->FindBin(max_value + 1.5);
  h1_epi_PcointimeROC2_C->GetXaxis()->SetRange(bin_low,bin_hi);

  // 0.1ns = 1 bin;
  Int_t sh1=max_value + 3*4 -1.5;
  Int_t sh2=max_value + 4*4 +2.;
  Int_t bin1 = h1_epi_PcointimeROC2->FindBin(sh1);
  Int_t bin2 = h1_epi_PcointimeROC2->FindBin(sh2);                                                           
  h1_epi_PcointimeROC2_R->GetXaxis()->SetRange(bin1,bin2);

  Int_t sh3=max_value - 1*4 +1.5;
  Int_t sh4=max_value - 3*4 -1.5;
  Int_t bin3 = h1_epi_PcointimeROC2->FindBin(sh3);
  Int_t bin4 = h1_epi_PcointimeROC2->FindBin(sh4);
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

  // draw the expected location of accidental peaks

  Double_t max_cnt=h1_epi_PcointimeROC2->GetMaximum();
  TLine *cline[8];
  for (Int_t ip=0;ip<9;ip++) {
    cline[ip]= new TLine(max_value+(ip-4)*4,0.,max_value+(ip-4)*4,max_cnt/2.); 
    cline[ip]->Draw();
    cline[ip]->SetLineColor(2);
    cline[ip]->SetLineWidth(2);
    if ((ip-4)==0) cline[ip]->SetLineColor(4);
  }
  // Draw expected accidental proton peak for p_cent
  Double_t vel_p = speedOfLight*p_cent/sqrt(p_cent*p_cent+(.93827)*(.932827));
  Double_t vel_k = speedOfLight*p_cent/sqrt(p_cent*p_cent+(.496)*(.496));
  Double_t vel_pi = speedOfLight*p_cent/sqrt(p_cent*p_cent+(.13957)*(.13957));
  Double_t tdiff = SHMScentralPathLen*(1./vel_p-1./vel_pi);
  cout << "pcent = " << p_cent << " tdiff = " << tdiff << " " << 2200./vel_p << " " <<  2200./vel_pi << endl;
  TLine *pline = new TLine(max_value+tdiff,0.,max_value+tdiff,max_cnt/2.);
  pline->Draw();
  pline->SetLineColor(3);
  cout << "pcent = " << p_cent << " tdiff = " << tdiff << " " << 2200./vel_p << " " <<  2200./vel_pi << endl;
  Double_t vel_p10m = speedOfLight*p_cent*.9/sqrt(p_cent*p_cent*.9*.9+(.93827)*(.932827));
  Double_t tdiff10m = SHMScentralPathLen*(1./vel_p10m-1./vel_pi);
  Double_t vel_p30p = speedOfLight*p_cent*1.3/sqrt(p_cent*p_cent*1.3*1.3+(.93827)*(.932827));
  Double_t tdiff30p = SHMScentralPathLen*(1./vel_p30p-1./vel_pi);
  TLine *parrow = new TArrow(max_value+tdiff10m,max_cnt/4.,max_value+tdiff30p,max_cnt/4.,.01,"<>");
  parrow->Draw();
  parrow->SetLineColor(3);
  Double_t tkdiff = SHMScentralPathLen*(1./vel_k-1./vel_pi);
  TLine *kline = new TLine(max_value+tkdiff,0.,max_value+tkdiff,max_cnt/2.);
  kline->Draw();
  kline->SetLineColor(6);

  can2->cd(1); 
  can2->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_epi_cointime.pdf",runNumber));
   

  Double_t area1 =h1_epi_PcointimeROC2->Integral(bin_low, bin_hi);
  cout<<"The integral of mainpeak for epi= "<<area1<<endl;
  //  cout<<bin_low<< "   \t"<<bin_hi<<endl;


  Double_t area2 =h1_epi_PcointimeROC2->Integral(bin4, bin3);
  cout<<"The integral of left Red Portion for epi = "<<area2<<endl;
  //  cout<<bin_low<< "   \t"<<bin_hi<<endl;


  Double_t area3 =h1_epi_PcointimeROC2->Integral(bin1, bin2);
   cout<<"The integral of right Red Portion epi = "<<area3<<endl;
  // cout<<bin_low<< "   \t"<<bin_hi<<endl;



   Double_t av_acc = (area2 + area3)/((sh2-sh1)+(sh3-sh4)) ;
  Int_t good_cnts = area1 - av_acc*3;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;
  cout << "NOTE:>>>> " << good_cnts <<"  <<<<<  GOOD epi EVENTS have passed all cuts (update the wiki and run book)" << endl;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;

    TCanvas *can3 = new TCanvas ("can3",Form("ROC correlation plots Run: %d",runNumber),800,1000);
  can3->SetWindowPosition(500,500);
  can3->Divide(2,2);
  can3->cd(1);
  gStyle->SetOptStat(0);
  if (dcref_vs_hodoref) {
  dcref_vs_hodoref->Draw("colz");
  } else {
    cout << " no HMS reference time histo" << endl;
  }
  can3->cd(2);
  gStyle->SetOptStat(0);
  if (shms_dcref_vs_hodoref) {
  shms_dcref_vs_hodoref->Draw("colz");
  } else {
    cout << " no SHMS reference time histo" << endl;
  }
  can3->cd(3);
  gStyle->SetOptStat(0);
  if (raster_frx_hms_shms) {
  raster_frx_hms_shms->Draw("colz");
  } else {
    cout << " no SHMS reference time histo" << endl;
  }
  

}
