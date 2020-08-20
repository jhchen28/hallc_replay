// Filename: Bean_counter.C
// Description: delta, xptar,yptar, ytar distributions for SHMS and HMS and W, Em, Pm and Cointime distributions 
// are plotted and the number of good counts are printed out to add to the count of good events.
// Needs runnumber, and if target is "h" or "d"

void check_cointime(Int_t runNumber, Int_t targ=1){

  //read the input file from data
  TString fileNameD = "/net/cdaq/cdaql3data/cdaq/hallc-online/ROOTfiles/" ;
  fileNameD += "coin_replay_production_"; //read the root file from data
  fileNameD += runNumber; //read the root file from data
  fileNameD += "_50000.root"; //read the root file from data
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

  TH1D *h1PcointimeROC1    = new TH1D("SHMS ROC1 Corrected Coin Time","SHMS ROC1 Corrected Coin Time; cointime [ns]",       400, -20, 20);    
  TH1D *h1PcointimeROC2    = new TH1D("SHMS ROC2 Corrected Coin Time","SHMS ROC2 Corrected Coin Time; cointime [ns]",       400, -20, 20); 
  TH1D *h1PcointimeROC2_R  = new TH1D("SHMS ROC2 Corrected Coin Time_R","SHMS ROC2 Corrected Coin Time_R; cointime [ns]",   400, -20, 20); 
  TH1D *h1PcointimeROC2_L  = new TH1D("SHMS ROC2 Corrected Coin Time_L","SHMS ROC2 Corrected Coin Time_L; cointime [ns]",   400, -20, 20); 

  TH1D *h1PcointimeROC2_C  = new TH1D("SHMS ROC2 Corrected Coin Time_C","SHMS ROC2 Corrected Coin Time_C; cointime [ns]",   400, -20, 20); 



  Double_t HgtrX, HgtrTh, HgtrY, HgtrPh, hdelta, PgtrX, PgtrTh, PgtrY, PgtrPh, pdelta;
  Double_t HgtrBetaCalc, PgtrBetaCalc, evtType, PgtrP, HgtrP, PhodStatus, PhodStartTime, PhodfpHitsTime;
  Double_t cointime, HhodStatus, HhodStartTime, HhodfpHitsTime, SHMSpartMass, HMSpartMass, paeronpe;
  Double_t pkinW, pEm, pPm, modPm, pbeta, hbeta, hcalepr, hcaletot, hcernpe, pcaletot, pcalepr, pcernpe;
  Double_t TcoinpTRIG1_ROC1_tdcTimeRaw, TcoinpTRIG4_ROC1_tdcTimeRaw, TcoinpTRIG1_ROC2_tdcTimeRaw;
  Double_t TcoinhTRIG1_ROC1_tdcTimeRaw, TcoinhTRIG1_ROC2_tdcTimeRaw, TcoinhTRIG4_ROC1_tdcTimeRaw;
  Double_t TcoinhTRIG4_ROC2_tdcTimeRaw, TcoinpTRIG4_ROC2_tdcTimeRaw;

  int cnts=0;
  TCut hpdelta;
  TCut evtCut;                                                                   
  TCut eBetaCut;                                                                   
  TCut pBetaCut;                                                                   
  TCut cerCut;                                                                   
  TCut calCut;                                                                   
  TCut hodoTimeCut;
  TCut paeroCut;

  tt->SetBranchAddress("P.gtr.p", &PgtrP); 
  tt->SetBranchAddress("H.gtr.p", &HgtrP); 
  tt->SetBranchAddress("P.gtr.beta", &pbeta);                                                                                 
  tt->SetBranchAddress("H.gtr.beta", &hbeta); 
  tt->SetBranchAddress("H.gtr.dp", &hdelta);                                                                                
  tt->SetBranchAddress("P.gtr.dp", &pdelta);                                                                                
                                                                                                                            
  // tt->SetBranchAddress("P.cal.eprtracknorm", &pcalepr);                                                                    
  // tt->SetBranchAddress("P.cal.etottracknorm", &pcaletot);                                                                  
  tt->SetBranchAddress("P.hgcer.npeSum", &pcernpe);                                                 
  tt->SetBranchAddress("P.aero.npeSum", &paeronpe);                                                 
 
  //    tt->SetBranchAddress("H.cal.eprtracknorm", &hcalepr);                                            
   tt->SetBranchAddress("H.cal.etottracknorm", &hcaletot);                                          
  tt->SetBranchAddress("H.cer.npeSum", &hcernpe); 

                                                                                                              
  //tt->SetBranchAddress("P.hod.goodstarttime", &PhodStatus);                                              
  tt->SetBranchAddress("P.hod.starttime", &PhodStartTime);                                               
  tt->SetBranchAddress("P.hod.fpHitsTime", &PhodfpHitsTime);                                             
                                                                                                             
  //  tt->SetBranchAddress("H.hod.goodstarttime", &HhodStatus);                                              
  tt->SetBranchAddress("H.hod.starttime", &HhodStartTime);                                               
  tt->SetBranchAddress("H.hod.fpHitsTime", &HhodfpHitsTime);                                             
                                                                                                             
                                                                                                             
  tt->SetBranchAddress("T.coin.pTRIG1_ROC1_tdcTimeRaw", &TcoinpTRIG1_ROC1_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG4_ROC1_tdcTimeRaw", &TcoinpTRIG4_ROC1_tdcTimeRaw);
  tt->SetBranchAddress("T.coin.pTRIG1_ROC2_tdcTimeRaw", &TcoinpTRIG1_ROC2_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG4_ROC2_tdcTimeRaw", &TcoinpTRIG4_ROC2_tdcTimeRaw);                   
                                                                                                             
  //  tt->SetBranchAddress("T.coin.hTRIG1_ROC1_tdcTimeRaw", &TcoinhTRIG1_ROC1_tdcTimeRaw);                   
  //tt->SetBranchAddress("T.coin.hTRIG4_ROC1_tdcTimeRaw", &TcoinhTRIG4_ROC1_tdcTimeRaw);                   
  //tt->SetBranchAddress("T.coin.hTRIG1_ROC2_tdcTimeRaw", &TcoinhTRIG1_ROC2_tdcTimeRaw);                   
  // tt->SetBranchAddress("T.coin.hTRIG4_ROC2_tdcTimeRaw", &TcoinhTRIG4_ROC2_tdcTimeRaw);                   

  SHMSpartMass = 0.1395704; // pion mass in GeV/c^2                
  HMSpartMass = 0.000510998; // electron mass in GeV/c^2                 
  hpdelta = "P.gtr.dp > -10 && P.gtr.dp < 20 && H.gtr.dp > -10 && H.gtr.dp < 10";
  eBetaCut = "P.gtr.beta > 0.8 && P.gtr.beta < 1.3";                        
  pBetaCut = "H.gtr.beta > 0.8 &&  H.gtr.beta < 1.3";                       
  cerCut = "P.hgcer.npeSum > 1.0 && H.cer.npeSum > 0.5";                    
  //  calCut = "H.cal.etottracknorm > 0.8 && H.cal.etottracknorm < 1.5 && H.cal.eprtracknorm  > 0.2";         
   calCut = "1==1";         
   // hodoTimeCut ="P.hod.goodstarttime == 1 && H.hod.goodstarttime == 1";      
    hodoTimeCut ="1==1";      
  paeroCut = "P.aero.npeSum > 2.0";

  

  TCanvas *canvas1 = new TCanvas("canvas1","canvas1");                           
  tt->Draw("P.hod.starttime >> SHMShodoStartTime", eBetaCut && pBetaCut && cerCut );  
  TH1D *h1PhodoStartTime = (TH1D*)gDirectory->Get("SHMShodoStartTime");
  h1PhodoStartTime->GetXaxis()->SetTitle("SHMS hodo start time [ns]");           
  Double_t PhodoStartTimeMean = h1PhodoStartTime->GetMean();  



                   
  TCanvas *canvas2 = new TCanvas("canvas2","canvas2");                           
  tt->Draw("H.hod.starttime >> HMShodoStartTime", eBetaCut && pBetaCut && cerCut);  
  TH1D *h1HhodoStartTime = (TH1D*)gDirectory->Get("HMShodoStartTime");           
  h1HhodoStartTime->GetXaxis()->SetTitle("HMS hodo start time [ns]");            
  Double_t HhodoStartTimeMean = h1HhodoStartTime->GetMean();                     

 
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
     
  Bool_t event_cut;
  for (int kk=0; kk<nentriesD; kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
    evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    //  if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.2 && hcernpe>0. && hcaletot >0.6 && hcaletot<2.0 && PhodStatus == 1 && HhodStatus ==1 && hdelta > -10 && hdelta < 10 && pdelta > -15 && pdelta < 15 && pcernpe > 2.0 && paeronpe > 2.0)
    event_cut =  hdelta > -10 && hdelta < 10 && pdelta > -10 && pdelta < 20 ;
    if (hcaletot) event_cut=hcaletot >0.6 && hcaletot<2.0 && hdelta > -10 && hdelta < 10 && pdelta > -10 && pdelta < 20;
    // if ( hcaletot >0.6 && hcaletot<2.0 && hdelta > -10 && hdelta < 10 && pdelta > -10 && pdelta < 20 )
     if (event_cut) 

   
   { 
	DeltaHMSpathLength = 12.462*HgtrTh + 0.1138*HgtrTh*HgtrX - 0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 0.0000544*HgtrX*HgtrX - 116.52*HgtrPh*HgtrPh;               
	PgtrBetaCalc = PgtrP/sqrt(PgtrP*PgtrP + SHMSpartMass*SHMSpartMass);        
	HgtrBetaCalc = HgtrP/sqrt(HgtrP*HgtrP + HMSpartMass*HMSpartMass);          
	SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*PgtrBetaCalc) + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
	HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / (speedOfLight*HgtrBetaCalc) + (HhodoStartTimeMean - HhodfpHitsTime);      
	SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*PgtrBetaCalc) + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
	HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / (speedOfLight*HgtrBetaCalc) + (HhodoStartTimeMean - HhodfpHitsTime);      
	SHMScorrCoinTimeROC1 = (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; // 0.1 to convert to ns 
	SHMScorrCoinTimeROC2 = (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; 
	h1PcointimeROC1->Fill(SHMScorrCoinTimeROC1);    
	h1PcointimeROC2->Fill(SHMScorrCoinTimeROC2);    
	//	if(SHMScorrCoinTimeROC2>0)      
	  h1PcointimeROC2_R->Fill(SHMScorrCoinTimeROC2);
      
	  //	if(SHMScorrCoinTimeROC2>-20 && SHMScorrCoinTimeROC2< -10)      
	  h1PcointimeROC2_L->Fill(SHMScorrCoinTimeROC2);
	  h1PcointimeROC2_C->Fill(SHMScorrCoinTimeROC2);
   }
      
  }

  gROOT->SetBatch(kFALSE);

  TCanvas *can2 = new TCanvas ("can2",Form("can2 Run: %d",runNumber));
  //*******************************************************************************

  int bin_max = h1PcointimeROC2->GetMaximumBin();
  double max_value = h1PcointimeROC2->GetBinCenter(bin_max);

  Int_t bin_low =  h1PcointimeROC2->FindBin(max_value - 2);
  Int_t bin_hi = h1PcointimeROC2->FindBin(max_value + 2);
  h1PcointimeROC2_C->GetXaxis()->SetRange(bin_low,bin_hi);

  Int_t bin1 = h1PcointimeROC2->FindBin(max_value + 3);
  Int_t bin2 = h1PcointimeROC2->FindBin(max_value + 15);                                                           
  h1PcointimeROC2_R->GetXaxis()->SetRange(bin1,bin2);

  Int_t bin3 = h1PcointimeROC2->FindBin(max_value - 3);
  Int_t bin4 = h1PcointimeROC2->FindBin(max_value - 11);
  h1PcointimeROC2_L->GetXaxis()->SetRange(bin4,bin3);

  //  TF1 *fun = new TF1("fun", "gaus", -7.5, -3.5);
  //TF1 *fun = new TF1("fun", "gaus", max_value - 2, max_value + 2);
  //  fun->SetLineColor(4);
  h1PcointimeROC2->SetLineColor(kBlack);
  h1PcointimeROC2_R->SetLineColor(kRed);
  h1PcointimeROC2_L->SetLineColor(kRed);
  h1PcointimeROC2_C->SetLineColor(kBlue);

  h1PcointimeROC2->SetLineWidth(2);
  //  h1PcointimeROC2->Fit("fun", "R");  //->SetLineColor(3);
  //double mean = fun->GetParameter(1);
  //cout<< "The Mean of Main Peak = "<<mean<<endl;
  h1PcointimeROC2->Draw();
  h1PcointimeROC2_R->Draw("same");
  h1PcointimeROC2_L->Draw("same");
  h1PcointimeROC2_C->Draw("same");


  can2->cd(1); 
  can2->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_cointime.pdf",runNumber));
   

  Double_t area1 =h1PcointimeROC2->Integral(bin_low, bin_hi);
  cout<<"The integral of mainpeak = "<<area1<<endl;
  // cout<<bin_low<< "   \t"<<bin_hi<<endl;


  Double_t area2 =h1PcointimeROC2->Integral(bin4, bin3);
  cout<<"The integral of left Red Portion = "<<area2<<endl;
  //  cout<<bin_low<< "   \t"<<bin_hi<<endl;


  Double_t area3 =h1PcointimeROC2->Integral(bin1, bin2);
   cout<<"The integral of right Red Portion = "<<area3<<endl;
  // cout<<bin_low<< "   \t"<<bin_hi<<endl;



  Double_t av_acc = (area2 + area3)/20 ;
  Double_t good_cnts = area1 - av_acc*4;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;
  cout << "NOTE:>>>>  " << good_cnts <<"  <<<<<  GOOD EVENTS have passed all cuts (update the white board and excel sheet)" << endl;
  cout << "-------------------------------------------------------------------------------------------------------" << endl;


  

}
