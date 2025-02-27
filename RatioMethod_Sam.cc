#include <iostream>
#include <fstream>
#include <iomanip>
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TFile.h"
#include "TLine.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TF1.h"
#include "TF2.h"
#include "TRandom3.h"
#include "TVirtualFFT.h"
#include "TFitter.h"
#include "TVirtualFitter.h"
#include "TFitResult.h"
#include <TMatrixDSym.h>
#include <cmath>
#include <math.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

//const double totalTime = 30 * 4200;

vector<TH1D*> GetRatioMethodHistFromData(TH1D *data_1,TH1D *data_2, TH1D *data_3, TH1D *data_4, double totalTime, double binWidth, int binNum, int choice = 0){

  Double_t Ta    = 2100.0;   // Ta = 2pi/omega_a -> Ta would be 2100 ns
  int binshift = Ta / binWidth;

  // Book histograms for R(t), D(t), S(t)

  TH1D* RatDist_hist = new TH1D("" , "" , binNum , 0 , totalTime );
  TH1D* Ddist_hist   = new TH1D("" , "" , binNum , 0 , totalTime );
  TH1D* Sdist_hist   = new TH1D("" , "" , binNum , 0 , totalTime );

 // Loop over bins

  for (int ibin = binshift+1; ibin < (binNum  - binshift); ibin++ ){

    int ibin_up = ibin + binshift;
    int ibin_do = ibin - binshift;

    // Divide bin content by 4, apply shifts and add to correct array
    double v1 = data_1 -> GetBinContent(ibin) ;
    double v2 = data_2 -> GetBinContent(ibin) ;   
    double uPlus = data_3 -> GetBinContent(ibin_up) ;
    double uMinus = data_4 -> GetBinContent(ibin_do) ;

    double Ddist = (uPlus + uMinus) - (v1 + v2); // + or -? 
    double Sdist = uPlus + uMinus + v1 + v2;
    double RatioValue = Ddist/Sdist;

    // Find error on R(t), D(t), S(t)
    double RatioError = sqrt( (1. - pow(RatioValue,2)) / Sdist );
    double DError = sqrt(Sdist);

    // Set error on R(t), D(t), S(t)
    
    RatDist_hist -> SetBinError(ibin, RatioError);
    gStyle       -> SetEndErrorSize(2);
    RatDist_hist -> SetMarkerStyle(20);
    RatDist_hist -> SetMarkerSize(0.5);
    RatDist_hist -> SetMarkerColor(kBlack);
    RatDist_hist -> Draw("E1");

    Ddist_hist -> SetBinError(ibin, DError);
    gStyle     -> SetEndErrorSize(2);
    Ddist_hist -> SetMarkerStyle(20);
    Ddist_hist -> SetMarkerSize(0.5);
    Ddist_hist -> SetMarkerColor(kBlack);
    Ddist_hist -> Draw("E1");

    Sdist_hist -> SetBinError(ibin, DError);
    gStyle     -> SetEndErrorSize(2);
    Sdist_hist -> SetMarkerStyle(1);
    Sdist_hist -> SetMarkerSize(0.5);
    Sdist_hist -> SetMarkerColor(kBlack);
    Sdist_hist -> Draw("E1");
  }
  
  vector<TH1D*> tmp = {RatDist_hist, Ddist_hist, Sdist_hist};

  return tmp;

}

int main() {

  //Try to open ROOT data file 
  //  int nHist;
  
  //  cout << "Please enter the number of histograms you want to use for the Ratio Method: " << endl;
  // cin >>  nHist;

TFile *file1 = TFile::Open("plots_60hr_15922.root", "READ");  

  if (file1 == 0) {
    
    // if we cannot open the file, print an error message and return immediatly
    printf("Oh no! Cannot open file");
    return 1;
   
  }

  TFile* file2 = new TFile("RatioMethodDataSam.root", "RECREATE"); 

//Loop over data, 24 histograms - shouldnt be hardcoded but heck it

// Just test with the first four
//for (int i = 1; i < 5; i++){ 
  int i = 1;
   string num;
  ostringstream conv;
   conv << i;

  //Need to equally fill four seperate histograms from the data file. 

   string name1 = "data_1";//+ conv.str();
   string name2 = "data_2";//+ conv.str();
  string name3 = "data_3";//+ conv.str();
  string name4 = "data_4";//+ conv.str();
  string outname1 = "RatDist_";// + conv.str();
  string outname2 = "DDist_";// + conv.str();
  string outname3 = "SDist_";// + conv.str();
  string outtitle1 = "R(t) for  ";// + conv.str();
  string outtitle2 = "D(t) for Pseudo Experiement ";// + conv.str();
  string outtitle3 = "S(t) for Pseudo Experiement ";// + conv.str();


    TH1D *h1 = (TH1D*)file1 -> Get(name1.c_str());   // Gets the  wiggle plot
    TH1D *h2 = (TH1D*)file1 -> Get(name2.c_str());   
    TH1D *h3 = (TH1D*)file1 -> Get(name3.c_str());   
    TH1D *h4 = (TH1D*)file1 -> Get(name4.c_str());   

    Double_t binWidth = h1 -> GetBinWidth(1);         
    Double_t binNum =  h1->GetNbinsX();
    Double_t totalTime = binWidth * binNum;       // Find total time
    
    // print some parameters to check everything  
    /* cout<<"total time "<< totalTime <<endl;
    cout<<"bin width for h1 "<< binWidth <<endl;
    cout<<"bin width for h2 "<< h2 -> GetBinWidth(1) <<endl;
    cout<<"bin width for h3 "<< h3 -> GetBinWidth(1) <<endl;
    cout<<"bin number for h1 "<< binNum << endl;
    cout<<"bin number for h2 "<< h2 -> GetNbinsX() << endl;
    cout<<"bin number for h3 "<< h3 -> GetNbinsX() << endl;
    */
    vector<TH1D*> tmp = GetRatioMethodHistFromData(h1, h2, h3, h4, totalTime , binWidth , binNum , 0);

    tmp.at(0) -> SetNameTitle(outname1.c_str(), outtitle1.c_str());    
    tmp.at(0) -> GetXaxis() -> SetTitle("Time (ns)");
    tmp.at(0) -> GetYaxis() -> SetTitle("R(t)");
    
    tmp.at(1) -> SetNameTitle(outname2.c_str(), outtitle2.c_str());
    tmp.at(1) -> GetXaxis() -> SetTitle("Time (ns)");
    tmp.at(1) -> GetYaxis() -> SetTitle("D(t)");
    
    tmp.at(2) -> SetNameTitle(outname3.c_str(), outtitle3.c_str());
    tmp.at(2) -> GetXaxis() -> SetTitle("Time (ns)");
    tmp.at(2) -> GetYaxis() -> SetTitle("S(t)");
     
    tmp.at(0) -> Write();
    tmp.at(1) -> Write();
    tmp.at(2) -> Write();

    cout << "End" << endl;
   
    // cout<<"iteration number "<<i<<endl;

    // }

}

