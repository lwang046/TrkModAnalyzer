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
     legend->AddEntry(h1,"MC multjet","pl");
     legend->AddEntry(h2,"Data JetHT","pl");
     legend->AddEntry(h4,"MC multjet scaled","pl");
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

void scalehist(){
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  float sf_ll_ip2d[40] = {0, 0.954437, 0.982722, 0.976605, 0.977314, 0.991134, 0.978813, 0.949165, 0.949382, 0.92692, 0.939051, 0.9371, 0.92061, 0.942483, 0.870573, 0.907867, 0.861894, 0.907211, 0.872308, 0.86322, 0.865968, 0.842095, 0.890404, 0.768298, 0.804297, 0.892434, 0.980858, 0.805239, 0.976068, 0.927401, 0.896811, 0.880994, 1.06216, 0.686474, 1.30167, 1.36107, 1.33748, 0.942785, 1};
  float sf_ll_pvdz[40] = {0, 1.00053, 0.996661, 1.00191, 1.00313, 1.01406, 1.02682, 1.01496, 1.0096, 1.01751, 1.04389, 1.01441, 0.997571, 1.02404, 1.01631, 1.03158, 1.0222, 1.02707, 1.01597, 0.998612, 1.0294, 0.986171, 0.93551, 1.00184, 0.966653, 0.959335, 0.875512, 0.974555, 0.868157, 0.929308, 0.864414, 1.02654, 1.18758, 0.980267, 0.9385, 1.01915, 1.03526, 1.16774, 1};
  float sf_lh_ip2d[40] = {0, 0.758381, 0.916829, 0.929424, 0.932543, 0.93212, 0.932828, 0.937354, 0.930218, 0.93758, 0.934587, 0.930322, 0.929186, 0.928835, 0.928469, 0.922895, 0.914194, 0.911597, 0.897108, 0.885367, 0.904942, 0.896473, 0.902294, 0.920599, 0.909911, 0.885635, 0.925279, 0.969393, 1.01422, 1.00975, 0.983354, 1.07741, 1.02189, 0.95423, 0.974223, 1.02011, 1.11633, 1.06614, 1};
  float sf_lh_pvdz[40] = {0, 1.02841, 1.01274, 1.00178, 0.990667, 0.988653, 0.989245, 0.996869, 0.986228, 1.00673, 1.00609, 1.00404, 1.0151, 1.0088, 1.00533, 1.00584, 1.01882, 0.999982, 0.996981, 0.977961, 1.00091, 0.980627, 0.978314, 0.978449, 0.979661, 0.907885, 0.95006, 0.900013, 0.917037, 0.926044, 0.8841, 0.957713, 0.898902, 0.98874, 0.921616, 1.01968, 0.90432, 0.889422, 1};

  TFile* f1 = TFile::Open("QCD.root", "READ");
  TFile* f2 = TFile::Open("JetHT_2017B.root", "READ");
  TFile* f3 = TFile::Open("SinglePhoton_2017B.root", "READ");

  TTree* t1 = (TTree*)f1->Get("emj/seltrktree");
  TTree* t2 = (TTree*)f2->Get("emj/seltrktree");
  TTree* t3 = (TTree*)f3->Get("emj/seltrktree");

  double seltrk_ip2d_1, seltrk_pvdz_1, btag_1;
  double seltrk_ip2d_2, seltrk_pvdz_2, btag_2;
  double seltrk_ip2d_3, seltrk_pvdz_3, btag_3;
  double pt_1, pt_2, pt_3;
  double eta_1, eta_2, eta_3;

  t1->SetBranchAddress("seltrk_ip2d", &seltrk_ip2d_1);
  t1->SetBranchAddress("seltrk_pvdz", &seltrk_pvdz_1);
  t1->SetBranchAddress("btagscore2", &btag_1);
  t1->SetBranchAddress("seltrk_pt", &pt_1);
  t1->SetBranchAddress("seltrk_eta", &eta_1);
  t2->SetBranchAddress("seltrk_ip2d", &seltrk_ip2d_2);
  t2->SetBranchAddress("seltrk_pvdz", &seltrk_pvdz_2);
  t2->SetBranchAddress("btagscore2", &btag_2);
  t2->SetBranchAddress("seltrk_pt", &pt_2);
  t2->SetBranchAddress("seltrk_eta", &eta_2);
  t3->SetBranchAddress("seltrk_ip2d", &seltrk_ip2d_3);
  t3->SetBranchAddress("seltrk_pvdz", &seltrk_pvdz_3);
  t3->SetBranchAddress("btagscore2", &btag_3);
  t3->SetBranchAddress("seltrk_pt", &pt_3);
  t3->SetBranchAddress("seltrk_eta", &eta_3);

  TH1F* hl1_ip2d = new TH1F("hl1_ip2d", "hl_ip2d", 100, 0, 0.05);
  TH1F* hl1_pvdz = new TH1F("hl1_pvdz", "hl_pvdz", 200, -0.05, 0.05);
  TH1F* hl2_ip2d = new TH1F("hl2_ip2d", "hl_ip2d", 100, 0, 0.05);
  TH1F* hl2_pvdz = new TH1F("hl2_pvdz", "hl_pvdz", 200, -0.05, 0.05);
  TH1F* hl3_ip2d = new TH1F("hl3_ip2d", "hl_ip2d", 100, 0, 0.05);
  TH1F* hl3_pvdz = new TH1F("hl3_pvdz", "hl_pvdz", 200, -0.05, 0.05);

  TH1F* hb1_ip2d = new TH1F("hb1_ip2d", "hb_ip2d", 100, 0, 0.05);
  TH1F* hb1_pvdz = new TH1F("hb1_pvdz", "hb_pvdz", 200, -0.05, 0.05);
  TH1F* hb2_ip2d = new TH1F("hb2_ip2d", "hb_ip2d", 100, 0, 0.05);
  TH1F* hb2_pvdz = new TH1F("hb2_pvdz", "hb_pvdz", 200, -0.05, 0.05);
  TH1F* hb3_ip2d = new TH1F("hb3_ip2d", "hb_ip2d", 100, 0, 0.05);
  TH1F* hb3_pvdz = new TH1F("hb3_pvdz", "hb_pvdz", 200, -0.05, 0.05);

  TH1F* hl1_ip2d_s = new TH1F("hl1_ip2d_s", "hl_ip2d_s", 100, 0, 0.05);
  TH1F* hl1_pvdz_s = new TH1F("hl1_pvdz_s", "hl_pvdz_s", 200, -0.05, 0.05);

  TH1F* hb1_ip2d_s = new TH1F("hb1_ip2d_s", "hb_ip2d_s", 100, 0, 0.05);
  TH1F* hb1_pvdz_s = new TH1F("hb1_pvdz_s", "hb_pvdz_s", 200, -0.05, 0.05);


  for(int i=0; i<t1->GetEntries(); i++){
    if(i%10000000==0) std::cout << (float) i*100./(float) t1->GetEntries() << "%% processed" << std::endl;
    t1->GetEntry(i);
    int nn = (pt_1>200)? 40 : ( pt_1<20? (int) (pt_1/1) : 20 + (int) ((pt_1-20)/10) );
    if(btag_1<0.1){
      hl1_ip2d->Fill(seltrk_ip2d_1);
      hl1_pvdz->Fill(seltrk_pvdz_1);
      if(fabs(eta_1)<1.5) {
        hl1_ip2d_s->Fill(seltrk_ip2d_1*sf_ll_ip2d[nn]);
        hl1_pvdz_s->Fill(seltrk_pvdz_1*sf_ll_pvdz[nn]);
      } else {
        hl1_ip2d_s->Fill(seltrk_ip2d_1*sf_lh_ip2d[nn]);
        hl1_pvdz_s->Fill(seltrk_pvdz_1*sf_lh_pvdz[nn]);
      }
    } else if(btag_1>0.4506) {
      hb1_ip2d->Fill(seltrk_ip2d_1);
      hb1_pvdz->Fill(seltrk_pvdz_1);
      if(fabs(eta_1)<1.5) {
        hb1_ip2d_s->Fill(seltrk_ip2d_1*sf_ll_ip2d[nn]);
        hb1_pvdz_s->Fill(seltrk_pvdz_1*sf_ll_pvdz[nn]);
      } else {
        hb1_ip2d_s->Fill(seltrk_ip2d_1*sf_lh_ip2d[nn]);
        hb1_pvdz_s->Fill(seltrk_pvdz_1*sf_lh_pvdz[nn]);
      }
    }
  }

  for(int i=0; i<t2->GetEntries(); i++){
    if(i%10000000==0) std::cout << (float) i*100./(float) t2->GetEntries() << "%% processed" << std::endl;
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
    if(i%10000000==0) std::cout << (float) i*100./(float) t3->GetEntries() << "%% processed" << std::endl;
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

  //c1->Print("reweight(","pdf");
  //c2->Print("reweight","pdf");

  //c3->Print("reweight","pdf");
  //c4->Print("reweight)","pdf");

  c1->SaveAs("scale_ljet_ip2d.pdf");
  c2->SaveAs("scale_bjet_ip2d.pdf");
  c3->SaveAs("scale_ljet_pvdz.pdf");
  c4->SaveAs("scale_bjet_pvdz.pdf");

}



