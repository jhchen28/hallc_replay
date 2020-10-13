//Main Calibration Code
#include "DC_calib.h"
#include "TString.h"
#include "DC_calib.C"
#include <iostream>
#include <ctime>
using namespace std;

int main_calib_distonly(TString type = "SHMS",int run = 9644, int entries = -1, const string mode = "wire"/*"wire or card"*/, TString calibType = "tzero"/*tzero or dist*/, const string fitOption = "integral"/*raw or integral*/, const string fitFunc = "heaviside"/*linear or heaviside*/)
{

  //prevent root from displaying graphs while executing
  gROOT->SetBatch(1);
  if(calibType.CompareTo("tzero")&&calibType.CompareTo("dist")){
    cout << "Incalid calibration type selected, choose between 'tzero' or 'dist'.\n"<<endl;
    return 1;
  };

  //measure execution time
  clock_t cl;
  cl = clock();
  type.ToLower();
  TString rootFile(Form("../../../ROOTfiles/%s_replay_production_all_%05i_%i_unCal.root", type.Data(),run, entries));
  

  cout << rootFile.Data() << endl;
                                                                                                        //pid_elec,  pid_kFALSE (no PID cuts) 
                                                                                                        // |
                                                                                                        // v
  type.ToUpper();
  DC_calib obj(type.Data(), rootFile.Data(), run, -1, "pid_elec", mode);
  //use the following line if you want to specify the root file path                                                   
  //DC_calib obj("SHMS", "../../../ROOTfiles/shms_replay_production_all_09644_-1_wireCal.root", 9644, -1, "pid_elec", "wire");                                                                                                        

  obj.setup_Directory();
  obj.SetPlaneNames();
  obj.GetDCLeafs();
  obj.AllocateDynamicArrays();
  obj.SetTdcOffset();
  obj.CreateHistoNames();
  obj.EventLoop("FillUncorrectedTimes"); // if you only want to do drift distance calibration, use option "FillUncorrectedTimes" only
  if(!calibType.CompareTo("tzero")){
    obj.Calculate_tZero(fitOption, fitFunc);
    obj.EventLoop("ApplyT0Correction");
    obj.WriteTZeroParam();
    obj.WriteLookUpTable(calibType.Data()); // if you only want to do drift distance calibration, use option "dist", for tzero calib use "tzero"
    obj.WriteToFile(1);  //set argument to (1) for debugging
  }else if(!calibType.CompareTo("dist")){
    obj.WriteLookUpTable(calibType.Data()); // if you only want to do drift distance calibration, use option "dist", for tzero calib use "tzero"
  }

  //stop clock
  cl = clock() - cl;
  cout << "execution time: " << cl/(double)CLOCKS_PER_SEC << " sec" << endl;

  return 0;
}
