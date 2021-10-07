
TCanvas * MakeCanvas(TH1F *h1, TH1F *h2, TH1F *h4=NULL, bool dofit=false, std::string xtitle="xaxis title", std::string canv=""){
   h1->SetLineColor(kBlack);
   TCanvas * c =  new TCanvas(("c"+canv).c_str(), "c", 900, 900);

   TLine *line = new TLine(h1->GetXaxis()->GetXmin(), 1, h1->GetXaxis()->GetXmax(), 1);
   line->SetLineColor(kRed);

   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   pad1->SetBottomMargin(0); // Upper and lower plot are joined
   pad1->SetGridx();         // Vertical grid
   pad1->Draw();             // Draw the upper pad: pad1
   pad1->cd();               // pad1 becomes the current pad
   pad1->SetLogy();

   if(dofit){
     float N=3.5, bias=0;
     if(canv=="1") {N=3; bias = 0;}
     double fparmc[6];
     TF1 *fmc1 = new TF1("fmc1", "gaus", h1->GetXaxis()->GetXmin()+bias, h1->GetXaxis()->GetXmax()/N);
     TF1 *fmc2 = new TF1("fmc2", "gaus", h1->GetXaxis()->GetXmin()+bias, h1->GetXaxis()->GetXmax());
     TF1 *fmct = new TF1("fmct", "[0]*exp(-0.5*((x)/[1])**2) + [2]*exp(-0.5*((x)/[3])**2)", h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());
     h1->Fit(fmc1, "R");
     h1->Fit(fmc2, "R+");
     fmc1->GetParameters(&fparmc[0]);
     fmc2->GetParameters(&fparmc[3]);
     fmct->SetParameter(0, fparmc[0]);
     fmct->SetParameter(1, fparmc[2]);
     fmct->SetParameter(2, fparmc[3]);
     fmct->SetParameter(3, fparmc[5]);
     h1->Fit(fmct, "R");
   }

   h1->SetStats(0);          // No statistics on upper plot
   h1->SetLineColor(kBlack);
   h1->Draw();               // Draw h1
   h2->Draw("same");         // Draw h2 on top of h1
   if(h4!=NULL) h4->Draw("same");

   auto legend = new TLegend(0.65,0.7,0.85,0.85);
   if(h4!=NULL) {
     legend->AddEntry(h1,"MC mult. jet","pl");
     legend->AddEntry(h2,"Data JetHT","pl");
     legend->AddEntry(h4,"Data #gammaJet","pl");
   } else {
     legend->AddEntry(h1,"MC |#eta|<1.5","pl");
     legend->AddEntry(h2,"MC |#eta|>1.5","pl");
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
   h3->SetLineColor(kBlack);
   h3->SetMinimum(0.5);  // Define Y ..
   h3->SetMaximum(1.5); // .. range
   h3->Sumw2();
   h3->SetStats(0);      // No statistics on lower plot
   h3->Divide(h2);
   h3->SetMarkerStyle(21);
   h3->Draw("ep");       // Draw the ratio plot
   line->Draw("same");

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
   h3->GetYaxis()->SetTitle("MC/JetHT");

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

void trackpareta(){
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

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

  for(int i=0; i<t1->GetEntries(); i++){
    if(i%10000000==0) std::cout << (float) i*100./(float) t1->GetEntries() << "%% processed" << std::endl;
    t1->GetEntry(i);
    if(fabs(eta_1)<1.5){
      hl1_ip2d->Fill(seltrk_ip2d_1);
      hl1_pvdz->Fill(seltrk_pvdz_1);
    } else {
      hb1_ip2d->Fill(seltrk_ip2d_1);
      hb1_pvdz->Fill(seltrk_pvdz_1);
    }
  }

  for(int i=0; i<t2->GetEntries(); i++){
    if(i%10000000==0) std::cout << (float) i*100./(float) t2->GetEntries() << "%% processed" << std::endl;
    t2->GetEntry(i);
    if(fabs(eta_2)<1.5){
      hl2_ip2d->Fill(seltrk_ip2d_2);
      hl2_pvdz->Fill(seltrk_pvdz_2);
    } else {
      hb2_ip2d->Fill(seltrk_ip2d_2);
      hb2_pvdz->Fill(seltrk_pvdz_2);
    }
  }

  for(int i=0; i<t3->GetEntries(); i++){
    if(i%10000000==0) std::cout << (float) i*100./(float) t3->GetEntries() << "%% processed" << std::endl;
    t3->GetEntry(i);
    if(fabs(eta_3)<1.5){
      hl3_ip2d->Fill(seltrk_ip2d_3);
      hl3_pvdz->Fill(seltrk_pvdz_3);
    } else {
      hb3_ip2d->Fill(seltrk_ip2d_3);
      hb3_pvdz->Fill(seltrk_pvdz_3);
    }
  }

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


  TCanvas *c1 = MakeCanvas(hl1_ip2d, hl2_ip2d, hl3_ip2d, false, "IP_{2D}/cm", "1");
  //c1->Print("canv(","pdf");
  TCanvas *c2 = MakeCanvas(hb1_ip2d, hb2_ip2d, hb3_ip2d, false, "IP_{2D}/cm", "2");
  //c2->Print("canv", "pdf");
  TCanvas *c3 = MakeCanvas(hl1_pvdz, hl2_pvdz, hl3_pvdz, false, "z_{PV}-z_{trk}/cm", "3");
  //c3->Print("canv", "pdf");
  TCanvas *c4 = MakeCanvas(hb1_pvdz, hb2_pvdz, hb3_pvdz, false, "z_{PV}-z_{trk}/cm", "4");
  //c4->Print("canv", "pdf");
  TCanvas *c5 = MakeCanvas(hl1_ip2d, hb1_ip2d, NULL, false, "IP_{2D}/cm", "5");
  //c5->Print("canv", "pdf");
  TCanvas *c6 = MakeCanvas(hl1_pvdz, hb1_pvdz, NULL, false, "z_{PV}-z_{trk}/cm", "6");
  //c6->Print("canv)","pdf");

  c1->SaveAs("leta_ip2d.pdf");
  c2->SaveAs("heta_ip2d.pdf");
  c3->SaveAs("leta_pvdz.pdf");
  c4->SaveAs("heta_pvdz.pdf");
  c5->SaveAs("lvhip2d.pdf");
  c6->SaveAs("lvhpvdz.pdf");

}



