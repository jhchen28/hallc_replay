void rootlogon() {
  gSystem->SetBuildDir("$HOME/.root_build_dir");
  gROOT->ProcessLine(".include /usr/local/include/hcana");
  gROOT->ProcessLine(".include .");
  R__LOAD_LIBRARY(libfmt)
  //gPrintViaErrorHandler = kTRUE;
  //gErrorIgnoreLevel = kWarning;
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gSystem->Load("libTree");
  gSystem->Load("libTreePlayer");
  gSystem->Load("libHist");
}
