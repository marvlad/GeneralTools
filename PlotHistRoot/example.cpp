/////////////////////////////////////////////////////////////////////////////////////
// Draft script to plot a ROOT hist and ratio hist in a same canvas
// M. Ascencio-Sosa, Aug 14, 2024

#include <iostream>
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <TLatex.h>
#include <TStyle.h>
#include <TRatioPlot.h>

using namespace std;

TFile* get_file(std::string pth){
    TFile* file = TFile::Open(pth.c_str());
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file" << std::endl;
    }
    return file;
}

TH1D* get_hist(TFile *f, std::string variable){
    TH1D *h = (TH1D*)f->Get(variable.c_str());
    return h;
}

TH1D* get_ratio(TH1D* h1, TH1D* h2){
	TH1D* ratio = (TH1D*)h1->Clone();
	ratio->Divide(h2);
	return ratio;
}

void plotHistogramsWithRatio(TH1D* h1, TH1D* h2, std::string vr_n, std::string vr_l1, std::string vr_l2) {

    gStyle->SetOptStat(0);

    auto c1 = new TCanvas("c1", "A ratio example", 800, 600);

    h1->SetLineColor(kRed);  
    h2->SetLineColor(kBlue);  
    h1->SetLineStyle(1);     
    h2->SetLineStyle(1);     
    h1->SetLineWidth(3);
    h2->SetLineWidth(3);

    double maxY1 = h1->GetMaximum();
    double maxY2 = h2->GetMaximum();
    
    double maxY = TMath::Max(maxY1, maxY2) * 1.2; 
    h1->GetYaxis()->SetRangeUser(0, maxY);
    h1->GetXaxis()->SetTitle(vr_n.c_str());
    h1->GetYaxis()->SetTitle("Entries");

    auto legend = new TLegend(0.5, 0.6, 0.9, 0.8); 
    legend->AddEntry(h1, vr_l1.c_str(), "l"); 
    legend->AddEntry(h2, vr_l2.c_str(), "l");
    legend->SetBorderSize(0); 
    legend->SetFillStyle(0); 
    legend->Draw(); 

    auto rp = new TRatioPlot(h1, h2);
    rp->SetH1DrawOpt("hist");
    rp->SetH2DrawOpt("hist same");
    rp->Draw("hist");
    legend->Draw("same");

    c1->Update();
    std::string out = vr_n + "_rt.pdf";
    c1->SaveAs(out.c_str()); 
} 

void plot_hist(TH1D*h){
    TCanvas *c = new TCanvas("c", "c", 100, 80);
    c->cd();
    h->Draw("hist");
    c->SaveAs("ratio.pdf");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Hey! Usage: " << argv[0] << " <string1> [string2] ..." << std::endl;
        return 1;
    }

    std::string vr(argv[1]);

    std::string pth_01 = "A.root";
    std::string pth_02 = "B.root";

    std::string vr_l1 = "noOpalVarND_fhc_nocut";
    std::string vr_l2 = "wOpalVarND_fhc_nocut";

    TH1D* h1 = get_hist(get_file(pth_01.c_str()), vr.c_str());
    TH1D* h2 = get_hist(get_file(pth_02.c_str()), vr.c_str());
    TH1D* ratio = get_ratio(h1, h2);

    plotHistogramsWithRatio(h1,h2,vr,vr_l1,vr_l2);

    return 0;
}
