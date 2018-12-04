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
    Double_t SHMSpartMass; 
    Double_t HMSpartMass;
    Double_t SHMSbetaMin;
    Double_t SHMSbetaMax;
    Double_t HMSbetaMin;
    Double_t HMSbetaMax;

    SHMSpartMass = 139.5704/1000.0; // pion mass in GeV/c^2 
    HMSpartMass = 0.510998/1000.0; // electron mass in GeV/c^2

    TH1D *h1PgtrBetaMes    = new TH1D("P.gtr.beta", "P.gtr.beta; #beta ", 500, 0.5, 1.2);
    TH1D *h1HgtrBetaMes    = new TH1D("H.gtr.beta", "H.gtr.beta; #beta", 500, 0.5, 1.2);
    TH1D *h1PgtrBetaCalc   = new TH1D("SHMSbetaCalc", "SHMSbetaCalc; #beta ", 500, 0.5, 1.2);
    TH1D *h1HgtrBetaCalc   = new TH1D("HMSbetaCalc", "HMSbetaCalc; #beta", 500, 0.5, 1.2);
    TH1D *h1PhodfpHitsTime = new TH1D("P.hod.fpHitsTime", "P.hod.fpHitsTime; SHMS fp Hits Time [ns]", 500, 0, 40);
    TH1D *h1HhodfpHitsTime = new TH1D("H.hod.fpHitsTime", "H.hod.fpHitsTime; HMS fp Hits Time [ns]", 500, 0, 40);

    //----------------------- Histogram for coincidence time ----------------------------
    TH1D *h1PcointimeROC1 = new TH1D("SHMS ROC1 Corrected Coin Time","SHMS ROC1 Corrected Coin Time; cointime [ns]", 400, -20, 20);
    TH1D *h1PcointimeROC2 = new TH1D("SHMS ROC2 Corrected Coin Time","SHMS ROC2 Corrected Coin Time; cointime [ns]", 400, -20, 20);
    TH1D *h1HcointimeROC1 = new TH1D("HMS ROC1 Corrected Coin Time","HMS ROC1 Corrected Coin Time; cointime [ns]", 400, -60, 60);
    TH1D *h1HcointimeROC2 = new TH1D("HMS ROC2 Corrected Coin Time","HMS ROC2 Corrected Coin Time; cointime [ns]", 400, -100, 150);

    // Momentum and beta
    Double_t PgtrP;
    Double_t HgtrP;
    Double_t PgtrBetaMes;    
    Double_t HgtrBetaMes;        
    Double_t PgtrBetaCalc;    
    Double_t HgtrBetaCalc;        
    
    //Detector Response
    Double_t PcalEprTrackNorm;
    Double_t PcalEtotTrackNorm;
    Double_t PhgcerNpeSum;
    Double_t paeronpe;

    Double_t HcalEprTrackNorm;
    Double_t HcalEtotTrackNorm;
    Double_t HcerNpeSum;

    //Hodoscope start time 
    Double_t HhodStartTime;
    Double_t PhodStartTime;
    Double_t PhodStatus;
    Double_t HhodStatus;
   
    Double_t PhodfpHitsTime;
    Double_t HhodfpHitsTime;

    // Foal plane coords
    Double_t PdcXfp;
    Double_t PdcXpfp;
    Double_t PdcYfp;
    Double_t PdcYpfp;

    Double_t HdcXfp;
    Double_t HdcXpfp;
    Double_t HdcYfp;
    Double_t HdcYpfp;
 
    //Vertex coord
    Double_t PgtrdP;
    Double_t PgtrX;
    Double_t PgtrTh;
    Double_t PgtrY;
    Double_t PgtrPh;
   
    Double_t HgtrdP;
    Double_t HgtrX;
    Double_t HgtrTh;
    Double_t HgtrY;
    Double_t HgtrPh;
    
    //Trigger 
    Double_t TcoinpTRIG1_ROC1_tdcTimeRaw;
    Double_t TcoinpTRIG4_ROC1_tdcTimeRaw;
    Double_t TcoinpTRIG1_ROC2_tdcTimeRaw;
    Double_t TcoinpTRIG4_ROC2_tdcTimeRaw;
    
    Double_t TcoinhTRIG1_ROC1_tdcTimeRaw;
    Double_t TcoinhTRIG4_ROC1_tdcTimeRaw;
    Double_t TcoinhTRIG1_ROC2_tdcTimeRaw;
    Double_t TcoinhTRIG4_ROC2_tdcTimeRaw;
        
    //Int_t EvtType;
    Double_t EvtType;  // GetLeaf()->GetValue() returns double only
    
    td->SetBranchAddress("P.gtr.p", &PgtrP);            
    td->SetBranchAddress("H.gtr.p", &HgtrP);            
    td->SetBranchAddress("P.gtr.dp", &PgtrdP);            
    td->SetBranchAddress("H.gtr.dp", &HgtrdP);            
    td->SetBranchAddress("P.gtr.beta", &PgtrBetaMes);        
    td->SetBranchAddress("H.gtr.beta", &HgtrBetaMes);

    td->SetBranchAddress("P.cal.eprtracknorm", &PcalEprTrackNorm);
    td->SetBranchAddress("P.cal.etottracknorm", &PcalEtotTrackNorm);
    td->SetBranchAddress("P.hgcer.npeSum", &PhgcerNpeSum);
    td->SetBranchAddress("P.aero.npeSum", &paeronpe);
    
    td->SetBranchAddress("H.cal.eprtracknorm", &HcalEprTrackNorm);
    td->SetBranchAddress("H.cal.etottracknorm", &HcalEtotTrackNorm);
    td->SetBranchAddress("H.cer.npeSum", &HcerNpeSum);

    td->SetBranchAddress("P.hod.goodstarttime", &PhodStatus);
    td->SetBranchAddress("P.hod.starttime", &PhodStartTime);
    td->SetBranchAddress("P.hod.fpHitsTime", &PhodfpHitsTime);
    
    td->SetBranchAddress("H.hod.goodstarttime", &HhodStatus);    
    td->SetBranchAddress("H.hod.starttime", &HhodStartTime);    
    td->SetBranchAddress("H.hod.fpHitsTime", &HhodfpHitsTime);

    td->SetBranchAddress("P.gtr.x", &PgtrX);
    td->SetBranchAddress("P.gtr.th", &PgtrTh);
    td->SetBranchAddress("P.gtr.y", &PgtrY);
    td->SetBranchAddress("P.gtr.ph", &PgtrPh);
    
    td->SetBranchAddress("H.gtr.x", &HgtrX);
    td->SetBranchAddress("H.gtr.th", &HgtrTh);
    td->SetBranchAddress("H.gtr.y", &HgtrY);
    td->SetBranchAddress("H.gtr.ph", &HgtrPh);
    
    td->SetBranchAddress("H.dc.x_fp", &HdcXfp);
    td->SetBranchAddress("H.dc.xp_fp", &HdcXpfp);
    td->SetBranchAddress("H.dc.y_fp", &HdcYfp);
    td->SetBranchAddress("H.dc.yp_fp", &HdcYpfp);    

    td->SetBranchAddress("P.dc.x_fp", &PdcXfp);
    td->SetBranchAddress("P.dc.xp_fp", &PdcXpfp);
    td->SetBranchAddress("P.dc.y_fp", &PdcYfp);
    td->SetBranchAddress("P.dc.yp_fp", &PdcYpfp);    

    td->SetBranchAddress("T.coin.pTRIG1_ROC1_tdcTimeRaw", &TcoinpTRIG1_ROC1_tdcTimeRaw);
    td->SetBranchAddress("T.coin.pTRIG4_ROC1_tdcTimeRaw", &TcoinpTRIG4_ROC1_tdcTimeRaw);
    td->SetBranchAddress("T.coin.pTRIG1_ROC2_tdcTimeRaw", &TcoinpTRIG1_ROC2_tdcTimeRaw);
    td->SetBranchAddress("T.coin.pTRIG4_ROC2_tdcTimeRaw", &TcoinpTRIG4_ROC2_tdcTimeRaw);

    td->SetBranchAddress("T.coin.hTRIG1_ROC1_tdcTimeRaw", &TcoinhTRIG1_ROC1_tdcTimeRaw);
    td->SetBranchAddress("T.coin.hTRIG4_ROC1_tdcTimeRaw", &TcoinhTRIG4_ROC1_tdcTimeRaw);
    td->SetBranchAddress("T.coin.hTRIG1_ROC2_tdcTimeRaw", &TcoinhTRIG1_ROC2_tdcTimeRaw);
    td->SetBranchAddress("T.coin.hTRIG4_ROC2_tdcTimeRaw", &TcoinhTRIG4_ROC2_tdcTimeRaw);


    Int_t nEvents = td->GetEntries();
    cout << "Number of events: "<< nEvents <<endl;

    TCut evtCut;
    TCut eBetaCut;
    TCut pBetaCut;
    TCut cerCut; 
    TCut calCut; 
    TCut hodoTimeCut;
    TCut aeroCut;

	 eBetaCut = "P.gtr.beta > 0.5 && P.gtr.beta < 1.5";
	 pBetaCut = "H.gtr.beta > 0.5 &&  H.gtr.beta < 1.5";
	 cerCut = "P.hgcer.npeSum > 2.0 && H.cer.npeSum > 0.1";
	 calCut = "H.cal.etottracknorm > 0.8 && H.cal.etottracknorm < 1.5 && H.cal.eprtracknorm  > 0.2";
	 hodoTimeCut ="P.hod.goodstarttime == 1 && H.hod.goodstarttime == 1";	 
         aeroCut = "P.aero.npeSum > 2.0";
    
    TCanvas *canvas1 = new TCanvas("canvas1","canvas1");
    td->Draw("P.hod.starttime >> SHMShodoStartTime",  eBetaCut && pBetaCut && cerCut && calCut && hodoTimeCut);
    TCanvas *canvas2 = new TCanvas("canvas2","canvas2");
    td->Draw("H.hod.starttime >> HMShodoStartTime",  eBetaCut && pBetaCut && cerCut && calCut && hodoTimeCut);

    TH1D *h1PhodoStartTime = (TH1D*)gDirectory->Get("SHMShodoStartTime");
    TH1D *h1HhodoStartTime = (TH1D*)gDirectory->Get("HMShodoStartTime");
    h1PhodoStartTime->GetXaxis()->SetTitle("SHMS hodo start time [ns]");
    h1HhodoStartTime->GetXaxis()->SetTitle("HMS hodo start time [ns]");

    Double_t pOffset = 1.5;  // in ns
    Double_t hOffset = 335;
    Double_t speedOfLight = 29.9792; // in cm/ns
    
    Double_t SHMScentralPathLen = 18.1*100;  // SHMS Target to focal plane path length converted to cm
    Double_t SHMSpathLength = 18.1*100;      // For now assume that it's same as SHMScentralPathLen
    Double_t HMScentralPathLen = 22*100;     // HMS Target to focal plane path length converted to cm
    Double_t DeltaHMSpathLength;    
    Double_t DeltaSHMSpathLength;    

    Double_t SHMScoinCorr = 0.0;
    Double_t HMScoinCorr = 0.0;
    Double_t PhodoStartTimeMean = h1PhodoStartTime->GetMean();
    Double_t HhodoStartTimeMean =  h1HhodoStartTime->GetMean();
    Int_t totEvents = 0;

    Double_t SHMSrawCoinTimeROC1;
    Double_t SHMSrawCoinTimeROC2;
    Double_t HMSrawCoinTimeROC1;
    Double_t HMSrawCoinTimeROC2;

    Double_t SHMScorrCoinTimeROC1;
    Double_t SHMScorrCoinTimeROC2;
    Double_t HMScorrCoinTimeROC1;
    Double_t HMScorrCoinTimeROC2;
        

    for (Int_t event = 0; event < nEvents; ++event)
    {
	if(event%10000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	td->GetEntry(event);
	
        //----- PID Cut -------
	    if( !(PgtrBetaMes > 0.6 && PgtrBetaMes < 1.4                       // proton beta cut
		  && HgtrBetaMes > 0.8 &&  HgtrBetaMes < 1.2                   // e beta cut
		  && HcerNpeSum > 0.1 && PhgcerNpeSum > 2.0       // Cerenkov counter cut for e- and pion
                  && paeronpe > 2.0
		  && HcalEtotTrackNorm > 0.8 && HcalEtotTrackNorm < 1.5       // HMS Calorimeter cut 
		  && HcalEprTrackNorm > 0.2) 	                               // HMS calorimeter cut       
		)
		continue;
	

	if(PhodStatus == 0 || HhodStatus ==0)
	    continue;


        d_hdelta->Fill(HgtrdP);
        d_hxptar->Fill(HgtrTh);
        d_hyptar->Fill(HgtrPh);
        d_hytar->Fill(HgtrY);
        d_pdelta->Fill(PgtrdP);
        d_pxptar->Fill(PgtrTh);
        d_pyptar->Fill(PgtrPh);
        d_pytar->Fill(PgtrY);


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

	++totEvents;

    }
    cout << "Total number of events passing the PID cuts (looping over events): "<< totEvents <<endl;

	//end of cointime


  gROOT->SetBatch(kFALSE);

  TCanvas *c1 = new TCanvas ("cc1","cc1",800,600);
  c1->SetGrid();
  c1->SetLeftMargin(0.2);
  c1->SetRightMargin(0.2);
  gStyle->SetOptStat(0);

  c1->Divide(4,2);

  c1->cd(1);
  d_hdelta->SetLineColor(kRed);
  d_hdelta->DrawNormalized("p e1"); 
  s_hdelta->SetLineColor(kBlue); 
  s_hdelta->DrawNormalized("same"); 
  c1->cd(2);
  d_hxptar->SetLineColor(kRed);
  d_hxptar->DrawNormalized("p e1"); 
  s_hxptar->SetLineColor(kBlue);
  s_hxptar->DrawNormalized("same"); 
  c1->cd(3);
  d_hyptar->SetLineColor(kRed);
  d_hyptar->DrawNormalized("p e1");
  s_hyptar->SetLineColor(kBlue);
  s_hyptar->DrawNormalized("same");  
  c1->cd(4);
  d_hytar->SetLineColor(kRed);
  d_hytar->DrawNormalized("p e1");
  s_hytar->SetLineColor(kBlue);
  s_hytar->DrawNormalized("same");  

  c1->cd(5);
  d_pdelta->SetLineColor(kRed);
  d_pdelta->DrawNormalized("p e1");
  s_pdelta->SetLineColor(kBlue);
  s_pdelta->DrawNormalized("same"); 
  c1->cd(6);
  d_pxptar->SetLineColor(kRed);
  d_pxptar->DrawNormalized("p e1");
  s_pxptar->SetLineColor(kBlue);
  s_pxptar->DrawNormalized("same");
  c1->cd(7);
  d_pyptar->SetLineColor(kRed);
  d_pyptar->DrawNormalized("p e1");
  s_pyptar->SetLineColor(kBlue);
  s_pyptar->DrawNormalized("same"); 
  c1->cd(8);
  d_pytar->SetLineColor(kRed);
  d_pytar->DrawNormalized("p e1");
  s_pytar->SetLineColor(kBlue);
  s_pytar->DrawNormalized("same"); 

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
