// Filename: SIDIScoinTime.cc
// Description: Calculate coincidence time
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:03:26 2018 (-0500)
// URL: jlab.org/~latif

void SIDISpid(Int_t runNumber=3400){

    TString ROOT_FILE_PATH = "/net/cdaq/cdaql3data/cdaq/hallc-online/ROOTfiles/";

    TString fileName = ROOT_FILE_PATH;
    fileName += "coin_replay_production_";
    fileName += runNumber;
    fileName += "_-1.root";

    Bool_t runNotFound = gSystem->AccessPathName(fileName);
    if(runNotFound)
    {	    
	cout << "Requested file " << fileName << " does NOT exist" <<endl;
	return;
    }
    
    TFile *file = new TFile(fileName);
    TTree *tree = (TTree*)file->Get("T");    
    //tree->Print();
    
    Double_t SHMSpartMass; 
    Double_t HMSpartMass;
    Double_t SHMSbetaMin;
    Double_t SHMSbetaMax;
    Double_t HMSbetaMin;
    Double_t HMSbetaMax;
    
    
    SHMSpartMass = 139.5/1000.0; // pion mass in GeV/c^2 
    HMSpartMass = 0.510998/1000.0; // electron mass in GeV/c^2
    cout << "------------> Set SHMS for pion and HMS for e <--------------" <<endl;

    
    TH1D *h1PgtrBetaMes    = new TH1D("P.gtr.beta", "P.gtr.beta; #beta ", 500, 0.5, 1.2);
    TH1D *h1HgtrBetaMes    = new TH1D("H.gtr.beta", "H.gtr.beta; #beta", 500, 0., 1.2);
    TH1D *h1PgtrBetaCalc   = new TH1D("SHMSbetaCalc", "SHMSbetaCalc; #beta ", 100, 0.9, 1.1);
    TH1D *h1HgtrBetaCalc   = new TH1D("HMSbetaCalc", "HMSbetaCalc; #beta", 500, 0.5, 1.2);
    TH1D *h1Paeronpe       = new TH1D("SHMSaeronpe", "SHMSaeronpe; #npe", 500, 0., 15);
    TH1D *h1hgcernpe       = new TH1D("P.hgcer.npeSum", "P.hgcer.npeSum; #npeSum ", 500, 0., 30);
    TH1D *h1caletot        = new TH1D("P.cal.etottracknorm", "P.cal.etottracknorm; #etot ", 500, 0., 2.);
    TH1D *h1calepr         = new TH1D("P.cal.eprtracknorm", "P.cal.eprtracknorm; #epr ", 500, 0., 2.);
    TH2D *h2ctimevsepr     = new TH2D("h2ctimevsepr","SHMS Ctime vs Epr; Ctime; Epr", 400, -24., 24, 100,0.,2);
    TH2D *h2ctimevsbeta    = new TH2D("h2ctimevsbeta","SHMS Ctime vs beta; ctime; beta", 400, -24, 24, 100,0.75,1.25);
    TH2D *h2calevsbeta    = new TH2D("h2calevsbeta","SHMS Cal Etot vs beta; cale; beta", 100, 0., 2.0, 100,0.75,1.25);   
                                                                                                                         

    TH1D *h1PhodfpHitsTime = new TH1D("P.hod.fpHitsTime", "P.hod.fpHitsTime; SHMS fp Hits Time [ns]", 500, 0, 40);
    TH1D *h1HhodfpHitsTime = new TH1D("H.hod.fpHitsTime", "H.hod.fpHitsTime; HMS fp Hits Time [ns]", 500, 0, 40);

    //----------------------- Histogram for pion coincidence time ----------------------------
    TH1D *h1PcointimeROC1 = new TH1D("SHMS ROC1 Corrected Coin Time","SHMS ROC1 Corrected Coin Time; cointime [ns]", 400, -20, 20);
    TH1D *h1PcointimeROC2 = new TH1D("SHMS ROC2 Corrected Coin Time","SHMS ROC2 Corrected Coin Time; cointime [ns]", 400, -20, 20);
    TH1D *h1HcointimeROC1 = new TH1D("HMS ROC1 Corrected Coin Time","HMS ROC1 Corrected Coin Time; cointime [ns]", 400, -60, 60);
    TH1D *h1HcointimeROC2 = new TH1D("HMS ROC2 Corrected Coin Time","HMS ROC2 Corrected Coin Time; cointime [ns]", 400, -100, 150);


    // Momentum and beta
    Double_t PgtrP;
    Double_t PgtrdP;
    Double_t HgtrP;
    Double_t PgtrBetaMes;    
    Double_t HgtrBetaMes;        
    Double_t PgtrBetaCalc;    
    Double_t HgtrBetaCalc;        
    
    //Detector Response
    Double_t PcalEprTrackNorm;
    Double_t PcalEtotTrackNorm;
    Double_t PhgcerNpeSum;
    Double_t Paeronpesum;
    Double_t HcalEprTrackNorm;
    Double_t HcalEtotTrackNorm;
    Double_t HcerNpeSum;

    //Hodoscope start time 
    Double_t HhodStartTime;
    Double_t PhodStartTime;
    
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
    Double_t PgtrX;
    Double_t PgtrTh;
    Double_t PgtrY;
    Double_t PgtrPh;
   
    Double_t HgtrX;
    Double_t HgtrTh;
    Double_t HgtrY;
    Double_t HgtrPh;
    
    //Trigger 
    Double_t TcoinpTRIG1_ROC1_tdcTimeRaw;
    Double_t TcoinpTRIG4_ROC1_tdcTimeRaw;
    Double_t TcoinpTRIG1_ROC2_tdcTimeRaw;
    Double_t TcoinpTRIG4_ROC2_tdcTimeRaw;
    
        
    //Int_t EvtType;
    Double_t EvtType;  // GetLeaf()->GetValue() returns double only
    
    tree->SetBranchAddress("P.gtr.p", &PgtrP);            
    tree->SetBranchAddress("H.gtr.p", &HgtrP);            
    tree->SetBranchAddress("P.gtr.beta", &PgtrBetaMes);        
    tree->SetBranchAddress("H.gtr.beta", &HgtrBetaMes);

    tree->SetBranchAddress("P.cal.eprtracknorm", &PcalEprTrackNorm);
    tree->SetBranchAddress("P.cal.etottracknorm", &PcalEtotTrackNorm);
    tree->SetBranchAddress("P.hgcer.npeSum", &PhgcerNpeSum); 
    tree->SetBranchAddress("P.aero.npeSum", &Paeronpesum);
   
    tree->SetBranchAddress("H.cal.eprtracknorm", &HcalEprTrackNorm);
    tree->SetBranchAddress("H.cal.etottracknorm", &HcalEtotTrackNorm);
    tree->SetBranchAddress("H.cer.npeSum", &HcerNpeSum);

    tree->SetBranchAddress("P.hod.starttime", &PhodStartTime);
    tree->SetBranchAddress("P.hod.fpHitsTime", &PhodfpHitsTime);
    tree->SetBranchAddress("H.hod.starttime", &HhodStartTime);    
    tree->SetBranchAddress("H.hod.fpHitsTime", &HhodfpHitsTime);

    tree->SetBranchAddress("P.gtr.x", &PgtrX);
    tree->SetBranchAddress("P.gtr.th", &PgtrTh);
    tree->SetBranchAddress("P.gtr.y", &PgtrY);
    tree->SetBranchAddress("P.gtr.ph", &PgtrPh);
    
    tree->SetBranchAddress("H.gtr.x", &HgtrX);
    tree->SetBranchAddress("H.gtr.th", &HgtrTh);
    tree->SetBranchAddress("H.gtr.y", &HgtrY);
    tree->SetBranchAddress("H.gtr.ph", &HgtrPh);
    
    tree->SetBranchAddress("H.dc.x_fp", &HdcXfp);
    tree->SetBranchAddress("H.dc.xp_fp", &HdcXpfp);
    tree->SetBranchAddress("H.dc.y_fp", &HdcYfp);
    tree->SetBranchAddress("H.dc.yp_fp", &HdcYpfp);    

    tree->SetBranchAddress("P.dc.x_fp", &PdcXfp);
    tree->SetBranchAddress("P.dc.xp_fp", &PdcXpfp);
    tree->SetBranchAddress("P.dc.y_fp", &PdcYfp);
    tree->SetBranchAddress("P.dc.yp_fp", &PdcYpfp);    

    tree->SetBranchAddress("T.coin.pTRIG1_ROC1_tdcTimeRaw", &TcoinpTRIG1_ROC1_tdcTimeRaw);
    tree->SetBranchAddress("T.coin.pTRIG4_ROC1_tdcTimeRaw", &TcoinpTRIG4_ROC1_tdcTimeRaw);
    tree->SetBranchAddress("T.coin.pTRIG1_ROC2_tdcTimeRaw", &TcoinpTRIG1_ROC2_tdcTimeRaw);
    tree->SetBranchAddress("T.coin.pTRIG4_ROC2_tdcTimeRaw", &TcoinpTRIG4_ROC2_tdcTimeRaw);


    //tree->SetBranchAddress("fEvtHdr.fEvtType", &EvtData);  // This branch has some problem at this moment, SetBranchAddress() does not work. It's a nested branch.

    Int_t nEvents = tree->GetEntries();
    cout << "Number of events: "<< nEvents <<endl;

    TCut evtCut;
    TCut eBetaCut;
    TCut pBetaCut;
    TCut cerCut; 
    TCut calCut; 
    TCut hodoTimeCut;
    TCut paeroCut;
	 eBetaCut = "P.gtr.beta < 2.5";
	 pBetaCut = "H.gtr.beta < 2.0";
	 cerCut = "P.hgcer.npeSum > 0. && H.cer.npeSum > 0.0";
         paeroCut = "P.aero.npeSum > 0.0";
	 calCut = "H.cal.etottracknorm > 0.6 && H.cal.etottracknorm < 2.0 && H.cal.eprtracknorm  > 0.2";

    
    TCanvas *canvas1 = new TCanvas("canvas1","canvas1");
    tree->Draw("P.hod.starttime >> SHMShodoStartTime",  eBetaCut && pBetaCut && cerCut && calCut);
    TCanvas *canvas2 = new TCanvas("canvas2","canvas2");
    tree->Draw("H.hod.starttime >> HMShodoStartTime",  eBetaCut && pBetaCut && cerCut && calCut);

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
        
    cout << "Mean of SHMS Hodoscope start time: "<< PhodoStartTimeMean <<endl;
    cout << "Mean of HMS Hodoscope start time: "<< HhodoStartTimeMean <<endl;
    cout << "Number of events passing PID cut (without looping over events approach): "<<h1PhodoStartTime->GetEntries()<<endl;

    for (Int_t event = 0; event < nEvents; ++event)
    {
	if(event%10000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);
	
	    if( !(PgtrBetaMes > 0. && PgtrBetaMes < 2.5                       // proton beta cut
		  //		  && HgtrBetaMes < 2.5                   // e beta cut
		    && HcerNpeSum > 1. 
		  && PhgcerNpeSum > 0.        // Cerenkov counter cut for e- and pion
                    && Paeronpesum > 0.
                    && PcalEtotTrackNorm < 0.2
		  && HcalEtotTrackNorm > 0.6 && HcalEtotTrackNorm < 2.0        // HMS Calorimeter cut 
		  && HcalEprTrackNorm > 0.2) 	                               // HMS calorimeter cut       
		)
		continue;



	DeltaHMSpathLength = 12.462*HdcXpfp + 0.1138*HdcXpfp*HdcXfp - 0.0154*HdcXfp - 72.292*HdcXpfp*HdcXpfp - 0.0000544*HdcXfp*HdcXfp - 116.52*HdcYpfp*HdcYpfp;
	//	DeltaSHMSpathLength = -0.11*atan2(PgtrTh,1)*1000 - 0.057*PgtrdP;  // See elog entry: 3517525
	
	PgtrBetaCalc = PgtrP/sqrt(PgtrP*PgtrP + SHMSpartMass*SHMSpartMass);
	HgtrBetaCalc = HgtrP/sqrt(HgtrP*HgtrP + HMSpartMass*HMSpartMass);

	
	SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*PgtrBetaCalc) + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
	HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / (speedOfLight*HgtrBetaCalc) + (HhodoStartTimeMean - HhodfpHitsTime);      

	SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*PgtrBetaCalc) + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
	HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / (speedOfLight*HgtrBetaCalc) + (HhodoStartTimeMean - HhodfpHitsTime);      
	
	SHMScorrCoinTimeROC1 = (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; // 0.1 to convert to ns
	SHMScorrCoinTimeROC2 = (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; 
       	
	//------ Fill the hostograms -------
	h1PgtrBetaMes->Fill(PgtrBetaMes);	
        h1PcointimeROC2->Fill(SHMScorrCoinTimeROC2);                                                                
        h1HgtrBetaMes->Fill(HgtrBetaMes);
	h1PgtrBetaCalc->Fill(PgtrBetaCalc);	
	h1HgtrBetaCalc->Fill(HgtrBetaCalc);
	h1hgcernpe->Fill(PhgcerNpeSum);
	h1caletot->Fill(PcalEtotTrackNorm);
	h1calepr->Fill(PcalEprTrackNorm);
        h2ctimevsepr->Fill(SHMScorrCoinTimeROC2,PcalEprTrackNorm);
        h2ctimevsbeta->Fill(SHMScorrCoinTimeROC2,PgtrBetaMes);
        h2calevsbeta->Fill(PcalEtotTrackNorm,PgtrBetaMes);
	h1PhodfpHitsTime->Fill(PhodfpHitsTime);
	h1HhodfpHitsTime->Fill(HhodfpHitsTime);
	h1Paeronpe->Fill(Paeronpesum);
	h1PcointimeROC2->Fill(SHMScorrCoinTimeROC2);

	++totEvents;
    }
    cout << "Total number of events passing the PID cuts (looping over events): "<< totEvents <<endl;

    
    TCanvas *c2 = new TCanvas("c2", "SHMS PID");
    c2->Divide(2,2);
    c2->cd(1);
    h2calevsbeta->Draw("COLZ");
    c2->cd(2);
    h1caletot->Draw();
    c2->cd(3);
    h1calepr->Draw();
    c2->cd(4);
    h2ctimevsepr->Draw("COLZ");

    TCanvas *c1 = new TCanvas("c1", "SHMS cointime and beta");
    c1->Divide(2,2);
    c1->cd(1);
    h1HgtrBetaMes->Draw();
    c1->cd(2);
    h1Paeronpe->Draw();
    c1->cd(3);
    h1PcointimeROC2->Draw();
    c1->cd(4);
    h2ctimevsbeta->Draw("COLZ");

    
}
