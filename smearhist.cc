int findrange(float pt){
  int i=-1;
  if(pt<20) i=0;
  else if(pt<40) i=1;
  else if(pt<60) i=2;
  else if(pt<80) i=3;
  else if(pt<100) i=4;
  else if(pt<150) i=5;
  else if(pt<200) i=6;
  else if(pt<250) i=7;
  else if(pt<300) i=8;
  else if(pt<400) i=9;
  else i=10;
  return i;
}

TCanvas * MakeCanvas(TH1F* h1, TH1F* h2, TH1F* h4, std::string xtitle="xaxis title", std::string canv=""){
   TCanvas * c =  new TCanvas(("c"+canv).c_str(), "c", 900, 900);

   TLine *line = new TLine(h1->GetXaxis()->GetXmin(), 1, h1->GetXaxis()->GetXmax(), 1);
   line->SetLineColor(kRed);

   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   pad1->SetBottomMargin(0); // Upper and lower plot are joined
   pad1->SetGridx();         // Vertical grid
   pad1->Draw();             // Draw the upper pad: pad1
   pad1->cd();               // pad1 becomes the current pad
   pad1->SetLogy();

   h1->SetStats(0);          // No statistics on upper plot
   h1->Draw();               // Draw h1
   h2->Draw("same");         // Draw h2 on top of h1
   if(h4!=NULL) h4->Draw("same");

   auto legend = new TLegend(0.65,0.7,0.85,0.85);
   if(h4!=NULL) {
     legend->AddEntry(h1,"MC QCD","pl");
     legend->AddEntry(h2,"Data JetHT","pl");
     legend->AddEntry(h4,"MC QCD smeared","pl");
   } else {
     legend->AddEntry(h1,"MC QCD l","pl");
     legend->AddEntry(h2,"MC QCD b","pl");
   }
   legend->Draw();

   c->cd();          // Go back to the main canvas before defining pad2
   TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
   pad2->SetTopMargin(0);
   pad2->SetBottomMargin(0.2);
   pad2->SetGridx(); // vertical grid
   pad2->Draw();
   pad2->cd();       // pad2 becomes the current pad

   TH1F *h3 = (TH1F*)h1->Clone("h3");
   TH1F *h5 = (TH1F*)h4->Clone("h5");

   h3->SetLineColor(kBlack);
   h3->SetMinimum(0.5);  // Define Y ..
   h3->SetMaximum(1.5); // .. range
   h3->Sumw2();
   h3->SetStats(0);      // No statistics on lower plot
   h3->Divide(h2);
   h3->SetMarkerStyle(21);
   h3->Draw("ep");       // Draw the ratio plot
   line->Draw("same");
   h5->SetLineColor(kGreen);
   h5->Sumw2();
   h5->Divide(h2);
   h5->SetMarkerStyle(34);
   h5->SetMarkerColor(kGreen);
   h5->Draw("epsame");       // Draw the ratio plot

   h1->SetLineColor(kBlack);
   h1->SetLineWidth(2);
   h1->GetYaxis()->SetTitle("A.U.");
   h1->GetYaxis()->SetTitleSize(20);
   h1->GetYaxis()->SetTitleFont(43);
   h1->GetYaxis()->SetTitleOffset(1.55);
   h2->SetLineColor(kBlue);
   h2->SetLineWidth(2);
   if(h4!=NULL) {
     h4->SetLineColor(kGreen);
     h4->SetLineWidth(2);
   }

   h3->SetTitle(""); // Remove the ratio title
   h3->GetYaxis()->SetTitle("QCD/JetHT");

   h3->GetYaxis()->SetNdivisions(505);
   h3->GetYaxis()->SetTitleSize(20);
   h3->GetYaxis()->SetTitleFont(43);
   h3->GetYaxis()->SetTitleOffset(1.55);
   h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   h3->GetYaxis()->SetLabelSize(15);

   h3->GetXaxis()->SetTitle(xtitle.c_str());
   h3->GetXaxis()->SetTitleSize(20);
   h3->GetXaxis()->SetTitleFont(43);
   h3->GetXaxis()->SetTitleOffset(3.);
   h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   h3->GetXaxis()->SetLabelSize(15);

   return c;
}

void smearhist(){
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  float sf1[11] = {0.835875, 0.864944, 0.896117, 0.963411, 0.913783, 0.953378, 0.878533, 0.883834, 0.753378, 0.779021, 1};
  float sf2[11] = {0.852679, 0.87797, 0.943221, 0.947182, 0.958432, 0.971444, 1.03438, 1.03238, 0.954846, 1.09476, 1};

  TFile* f1 = TFile::Open("qcd.root", "READ");
  TFile* f2 = TFile::Open("JetHT.root", "READ");
  TFile* f3 = TFile::Open("GJet.root", "READ");

  TTree* t1 = (TTree*)f1->Get("emj/seltrktree");
  TTree* t2 = (TTree*)f2->Get("emj/seltrktree");
  TTree* t3 = (TTree*)f3->Get("emj/seltrktree");

  double seltrk_ip2d_1, seltrk_pvdz_1, btag_1;
  double seltrk_ip2d_2, seltrk_pvdz_2, btag_2;
  double seltrk_ip2d_3, seltrk_pvdz_3, btag_3;
  double pt_1, pt_2, pt_3;

  t1->SetBranchAddress("seltrk_ip2d", &seltrk_ip2d_1);
  t1->SetBranchAddress("seltrk_pvdz", &seltrk_pvdz_1);
  t1->SetBranchAddress("btagscore2", &btag_1);
  t1->SetBranchAddress("seltrk_pt", &pt_1);
  t2->SetBranchAddress("seltrk_ip2d", &seltrk_ip2d_2);
  t2->SetBranchAddress("seltrk_pvdz", &seltrk_pvdz_2);
  t2->SetBranchAddress("btagscore2", &btag_2);
  t2->SetBranchAddress("seltrk_pt", &pt_2);
  t3->SetBranchAddress("seltrk_ip2d", &seltrk_ip2d_3);
  t3->SetBranchAddress("seltrk_pvdz", &seltrk_pvdz_3);
  t3->SetBranchAddress("btagscore2", &btag_3);
  t3->SetBranchAddress("seltrk_pt", &pt_3);

  TH1F* hl1_ip2d = new TH1F("hl1_ip2d", "hl_ip2d", 50, 0, 0.005);
  TH1F* hl1_pvdz = new TH1F("hl1_pvdz", "hl_pvdz", 100, -0.01, 0.01);
  TH1F* hl2_ip2d = new TH1F("hl2_ip2d", "hl_ip2d", 50, 0, 0.005);
  TH1F* hl2_pvdz = new TH1F("hl2_pvdz", "hl_pvdz", 100, -0.01, 0.01);
  TH1F* hl3_ip2d = new TH1F("hl3_ip2d", "hl_ip2d", 50, 0, 0.005);
  TH1F* hl3_pvdz = new TH1F("hl3_pvdz", "hl_pvdz", 100, -0.01, 0.01);

  TH1F* hb1_ip2d = new TH1F("hb1_ip2d", "hb_ip2d", 50, 0, 0.005);
  TH1F* hb1_pvdz = new TH1F("hb1_pvdz", "hb_pvdz", 100, -0.01, 0.01);
  TH1F* hb2_ip2d = new TH1F("hb2_ip2d", "hb_ip2d", 50, 0, 0.005);
  TH1F* hb2_pvdz = new TH1F("hb2_pvdz", "hb_pvdz", 100, -0.01, 0.01);
  TH1F* hb3_ip2d = new TH1F("hb3_ip2d", "hb_ip2d", 50, 0, 0.005);
  TH1F* hb3_pvdz = new TH1F("hb3_pvdz", "hb_pvdz", 100, -0.01, 0.01);

  TH1F* hl1_ip2d_s = new TH1F("hl1_ip2d_s", "hl_ip2d_s", 50, 0, 0.005);
  TH1F* hl1_pvdz_s = new TH1F("hl1_pvdz_s", "hl_pvdz_s", 100, -0.01, 0.01);

  TH1F* hb1_ip2d_s = new TH1F("hb1_ip2d_s", "hb_ip2d_s", 50, 0, 0.005);
  TH1F* hb1_pvdz_s = new TH1F("hb1_pvdz_s", "hb_pvdz_s", 100, -0.01, 0.01);


  for(int i=0; i<t1->GetEntries(); i++){
    if(i%1000000==0) std::cout << (float) i*100./(float) t1->GetEntries() << "%% processed" << std::endl;
    t1->GetEntry(i);
    if(btag_1<0.1){
      hl1_ip2d->Fill(seltrk_ip2d_1);
      hl1_pvdz->Fill(seltrk_pvdz_1);

      //hl1_ip2d_s->Fill(gRandom->Gaus(seltrk_ip2d_1, 0.001));
      //hl1_pvdz_s->Fill(gRandom->Gaus(seltrk_pvdz_1,-1.59033e-03));
      hl1_ip2d_s->Fill(seltrk_ip2d_1/sf1[findrange(pt_1)]);
      hl1_pvdz_s->Fill(seltrk_pvdz_1/sf2[findrange(pt_1)]);
    } else if(btag_1>0.4506) {
      hb1_ip2d->Fill(seltrk_ip2d_1);
      hb1_pvdz->Fill(seltrk_pvdz_1);

      //hb1_ip2d_s->Fill(gRandom->Gaus(seltrk_ip2d_1, 0.001));
      //hb1_pvdz_s->Fill(gRandom->Gaus(seltrk_pvdz_1,-1.59033e-03));
      hb1_ip2d_s->Fill(seltrk_ip2d_1/sf1[findrange(pt_1)]);
      hb1_pvdz_s->Fill(seltrk_pvdz_1/sf2[findrange(pt_1)]);
    }
  }

  for(int i=0; i<t2->GetEntries(); i++){
    if(i%100000==0) std::cout << (float) i*100./(float) t2->GetEntries() << "%% processed" << std::endl;
    t2->GetEntry(i);
    if(btag_2<0.1){
      hl2_ip2d->Fill(seltrk_ip2d_2);
      hl2_pvdz->Fill(seltrk_pvdz_2);
    } else if(btag_2>0.4506) {
      hb2_ip2d->Fill(seltrk_ip2d_2);
      hb2_pvdz->Fill(seltrk_pvdz_2);
    }
  }

  for(int i=0; i<t3->GetEntries(); i++){
    if(i%100000==0) std::cout << (float) i*100./(float) t3->GetEntries() << "%% processed" << std::endl;
    t3->GetEntry(i);
    if(btag_3<0.1){
      hl3_ip2d->Fill(seltrk_ip2d_3);
      hl3_pvdz->Fill(seltrk_pvdz_3);
    } else if(btag_3>0.4506) {
      hb3_ip2d->Fill(seltrk_ip2d_3);
      hb3_pvdz->Fill(seltrk_pvdz_3);
    }
  }

/*
for(int ibin=1; ibin< hl1_pvdz->GetNbinsX()+1; ibin++){
    double ip = hl1_pvdz->GetBinCenter(ibin);
    double ntrk = hl1_pvdz->GetBinContent(ibin);
    for(int itrk=1; itrk< ntrk; itrk++){
      hl1_pvdz_s->Fill(gRandom->Gaus((ip), -1.59033e-03));
    }
}

for(int ibin=1; ibin< hb1_pvdz->GetNbinsX()+1; ibin++){
    double ip = hb1_pvdz->GetBinCenter(ibin);
    double ntrk = hb1_pvdz->GetBinContent(ibin);
    for(int itrk=1; itrk< ntrk; itrk++){
      hb1_pvdz_s->Fill(gRandom->Gaus((ip), -1.59033e-03));
    }
}
*/

  hl1_ip2d->Sumw2(kTRUE);
  hl1_pvdz->Sumw2(kTRUE);
  hl2_ip2d->Sumw2(kTRUE);
  hl2_pvdz->Sumw2(kTRUE);
  hl3_ip2d->Sumw2(kTRUE);
  hl3_pvdz->Sumw2(kTRUE);
  hb1_ip2d->Sumw2(kTRUE);
  hb1_pvdz->Sumw2(kTRUE);
  hb2_ip2d->Sumw2(kTRUE);
  hb2_pvdz->Sumw2(kTRUE);
  hb3_ip2d->Sumw2(kTRUE);
  hb3_pvdz->Sumw2(kTRUE);

  hl1_ip2d_s->Sumw2(kTRUE);
  hl1_pvdz_s->Sumw2(kTRUE);
  hb1_ip2d_s->Sumw2(kTRUE);
  hb1_pvdz_s->Sumw2(kTRUE);

  hl1_ip2d->Scale(1./hl1_ip2d->Integral());
  hl1_pvdz->Scale(1./hl1_pvdz->Integral());
  hl2_ip2d->Scale(1./hl2_ip2d->Integral());
  hl2_pvdz->Scale(1./hl2_pvdz->Integral());
  hl3_ip2d->Scale(1./hl3_ip2d->Integral());
  hl3_pvdz->Scale(1./hl3_pvdz->Integral());
  hb1_ip2d->Scale(1./hb1_ip2d->Integral());
  hb1_pvdz->Scale(1./hb1_pvdz->Integral());
  hb2_ip2d->Scale(1./hb2_ip2d->Integral());
  hb2_pvdz->Scale(1./hb2_pvdz->Integral());
  hb3_ip2d->Scale(1./hb3_ip2d->Integral());
  hb3_pvdz->Scale(1./hb3_pvdz->Integral());

  hl1_ip2d_s->Scale(1./hl1_ip2d_s->Integral());
  hl1_pvdz_s->Scale(1./hl1_pvdz_s->Integral());
  hb1_ip2d_s->Scale(1./hb1_ip2d_s->Integral());
  hb1_pvdz_s->Scale(1./hb1_pvdz_s->Integral());
  

  TCanvas* c1 = MakeCanvas(hl1_pvdz, hl2_pvdz, hl1_pvdz_s, "z_{PV}-z_{trk}/cm", "1");
  TCanvas* c2 = MakeCanvas(hb1_pvdz, hb2_pvdz, hb1_pvdz_s, "z_{PV}-z_{trk}/cm", "2");

  TCanvas* c3 = MakeCanvas(hl1_ip2d, hl2_ip2d, hl1_ip2d_s, "IP_{2D}/cm", "3");
  TCanvas* c4 = MakeCanvas(hb1_ip2d, hb2_ip2d, hb1_ip2d_s, "IP_{2D}/cm", "4");

  c1->Print("reweight(","pdf");
  c2->Print("reweight","pdf");

  c3->Print("reweight","pdf");
  c4->Print("reweight)","pdf");

}



