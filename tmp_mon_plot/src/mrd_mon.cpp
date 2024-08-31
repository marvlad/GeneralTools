#define mrd_mon_cxx
#include "mrd_mon.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

std::string unix_time(long long unixTimeMillis){
    unixTimeMillis = 1693456800000; 
    time_t unixTime = unixTimeMillis / 1000;
    struct tm *localTime = localtime(&unixTime);
    //std::cout << "CST Time: " << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << std::endl;
    
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    std::string timer = oss.str();
    //cout << timer <<endl;
    return timer;
}

void mrd_mon::Loop()
{
//   In a ROOT session, you can do:
//      root> .L mrd_mon.C
//      root> mrd_mon t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TH1F* channelcount_h = new TH1F("hits", "hits", 384, 0., 384.);

   std::vector<vector<int>> ch_hist;

   std::vector<int> myVector(384, 0);
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      fChain->GetEntry(jentry);   //nbytes += nb;
      if (Cut(ientry) < 0) continue;
      for (int a=0; a < myVector.size(); a++){
         myVector[a] += (*channelcount)[a];
      }
   }
   for(int a=0; a < myVector.size(); a++){
      channelcount_h->SetBinContent(a+1, myVector[a]);
      cout << myVector[a] << " ";
   }

   TCanvas *c = new TCanvas("c", "c", 800, 600);
   c->SetLogy();
   gStyle->SetOptStat(0);

   channelcount_h->Draw("hist");

   for (int bin = 1; bin <= 192; ++bin) {
      double xlow = channelcount_h->GetBinLowEdge(bin);
      double xup = channelcount_h->GetBinLowEdge(bin + 1);
      TBox *box = new TBox(xlow, 0, xup, channelcount_h->GetBinContent(bin));
      box->SetFillColor(kGreen);
      box->Draw("same");
   }

   for (int bin = 193; bin <= 384; ++bin) {
      double xlow = channelcount_h->GetBinLowEdge(bin);
      double xup = channelcount_h->GetBinLowEdge(bin + 1);
      TBox *box = new TBox(xlow, 0, xup, channelcount_h->GetBinContent(bin));
      box->SetFillColor(kBlue);
      box->Draw("same");
    }

   for (int bin = 32; bin <= 384; bin += 32) {
       double x = channelcount_h->GetBinLowEdge(bin); 
       TLine* line = new TLine(x, 0, x, channelcount_h->GetMaximum());
       line->SetLineStyle(2); // Dashed line
       line->Draw();
   }

   c->Update();
   c->SaveAs("./the_hist.pdf");
}

int main(int argc, char *argv[]){
   if (argc < 2) {
      cout << "Usage: " << argv[0] << " <rootfile>" << endl;
      return 1;  
   }

   cout << "hola "<< endl;
   long long unixTimeMillis = 1693456800000;
   std::string time = unix_time(unixTimeMillis);
   cout << "The time is "<< time << endl; // dummy test

   std::string rootFilePath = argv[1];
   TFile *f = TFile::Open(rootFilePath.c_str());

   TTree *tree = (TTree*)f->Get("mrdmonitor_tree");

   mrd_mon my_mrd(tree);
   my_mrd.Loop();

   return 0;
}
