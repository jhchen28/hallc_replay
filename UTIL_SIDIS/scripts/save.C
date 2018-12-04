// Filename: SIDIS_spec_compare.C
// Description: Plot delta, xptar, yptar and ytar distributions for SHMS and HMS from both SIMC and data for comparison;
// Plot coincidence time and focal plane hit time calculations 

// Needs input of run number, SIMC root filename and  normfac from SIMC

void SIDIS_spec_compare(int runno, Int_t targ=1, Int_t pisign=1, Int_t kingrp=1, Int_t ptgrp=1){

  TString fileNameM;
  fileNameM = "/net/cdaq/cdaql3data/cdaq/hallc-online/SIMC_ROOTfiles/";
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


  //  TString ofilename = "_hists_comp.root";
  //TFile *fout = new TFile(ofilename,"RECREATE");


  gROOT->SetBatch(kTRUE);
  gROOT->ForceStyle(); 
  //gStyle->SetPalette(kOcean);
  
  //make histograms:
  TH1D *s_hdelta = new TH1D("s_hdelta","HMS DELTA (%)",100,-15,15);
  TH1D *s_hxptar = new TH1D("s_hxptar","HMS XPTAR (rad)",100,-0.15,0.15);
  TH1D *s_hyptar = new TH1D("s_hyptar","HMS YPTAR (rad)",100,-0.1,0.1);
  TH1D *s_hytar = new TH1D("s_hytar","HMS YTAR (cm)",100,-5.0,5.0);
  TH1D *s_pdelta = new TH1D("s_pdelta","SHMS DELTA (%)",100,-25,30);
  TH1D *s_pxptar = new TH1D("s_sxptar","SHMS XPTAR (rad)",100,-0.15,0.15);
  TH1D *s_pyptar = new TH1D("s_syptar","SHMS YPTAR (rad)",100,-0.15,0.15);
  TH1D *s_pytar = new TH1D("s_sytar","SHMS YTAR (cm)",100,-5,5);

 

  double shxptar, shyptar, spxptar, spyptar, wt, sdeltap, sdeltae; 
  double normwt, shytar, spytar;

  for (int kk=0; kk<nentriesM; kk++){
    ts->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesM << "   % of simc done 2nd time" << endl;

    sdeltap = ts->GetLeaf("hsdelta")->GetValue();
    shxptar = ts->GetLeaf("hsxptar")->GetValue();
    shyptar = ts->GetLeaf("hsyptar")->GetValue();
    shytar = ts->GetLeaf("hsytar")->GetValue();
    sdeltae = ts->GetLeaf("ssdelta")->GetValue();
    spxptar = ts->GetLeaf("ssxptar")->GetValue();
    spyptar = ts->GetLeaf("ssyptar")->GetValue();
    spytar = ts->GetLeaf("ssytar")->GetValue();
    wt = ts->GetLeaf("Weight")->GetValue();
    normwt = wt;

    s_hdelta->Fill(sdeltap,normwt);
    s_hxptar->Fill(shxptar,normwt);
    s_hyptar->Fill(shyptar,normwt);
    s_hytar->Fill(shytar,normwt);
    s_pdelta->Fill(sdeltae,normwt);
    s_pxptar->Fill(spxptar,normwt);
    s_pyptar->Fill(spyptar,normwt);
    s_pytar->Fill(spytar,normwt);
  }
  
  // int runno = 2050; //eventually need to change into input
  TString fileNameD;
  fileNameD += "/net/cdaq/cdaql3data/cdaq/hallc-online/ROOTfiles/coin_replay_production_";
  fileNameD += runno;
  fileNameD += "_-1.root";
  TFile *f1 = new TFile(fileNameD);
  TTree *td = (TTree*)f1->Get("T");
  int nentriesD = td->GetEntries();
  
  TH1D *d_hdelta = new TH1D("d_hdelta","HMS DELTA (%)",100,-15,15);
  TH1D *d_hxptar = new TH1D("d_hxptar","HMS XPTAR (rad)",100,-0.15,0.15);
  TH1D *d_hyptar = new TH1D("d_hyptar","HMS YPTAR (rad)",100,-0.1,0.1);
  TH1D *d_hytar = new TH1D("d_hytar","HMS YTAR (cm)",100,-5.0,5.0);
  TH1D *d_pdelta = new TH1D("d_pdelta","SHMS DELTA (%)",100,-25,30);
  TH1D *d_pxptar = new TH1D("d_sxptar","SHMS XPTAR (rad)",100,-0.15,0.15);
  TH1D *d_pyptar = new TH1D("d_syptar","SHMS YPTAR (rad)",100,-0.15,0.15);
  TH1D *d_pytar = new TH1D("d_sytar","SHMS YTAR (cm)",100,-5,5);
  
  double ddeltap, dhxptar, dhyptar, dhytar; //HMS quantities
  double ddeltae, dpxptar, dpyptar, dpytar; //SHMS quantities
  
  //cointime addition:
    Double_t SHMScorrCoinTimeROC1;
    Double_t SHMScorrCoinTimeROC2;
    Double_t HMScorrCoinTimeROC1;
    Double_t HMScorrCoinTimeROC2;
    
    Double_t TcoinpTRIG1_ROC1_tdcTimeRaw;
    Double_t TcoinpTRIG4_ROC1_tdcTimeRaw;
    Double_t TcoinpTRIG1_ROC2_tdcTimeRaw;
    Double_t TcoinpTRIG4_ROC2_tdcTimeRaw;
    
    Double_t TcoinhTRIG1_ROC1_tdcTimeRaw;
    Double_t TcoinhTRIG4_ROC1_tdcTimeRaw;
    Double_t TcoinhTRIG1_ROC2_tdcTimeRaw;
    Double_t TcoinhTRIG4_ROC2_tdcTimeRaw;
    
    Double_t SHMScentralPathLen = 18.1*100;  // SHMS Target to focal plane path length converted to cm
    Double_t SHMSpathLength = 18.1*100;      // For now assume that it's same as SHMScentralPathLen
    Double_t HMScentralPathLen = 22*100;     // HMS Target to focal plane path length converted to cm
    Double_t DeltaHMSpathLength;    
    Double_t DeltaSHMSpathLength; 
    
    Double_t PgtrP;
    Double_t HgtrP;
    Double_t SHMSpartMass = 139.5701835/1000.0; // pion mass in GeV/c^2 //do these need to be changed?
    Double_t HMSpartMass = 0.510998/1000.0; // electron mass in GeV/c^2
    
    Double_t pOffset = 1.5;  // in ns
    Double_t hOffset = 335;
    Double_t speedOfLight = 29.9792; // in cm/ns
    
    Double_t HdcXfp;
    Double_t HdcXpfp;
    Double_t HdcYfp;
    Double_t HdcYpfp;
    
    Double_t PgtrTh;
    Double_t PgtrdP; //Latif didn't assign this to "P.gtr.dp" branch. I did; not sure if I should have.
    Double_t PhodfpHitsTime;
    Double_t HhodfpHitsTime;
    
    Double_t PgtrBetaCalc;
    Double_t HgtrBetaCalc;
    Double_t SHMScoinCorr;
    Double_t HMScoinCorr;

    TCut evtCut;
    TCut eBetaCut;
    TCut pBetaCut;
    TCut cerCut; 
    TCut calCut; 
    TCut hodoTimeCut;
    TCut hpdelta;

    hpdelta = "P.gtr.dp > -10 && P.gtr.dp < 10 && H.gtr.dp > -8 && H.gtr.dp < 8";
    eBetaCut = "P.gtr.beta > 0.8 && P.gtr.beta < 1.3";
    pBetaCut = "H.gtr.beta > 0.8 &&  H.gtr.beta < 1.3";
    cerCut = "P.hgcer.npeSum > 0.5 && H.cer.npeSum > 0.1";
    calCut = "H.cal.etottracknorm > 0.8 && H.cal.etottracknorm < 1.5 && H.cal.eprtracknorm  > 0.2";
    hodoTimeCut ="P.hod.goodstarttime == 1 && H.hod.goodstarttime == 1";	    
    TCanvas *canvas1 = new TCanvas("canvas1","canvas1");
    td->Draw("P.hod.starttime >> SHMShodoStartTime",  eBetaCut && pBetaCut && cerCut && calCut && hodoTimeCut);
    TCanvas *canvas2 = new TCanvas("canvas2","canvas2");
    td->Draw("H.hod.starttime >> HMShodoStartTime",  eBetaCut && pBetaCut && cerCut && calCut && hodoTimeCut);

    TH1D *h1PhodoStartTime = (TH1D*)gDirectory->Get("SHMShodoStartTime");
    TH1D *h1HhodoStartTime = (TH1D*)gDirectory->Get("HMShodoStartTime");
    Double_t PhodoStartTimeMean = h1PhodoStartTime->GetMean();
    Double_t HhodoStartTimeMean =  h1HhodoStartTime->GetMean();

    TH1D *h1PcointimeROC1 = new TH1D("SHMS ROC1 Corrected Coin Time","SHMS ROC1 Corrected Coin Time; cointime [ns]", 400, -20, 20);
    TH1D *h1PcointimeROC2 = new TH1D("SHMS ROC2 Corrected Coin Time","SHMS ROC2 Corrected Coin Time; cointime [ns]", 400, -20, 20);
    TH1D *h1HcointimeROC1 = new TH1D("HMS ROC1 Corrected Coin Time","HMS ROC1 Corrected Coin Time; cointime [ns]", 400, -60, 60);
    TH1D *h1HcointimeROC2 = new TH1D("HMS ROC2 Corrected Coin Time","HMS ROC2 Corrected Coin Time; cointime [ns]", 400, -100, 150);

    td->SetBranchAddress("P.gtr.p", &PgtrP);            
    td->SetBranchAddress("H.gtr.p", &HgtrP); 
	
	td->SetBranchAddress("T.coin.pTRIG1_ROC1_tdcTimeRaw", &TcoinpTRIG1_ROC1_tdcTimeRaw);
    td->SetBranchAddress("T.coin.pTRIG4_ROC1_tdcTimeRaw", &TcoinpTRIG4_ROC1_tdcTimeRaw);
    td->SetBranchAddress("T.coin.pTRIG1_ROC2_tdcTimeRaw", &TcoinpTRIG1_ROC2_tdcTimeRaw);
    td->SetBranchAddress("T.coin.pTRIG4_ROC2_tdcTimeRaw", &TcoinpTRIG4_ROC2_tdcTimeRaw);

    td->SetBranchAddress("T.coin.hTRIG1_ROC1_tdcTimeRaw", &TcoinhTRIG1_ROC1_tdcTimeRaw);
    td->SetBranchAddress("T.coin.hTRIG4_ROC1_tdcTimeRaw", &TcoinhTRIG4_ROC1_tdcTimeRaw);
    td->SetBranchAddress("T.coin.hTRIG1_ROC2_tdcTimeRaw", &TcoinhTRIG1_ROC2_tdcTimeRaw);
    td->SetBranchAddress("T.coin.hTRIG4_ROC2_tdcTimeRaw", &TcoinhTRIG4_ROC2_tdcTimeRaw);
    
    td->SetBranchAddress("H.dc.x_fp", &HdcXfp);
    td->SetBranchAddress("H.dc.xp_fp", &HdcXpfp);
    td->SetBranchAddress("H.dc.y_fp", &HdcYfp);
    td->SetBranchAddress("H.dc.yp_fp", &HdcYpfp);
    
    td->SetBranchAddress("P.gtr.th", &PgtrTh);
    td->SetBranchAddress("P.gtr.dp", &PgtrdP);
    
    td->SetBranchAddress("P.hod.fpHitsTime", &PhodfpHitsTime);
    td->SetBranchAddress("H.hod.fpHitsTime", &HhodfpHitsTime);
  //end of cointime addition
  
  for (int kk=0; kk<nentriesD; kk++){
    td->GetEntry(kk);
    //if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of simc done 2nd time" << endl;

 //PID & kinematic cuts	
 if( !(td->GetLeaf("P.gtr.beta")->GetValue() > 0.6 && td->GetLeaf("P.gtr.beta")->GetValue() < 1.4 //pion beta cut
 	&& td->GetLeaf("H.gtr.beta")->GetValue() > 0.8 && td->GetLeaf("H.gtr.beta")->GetValue() < 1.2 //e beta cut
 	&& td->GetLeaf("H.cer.npeSum")->GetValue() > 0. && td->GetLeaf("P.hgcer.npeSum")->GetValue() > 0.5) //Cerenkov counter cut for c- and pion
 	&& td->GetLeaf("H.cal.etottracknorm")->GetValue() > 0.6 && td->GetLeaf("H.cal.etottracknorm")->GetValue() < 2.0 //HMS Calorimeter cut
     && td->GetLeaf("H.cal.eprtracknorm")->GetValue() > 0.2 // pre shower cut
 	&& td->GetLeaf("H.hod.goodstarttime")->GetValue() == 1 //good hms start time
 	&& td->GetLeaf("P.hod.goodstarttime")->GetValue() == 1 //good shms start time
 	&& td->GetLeaf("H.gtr.dp")->GetValue() > -10 && td->GetLeaf("H.gtr.dp")->GetValue() > 10 //e Delta cut
 	&& td->GetLeaf("P.gtr.dp")->GetValue() > -12 && td->GetLeaf("P.gtr.dp")->GetValue() > 12) //p Delta cut
 	continue;

//target cuts???

    ddeltap = td->GetLeaf("H.gtr.dp")->GetValue();
    dhxptar = td->GetLeaf("H.gtr.th")->GetValue();
    dhyptar = td->GetLeaf("H.gtr.ph")->GetValue();
    dhytar = td->GetLeaf("H.gtr.y")->GetValue();
    ddeltae = td->GetLeaf("P.gtr.dp")->GetValue();
    dpxptar = td->GetLeaf("P.gtr.th")->GetValue();
    dpyptar = td->GetLeaf("P.gtr.ph")->GetValue();
    dpytar = td->GetLeaf("P.gtr.y")->GetValue();

    d_hdelta->Fill(ddeltap);
    d_hxptar->Fill(dhxptar);
    d_hyptar->Fill(dhyptar);
    d_hytar->Fill(dhytar);
    d_pdelta->Fill(ddeltae);
    d_pxptar->Fill(dpxptar);
    d_pyptar->Fill(dpyptar);
    d_pytar->Fill(dpytar);
    
    //added for cointime
    DeltaHMSpathLength = 12.462*HdcXpfp + 0.1138*HdcXpfp*HdcXfp - 0.0154*HdcXfp - 72.292*HdcXpfp*HdcXpfp - 0.0000544*HdcXfp*HdcXfp - 116.52*HdcYpfp*HdcYpfp;
	DeltaSHMSpathLength = -0.11*atan2(PgtrTh,1)*1000 - 0.057*PgtrdP;  // See elog entry: 3517525
	
	PgtrBetaCalc = PgtrP/sqrt(PgtrP*PgtrP + SHMSpartMass*SHMSpartMass);
	HgtrBetaCalc = HgtrP/sqrt(HgtrP*HgtrP + HMSpartMass*HMSpartMass);

	if(PgtrBetaCalc == 0 || HgtrBetaCalc == 0)
	    continue;

	SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + DeltaSHMSpathLength / (speedOfLight*PgtrBetaCalc) + (PhodoStartTimeMean - PhodfpHitsTime); 
	HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / (speedOfLight*HgtrBetaCalc) + (HhodoStartTimeMean - HhodfpHitsTime); 

	SHMScorrCoinTimeROC1 = (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; // 0.1 to convert to ns
	SHMScorrCoinTimeROC2 = (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; 
	HMScorrCoinTimeROC1 = (TcoinhTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinhTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - hOffset;
	HMScorrCoinTimeROC2 = (TcoinhTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinhTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - hOffset;

	h1PcointimeROC1->Fill(SHMScorrCoinTimeROC1); 
	h1PcointimeROC2->Fill(SHMScorrCoinTimeROC2);
	h1HcointimeROC1->Fill(HMScorrCoinTimeROC1);
	h1HcointimeROC2->Fill(HMScorrCoinTimeROC2);
	

	//end of cointime

  }
  gROOT->SetBatch(kFALSE);

  TCanvas *c1 = new TCanvas ("cc1","cc1",800,600);
  c1->SetGrid();
  c1->SetLeftMargin(0.2);
  c1->SetRightMargin(0.2);
  gStyle->SetOptStat(0);

  c1->Divide(4,2);

  c1->cd(1);
  s_hdelta->SetLineColor(kBlue); 
  s_hdelta->DrawNormalized(); 
  d_hdelta->SetLineColor(kRed);
  d_hdelta->DrawNormalized("p e1 same"); 
  c1->cd(2);
  s_hxptar->SetLineColor(kBlue);
  s_hxptar->DrawNormalized(); 
  d_hxptar->SetLineColor(kRed);
  d_hxptar->DrawNormalized("p e1 same"); 
  c1->cd(3);
  s_hyptar->SetLineColor(kBlue);
  s_hyptar->DrawNormalized();  
  d_hyptar->SetLineColor(kRed);
  d_hyptar->DrawNormalized("p e1 same");
  c1->cd(4);
  s_hytar->SetLineColor(kBlue);
  s_hytar->DrawNormalized();  
  d_hytar->SetLineColor(kRed);
  d_hytar->DrawNormalized("p e1 same");

  c1->cd(5);
  s_pdelta->SetLineColor(kBlue);
  s_pdelta->DrawNormalized(); 
  d_pdelta->SetLineColor(kRed);
  d_pdelta->DrawNormalized("p e1 same");
  c1->cd(6);
  s_pxptar->SetLineColor(kBlue);
  s_pxptar->DrawNormalized();
  d_pxptar->SetLineColor(kRed);
  d_pxptar->DrawNormalized("p e1 same");
  c1->cd(7);
  s_pyptar->SetLineColor(kBlue);
  s_pyptar->DrawNormalized(); 
  d_pyptar->SetLineColor(kRed);
  d_pyptar->DrawNormalized("p e1 same");
  c1->cd(8);
  s_pytar->SetLineColor(kBlue);
  s_pytar->DrawNormalized(); 
  d_pytar->SetLineColor(kRed);
  d_pytar->DrawNormalized("p e1 same");

  c1->SaveAs("UTIL_SIDIS/PLOTS/sidis_spec_comp.pdf");

//coincidence time:
	TCanvas *c2 = new TCanvas("c2","Coincidence Time");
	c2->Divide(2,1);
	c2->cd(1);
	h1PcointimeROC1->Draw();
	c2->cd(2);
	h1PcointimeROC2->Draw();
	c2->SaveAs("UTIL_SIDIS/PLOTS/cointime.pdf");

	//  fout->Write();
	//fout->Close();

}
