// Filename: SIDIS_kine_compare.C
// Description: W, Q2, x , z_had, MM, pt & phi_had distributions are compared between data and SIMC 
// Needs number of runs, particle in SHMS, SIMC filename and normfac


void SIDIS_kine_compare(Int_t runnumber, Int_t targ=1, Int_t pisign=1, Int_t kingrp=1, Int_t ptgrp=1){


  TString ROOT_FILE_PATH = "/net/cdaq/cdaql3data/cdaq/hallc-online/ROOTfiles/";
  TString SIMC_ROOT_FILE_PATH = "/net/cdaq/cdaql3data/cdaq/hallc-online/SIMC_ROOTfiles/";

  TString fileNameM = SIMC_ROOT_FILE_PATH;
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
  

  gROOT->SetBatch(kFALSE);
  gROOT->ForceStyle(); 
  //  gStyle->SetPalette(kOcean);

  Double_t pionMass = 0.1395701835;    //Gev/c^2       //added
  if (targ ==2) Double_t mtarget = 1.87561298212; //Gev/c2  mass of deuterium (check,verify)
  else if (targ == 1) Double_t mtarget = 0.93827; //Gev/c2  mass of hydrogen for run 2050

  Double_t z_had;                                         //added
  Double_t E_pi, diff_p, ptd, MissMass2d, pKinW_2, pkinW;                                //added

  
  //make histograms:
  TH1D *h_Q2d = new TH1D("h_Q2d","Q2 (GeV2)",100,0,15);  //Q2
  TH1D *h_Q2s = new TH1D("h_Q2s","Q2 (GeV2",100,0,15);
  TH1D *h_W2d = new TH1D("h_W2d","W2 (GeV2)", 150, 0, 15.0);// W2
  TH1D *h_W2s = new TH1D("h_W2s","W2 (GeV2)", 150, 0, 15.0);
  TH1D *h_xd = new TH1D("h_xd","X_bj", 100,0, 1.3);   //added                                     
  TH1D *h_xs = new TH1D("h_xs","X_bj", 100,0, 1.3);    //added                                   
  TH1D *h_zd = new TH1D("h_zd","z_had", 100,0, 1.3);   //added                                    
  TH1D *h_zs = new TH1D("h_zs","z_had", 100,0, 1.3);   //added  
  TH1D *h_ptd = new TH1D("h_ptd","pt (GeV)", 100,-0.5, 3);   
  TH1D *h_pts = new TH1D("h_pts","pt (GeV)", 100,-0.5, 3);
  TH1D *h_MissMass2d = new TH1D("h_MissMass2d", "MissMass2 (GeV2)", 150, -2, 2);
  TH1D *h_MissMass2s = new TH1D("h_MissMass2s", "MissMass2 (GeV2)", 150, -2, 2); 
  TH1D *h_phipqd = new TH1D ("h_phipqd", "Phi_hadron (Rad)", 100, -4, 7);
  TH1D *h_phipqs = new TH1D ("h_phipqs", "Phi_hadron (Rad)", 100, -4, 7);

  TH1D *s_tmp = new TH1D("s_delta","stmp",100,-12,12);  //tmp hist to get integral 

  TChain *tt = new TChain("T");

    cout << "opening run number: " <<  runnumber <<"\n";
    //read the input file from data
    TString fileNameD = ROOT_FILE_PATH;
    fileNameD += "coin_replay_production_"; //read the root file from data
    fileNameD += runnumber; //read the root file from data
    fileNameD += "_-1.root"; //read the root file from data

    tt->Add(fileNameD);
  
  //  TFile *f1 = new TFile(fileNameD);
  //  TTree *tt = (TTree*)f1->Get("T");
  //get the relevant branch
  int nentriesD = tt->GetEntries();
  cout<<"Entries:\t"<<nentriesD<<endl;

  Double_t HgtrX, HgtrTh, HgtrY, HgtrPh, hdelta, pdelta;// Ep, Epp, p_e, p_p, defEm;
  Double_t HgtrBetaCalc, PgtrBetaCalc, evtType, PgtrP, HgtrP, PhodStatus, PhodStartTime, PhodfpHitsTime;
  Double_t cointime, HhodStatus, HhodStartTime, HhodfpHitsTime, SHMSpartMass, HMSpartMass;
  Double_t pkinomega, pkinQ2, pkinxbj, pEm, pPm, pbeta, hbeta, hcalepr, hcaletot, hcernpe, pcaletot, pcalepr, pcernpe, paeronpe;
  Double_t pkinPmiss, pkinq, pkinthetaq, pkinphx_cm, pkinph_xq; //Added

  // Foal plane coords
  Double_t PdcXfp;
  Double_t PdcXpfp;
  Double_t PdcYfp;
  Double_t PdcYpfp;

  Double_t HdcXfp;
  Double_t HdcXpfp;
  Double_t HdcYfp;
  Double_t HdcYpfp;


  int cnts=0;
  TCut hpdelta;
  TCut evtCut;                                                                   
  TCut eBetaCut;                                                                 
  TCut pBetaCut;                                                                 
  TCut cerCut;                                                                   
  TCut calCut;                                                                   
  TCut hodoTimeCut;
  TCut Wcut;
  TCut aeroCut;

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
  tt->SetBranchAddress("H.kin.primary.W", &pkinW);
  tt->SetBranchAddress("H.kin.primary.omega", &pkinomega);
  tt->SetBranchAddress("H.kin.primary.Q2", &pkinQ2);
  tt->SetBranchAddress("H.kin.primary.x_bj", &pkinxbj); 

  tt->SetBranchAddress("H.kin.primary.th_q", &pkinthetaq);        
  tt->SetBranchAddress("H.kin.primary.q3m", &pkinq);              

  tt->SetBranchAddress("P.kin.secondary.ph_xq",&pkinph_xq);    //"Azimuth of Detected particle with scattering plane (rad)"                                   
  if (targ == 2) tt->SetBranchAddress("P.kin.secondary.emiss_nuc", &pEm);   //missing energy                                          
  else if (targ == 1) tt->SetBranchAddress("P.kin.secondary.emiss", &pEm);   //missing energy     
  tt->SetBranchAddress("P.kin.secondary.pmiss", &pPm);    //missing momentum                                              
                                                                                                              
  // tt->SetBranchAddress("P.hod.goodstarttime", &PhodStatus);                                              
  tt->SetBranchAddress("P.hod.starttime", &PhodStartTime);                                               
  tt->SetBranchAddress("P.hod.fpHitsTime", &PhodfpHitsTime);                                             
                                                                                                             
  //tt->SetBranchAddress("H.hod.goodstarttime", &HhodStatus);                                              
  tt->SetBranchAddress("H.hod.starttime", &HhodStartTime);                                               
  tt->SetBranchAddress("H.hod.fpHitsTime", &HhodfpHitsTime);                                             
                                                                                                             
  tt->SetBranchAddress("H.gtr.x", &HgtrX);                                                               
  tt->SetBranchAddress("H.gtr.th", &HgtrTh);                                                             
  tt->SetBranchAddress("H.gtr.y", &HgtrY);                                                               
  tt->SetBranchAddress("H.gtr.ph", &HgtrPh);                                                             

    
  tt->SetBranchAddress("H.dc.x_fp", &HdcXfp);
  tt->SetBranchAddress("H.dc.xp_fp", &HdcXpfp);
  tt->SetBranchAddress("H.dc.y_fp", &HdcYfp);
  tt->SetBranchAddress("H.dc.yp_fp", &HdcYpfp);    

  tt->SetBranchAddress("P.dc.x_fp", &PdcXfp);
  tt->SetBranchAddress("P.dc.xp_fp", &PdcXpfp);
  tt->SetBranchAddress("P.dc.y_fp", &PdcYfp);
  tt->SetBranchAddress("P.dc.yp_fp", &PdcYpfp);    

                                                                                                   
   
      SHMSpartMass = 0.1395701835; // pion mass in GeV/c^2                 
      HMSpartMass = 0.000510998; // electron mass in GeV/c^2                 
      hpdelta = "P.gtr.dp > -10 && P.gtr.dp < 10 && H.gtr.dp > -8 && H.gtr.dp < 8";
      eBetaCut = "P.gtr.beta > 0.6 && P.gtr.beta < 1.4";                        
      pBetaCut = "H.gtr.beta > 0.8 &&  H.gtr.beta < 1.2";                       
      cerCut = "P.hgcer.npeSum > 2.0 && H.cer.npeSum > 0.1";                    
      calCut = "H.cal.etottracknorm > 0.8 && H.cal.etottracknorm < 1.5 && H.cal.eprtracknorm  > 0.2";             aeroCut ="P.aero.npeSum > 2.0";
      //  Wcut = "H.kin.primary.W >0.5 && H.kin.primary.W < 1.5";


  for (int kk=0; kk<nentriesD; kk++){
    tt->GetEntry(kk);
    if (kk % 10000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
     evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
     if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.2 && pcernpe>2.0 && paeronpe>2.0 && hcernpe>0. && hcaletot >0.6 && hcaletot<2.0  && hdelta > -10 && hdelta < 10 && pdelta > -12 && pdelta < 12)     
      { //cuts to select the electrons and pions
	  cnts++;


	  h_Q2d->Fill(pkinQ2);
	  pKinW_2=pkinW*pkinW;
	  h_W2d->Fill(pKinW_2);
	  h_xd->Fill(pkinxbj);
	  E_pi = sqrt(pow(pionMass,2) + pow(PgtrP,2));
	  z_had = E_pi/pkinomega;
	  h_zd->Fill(z_had);
	  ptd = (PgtrP)*sin(pkinthetaq);
          h_ptd->Fill(ptd);  
          MissMass2d = pEm*pEm - pPm*pPm;
	  h_MissMass2d->Fill(MissMass2d); 
	  h_phipqd->Fill(pkinph_xq);

	}
  }  

  h_Q2d->GetXaxis()->SetTitle("Q2 (GeV2)");
  h_xd->GetXaxis()->SetTitle("x_bj");
  h_W2d->GetXaxis()->SetTitle("W2 (GeV2)");
  h_zd->GetXaxis()->SetTitle("z_had");
  h_ptd->GetXaxis()->SetTitle("pt(GeV)");  
  h_MissMass2d->GetXaxis()->SetTitle("MissMass2 (GeV2)");  
  h_phipqd->GetXaxis()->SetTitle("Phi_hadron (Rad)");

  cout << cnts <<" events passed all cuts" << endl;

  /////////////////////////////////////////////////////////
  ///////////SIMULATION STARTS HERE///////////////////////
  ///////////////////////////////////////////////////////

  double spdelta, sQ2, sxbj, szhad, wt, sdelta, sfac, sMissMass2, normwt;
  double sthetapq, spt, sMissMass, sEm, sPm, q_q, n_u ,E_pion,sWpr, p_pi, sphipq, p_0, sW, sW2;  
 

  for (int kk=0; kk<nentriesM; kk++){
    ts->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesM << "   % of simc done" << endl;

    sdelta = ts->GetLeaf("hsdelta")->GetValue();
    spdelta = ts->GetLeaf("ssdelta")->GetValue();
    sQ2 = ts->GetLeaf("Q2")->GetValue();
    sxbj = ts->GetLeaf("xbj")->GetValue();
    szhad = ts->GetLeaf("z")->GetValue();
    sMissMass = ts->GetLeaf("missmass")->GetValue(); 
    sMissMass2 = sMissMass*sMissMass;
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
    if (sdelta < 10 && sdelta > -10 && spdelta > -12 && spdelta < 12 )
      {
	h_Q2s->Fill(sQ2,normwt);
	h_xs->Fill(sxbj,normwt);
	h_W2s->Fill(sW2,normwt);
	h_zs->Fill(szhad,normwt);
	h_pts->Fill(spt,normwt);  
	h_MissMass2s->Fill(sMissMass2,normwt);  
	h_phipqs->Fill(sphipq, normwt);
      }
  }
  h_Q2s->GetXaxis()->SetTitle("Q2 (GeV2)");
  h_xs->GetXaxis()->SetTitle("x_bj");
  h_W2s->GetXaxis()->SetTitle("W2 (GeV2)");
  h_zs->GetXaxis()->SetTitle("z_had");
  h_pts->GetXaxis()->SetTitle("pt(GeV)");  
  h_MissMass2s->GetXaxis()->SetTitle("MissMass2 (GeV2)");
  h_phipqs->GetXaxis()->SetTitle("Phi_hadron (Rad)");

  // TCanvas *cc = new TCanvas ("cc","cc",1000,1000);
  TCanvas *cc = new TCanvas ("cc",Form("cc Run: %d",runnumber));

  cc->SetGrid();
  cc->SetLeftMargin(0.2);
  cc->SetRightMargin(0.2);
  gStyle->SetOptStat(0);

  cc->Divide(4,2);

  cc->cd(1);
  h_W2d->SetFillColor(kBlue);
  h_W2d->SetMarkerColor(kBlue);
  h_W2d->SetLineColor(kBlue);
  h_W2d->DrawNormalized("p e1");
  h_W2s->SetMarkerColor(kRed);
  h_W2s->SetLineColor(kRed);
  h_W2s->DrawNormalized("SAME");


  cc->cd(2);
  h_Q2d->SetFillColor(kBlue);
  h_Q2d->SetMarkerColor(kBlue);
  h_Q2d->SetLineColor(kBlue);
  h_Q2d->DrawNormalized("p e1");
  h_Q2s->SetFillStyle(1001);
  h_Q2s->SetLineColor(kRed);
  h_Q2s->SetMarkerColor(kRed);
  h_Q2s->DrawNormalized("same");


  cc->cd(3);
  h_xd->SetFillColor(kBlue);
  h_xd->SetMarkerColor(kBlue);
  h_xd->SetLineColor(kBlue);
  h_xd->DrawNormalized("p e1");
  h_xs->SetFillStyle(1001);
  h_xs->SetMarkerColor(kRed);
  h_xs->SetLineColor(kRed);
  h_xs->DrawNormalized("same");
  
  cc->cd(4);
  h_zd->SetFillColor(kBlue);
  h_zd->SetMarkerColor(kBlue);
  h_zd->SetLineColor(kBlue);
  h_zd->DrawNormalized("p e1");
  h_zs->SetFillStyle(1001);
  h_zs->SetMarkerColor(kRed);
  h_zs->SetLineColor(kRed);
  h_zs->DrawNormalized("same");

  
  cc->cd(5);
  h_ptd->SetFillColor(kBlue);
  h_ptd->SetMarkerColor(kBlue);
  h_ptd->SetLineColor(kBlue);
  h_ptd->DrawNormalized("p e1");
  h_pts->SetFillStyle(1001);
  h_pts->SetMarkerColor(kRed);
  h_pts->SetLineColor(kRed);
  h_pts->DrawNormalized("same");


  cc->cd(6);
  h_MissMass2d->SetFillColor(kBlue);
  h_MissMass2d->SetMarkerColor(kBlue);
  h_MissMass2d->SetLineColor(kBlue);
  h_MissMass2d->DrawNormalized("p e1");
  h_MissMass2s->SetFillStyle(1001);
  h_MissMass2s->SetMarkerColor(kRed);
  h_MissMass2s->SetLineColor(kRed);
  h_MissMass2s->DrawNormalized("same");

  cc->cd(7);
  h_phipqd->SetFillColor(kBlue);
  h_phipqd->SetMarkerColor(kBlue);
  h_phipqd->SetLineColor(kBlue);
  h_phipqd->DrawNormalized("p e1");
  h_phipqs->SetFillStyle(1001);
  h_phipqs->SetMarkerColor(kRed);
  h_phipqs->SetLineColor(kRed);
  h_phipqs->DrawNormalized("same");

  // cc->SaveAs("UTIL_SIDIS/PLOTS/sidis_kine_comp_${runnumber}.pdf");
  cc->Print(Form("/net/cdaqfs/home/cdaq/hallc-online/hallc_replay/UTIL_SIDIS/PLOTS/%d_SIDIS_kine_compare.pdf",runnumber));



}
