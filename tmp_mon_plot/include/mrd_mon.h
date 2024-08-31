//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Aug 30 13:37:36 2024 by ROOT version 6.32.04
// from TTree mrdmonitor_tree/MRD Monitoring tree
// found on file: MRD_2024_8-21.root
//////////////////////////////////////////////////////////

#ifndef mrd_mon_h
#define mrd_mon_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLine.h>
#include <TLine.h>
#include <TBox.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

class mrd_mon {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   ULong64_t       t_start;
   ULong64_t       t_end;
   vector<unsigned int> *crate;
   vector<unsigned int> *slot;
   vector<unsigned int> *channel;
   vector<double>  *tdc;
   vector<double>  *rms;
   vector<double>  *rate;
   vector<int>     *channelcount;
   Double_t        rate_cosmic;
   Double_t        rate_beam;
   Double_t        rate_noloopback;
   Double_t        rate_normalhit;
   Double_t        rate_doublehit;
   Double_t        rate_zerohits;
   Int_t           nevents;

   // List of branches
   TBranch        *b_t_start;   //!
   TBranch        *b_t_end;   //!
   TBranch        *b_crate;   //!
   TBranch        *b_slot;   //!
   TBranch        *b_channel;   //!
   TBranch        *b_tdc;   //!
   TBranch        *b_rms;   //!
   TBranch        *b_rate;   //!
   TBranch        *b_channelcount;   //!
   TBranch        *b_rate_cosmic;   //!
   TBranch        *b_rate_beam;   //!
   TBranch        *b_rate_noloopback;   //!
   TBranch        *b_rate_normalhit;   //!
   TBranch        *b_rate_doublehit;   //!
   TBranch        *b_rate_zerohits;   //!
   TBranch        *b_nevents;   //!

   mrd_mon(TTree *tree=0);
   virtual ~mrd_mon();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual bool     Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef mrd_mon_cxx
mrd_mon::mrd_mon(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("MRD_2024_8-21.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("MRD_2024_8-21.root");
      }
      f->GetObject("mrdmonitor_tree",tree);
   }
   Init(tree);
}

mrd_mon::~mrd_mon()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t mrd_mon::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t mrd_mon::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void mrd_mon::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   crate = 0;
   slot = 0;
   channel = 0;
   tdc = 0;
   rms = 0;
   rate = 0;
   channelcount = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("t_start", &t_start, &b_t_start);
   fChain->SetBranchAddress("t_end", &t_end, &b_t_end);
   fChain->SetBranchAddress("crate", &crate, &b_crate);
   fChain->SetBranchAddress("slot", &slot, &b_slot);
   fChain->SetBranchAddress("channel", &channel, &b_channel);
   fChain->SetBranchAddress("tdc", &tdc, &b_tdc);
   fChain->SetBranchAddress("rms", &rms, &b_rms);
   fChain->SetBranchAddress("rate", &rate, &b_rate);
   fChain->SetBranchAddress("channelcount", &channelcount, &b_channelcount);
   fChain->SetBranchAddress("rate_cosmic", &rate_cosmic, &b_rate_cosmic);
   fChain->SetBranchAddress("rate_beam", &rate_beam, &b_rate_beam);
   fChain->SetBranchAddress("rate_noloopback", &rate_noloopback, &b_rate_noloopback);
   fChain->SetBranchAddress("rate_normalhit", &rate_normalhit, &b_rate_normalhit);
   fChain->SetBranchAddress("rate_doublehit", &rate_doublehit, &b_rate_doublehit);
   fChain->SetBranchAddress("rate_zerohits", &rate_zerohits, &b_rate_zerohits);
   fChain->SetBranchAddress("nevents", &nevents, &b_nevents);
   Notify();
}

bool mrd_mon::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void mrd_mon::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t mrd_mon::Cut(Long64_t entry)
{
   //if (t_start > 1724230000000 && t_start < 1724242000000) return 1;
   //if (t_start > 1714044004000 && t_start < 1714130404000) return 1; //April 25-26
   if (t_start > 1719314404000 && t_start < 1719400804000) return 1; //June 25-26
   else return -1;
}
#endif // #ifdef mrd_mon_cxx
