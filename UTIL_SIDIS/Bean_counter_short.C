// Filename: Bean_counter.C
// Description: delta, xptar,yptar, ytar distributions for SHMS and HMS and W, Em, Pm and Cointime distributions 
// are plotted and the number of good counts are printed out to add to the count of good events.
// Needs runnumber, and if target is "h" or "d"

void Bean_counter_short(Int_t runNumber, Int_t targ=1){

  //read the input file from data
  TString fileNameD = "/net/cdaq/cdaql3data/cdaq/hallc-online/ROOTfiles/" ;
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


  Double_t HgtrX, HgtrTh, HgtrY, HgtrPh, hdelta, PgtrX, PgtrTh, PgtrY, PgtrPh, pdelta;
  Double_t HgtrBetaCalc, PgtrBetaCalc, evtType, PgtrP, HgtrP, PhodStatus, PhodStartTime, PhodfpHitsTime;
  Double_t cointime, HhodStatus, HhodStartTime, HhodfpHitsTime, paeronpe;
  Double_t pkinW, pEm, pPm, modPm, pbeta, hbeta, hcalepr, hcaletot, hcernpe, pcaletot, pcalepr, pcernpe;
  Double_t TcoinpTRIG1_ROC1_tdcTimeRaw, TcoinpTRIG4_ROC1_tdcTimeRaw, TcoinpTRIG1_ROC2_tdcTimeRaw;
  Double_t TcoinhTRIG1_ROC1_tdcTimeRaw, TcoinhTRIG1_ROC2_tdcTimeRaw, TcoinhTRIG4_ROC1_tdcTimeRaw;
  Double_t TcoinhTRIG4_ROC2_tdcTimeRaw, TcoinpTRIG4_ROC2_tdcTimeRaw;

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
                                                                                   
  Double_t SHMScorrCoinTimeROC1;                                                 
  Double_t SHMScorrCoinTimeROC2;                                                 
  Double_t HMScorrCoinTimeROC1;                                                  
  Double_t HMScorrCoinTimeROC2;   
     
  Bool_t epievent_cut, positron_cut, event_cut, hpdelta_cut; //epevent_cut, ekevent_cut, 
  //
  for (int kk=0; kk<nentriesD;  kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
    evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    hpdelta_cut = hdelta > -10 && hdelta < 10 && pdelta > -10 && pdelta < 20 ;
    //    epievent_cut = paeronpe > 0. && pcernpe > 0. && hcaletot > 0.6 && hcaletot < 2.0 && hpdelta_cut ;
    epievent_cut = paeronpe > 0. && hcaletot > 0.6 && hcaletot < 2.0 && hpdelta_cut  && hcernpe > 0.;
    positron_cut = paeronpe > 1.0 && pcernpe > 1.0 && pcalepr > 0.2 && hpdelta_cut ;


    event_cut = epievent_cut;// || epevent_cut || ekevent_cut ;

    if (positron_cut) cntpos++;


    if (epievent_cut) {
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

  Int_t bin1 = h1_epi_PcointimeROC2->FindBin(max_value + 6);
  Int_t bin2 = h1_epi_PcointimeROC2->FindBin(max_value + 18);                                                           
  h1_epi_PcointimeROC2_R->GetXaxis()->SetRange(bin1,bin2);

  Int_t bin3 = h1_epi_PcointimeROC2->FindBin(max_value - 3);
  Int_t bin4 = h1_epi_PcointimeROC2->FindBin(max_value - 15);
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


  can2->cd(1); 
  can2->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_epi_cointime.pdf",runNumber));
   

  Double_t area1 =h1_epi_PcointimeROC2->Integral(bin_low, bin_hi);
  cout<<"The integral of mainpeak for epi= "<<area1<<endl;
  // cout<<bin_low<< "   \t"<<bin_hi<<endl;


  Double_t area2 =h1_epi_PcointimeROC2->Integral(bin4, bin3);
  cout<<"The integral of left Red Portion for epi = "<<area2<<endl;
  //  cout<<bin_low<< "   \t"<<bin_hi<<endl;


  Double_t area3 =h1_epi_PcointimeROC2->Integral(bin1, bin2);
   cout<<"The integral of right Red Portion epi = "<<area3<<endl;
  // cout<<bin_low<< "   \t"<<bin_hi<<endl;



  Double_t av_acc = (area2 + area3)/24 ;
  Int_t good_cnts = area1 - av_acc*3;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;
  cout << "NOTE:>>>>  " << good_cnts <<"  <<<<<  GOOD epi EVENTS have passed all cuts (update the wiki and run book)" << endl;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;


  

}
