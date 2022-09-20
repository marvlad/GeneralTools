#include <iostream>
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "PlotUtils/MnvH1D.h"

using namespace std;
using namespace PlotUtils;


int main()
{
 TH1D* h = new TH1D("h","h",100,0.,100.);
 MnvH1D* h2 = new MnvH1D("h2","h2",100,0.,100.);

 TCanvas *c = new TCanvas("c","c",500,500);
 h2->Draw();
 c->SaveAs("MnvPlot.png"); 

 TFile fout("output.root","RECREATE");
 fout.cd();
 
 h2->Write("MnvHistogram");
 h->Write("RootHistogram");

 return 0;
}
