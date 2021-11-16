
TCanvas * MakeCanvas(TH1F* h1, TH1F* h2, TH1F* h3, std::string xtitle="xaxis title", std::string canv="", string legendheader=""){
   TCanvas * c =  new TCanvas(("c"+canv).c_str(), "c", 800, 800);

   TLine *line = new TLine(h1->GetXaxis()->GetXmin(), 1, h1->GetXaxis()->GetXmax(), 1);
   line->SetLineColor(kRed);

   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   pad1->SetBottomMargin(0); // Upper and lower plot are joined
   pad1->SetGridx();         // Vertical grid
   pad1->Draw();             // Draw the upper pad: pad1
   pad1->cd();               // pad1 becomes the current pad
   pad1->SetLogy();

   h1->GetListOfFunctions()->Delete();
   h2->GetListOfFunctions()->Delete();
   h3->GetListOfFunctions()->Delete();

   h1->Draw();               // Draw h1
   pad1->Update();
   TPaveStats *st1 = (TPaveStats*)h1->GetListOfFunctions()->FindObject("stats");
   st1->SetName("MC");
   st1->SetX1NDC(0.8);
   st1->SetX2NDC(0.95);
   st1->SetY1NDC(0.8);
   st1->SetY2NDC(0.95);

   h2->Draw("sames");         // Draw h2 on top of h1
   pad1->Update();
   TPaveStats *st2 = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
   st2->SetName("Data");
   st2->SetX1NDC(0.8);
   st2->SetX2NDC(0.95);
   st2->SetY1NDC(0.65);
   st2->SetY2NDC(0.8);

   h3->Draw("sames");
   pad1->Update();
   TPaveStats *st3 = (TPaveStats*)h3->GetListOfFunctions()->FindObject("stats");
   st3->SetName("MC scaled");
   st3->SetX1NDC(0.8);
   st3->SetX2NDC(0.95);
   st3->SetY1NDC(0.5);
   st3->SetY2NDC(0.65);

   h1->SetMinimum(0.0001);
   //h1->SetMaximum(0.018);
   h1->SetLineColor(kBlack);
   h1->SetLineWidth(2);
   h1->GetYaxis()->SetTitle("A.U.");
   h1->GetYaxis()->SetTitleSize(20);
   h1->GetYaxis()->SetTitleFont(43);
   h1->GetYaxis()->SetTitleOffset(1.55);
   h2->SetLineColor(kBlue);
   h2->SetLineWidth(2);
   h3->SetLineColor(kGreen);
   h3->SetLineWidth(2);

   auto legend = new TLegend(0.55,0.7,0.75,0.85);
   legend->SetHeader(legendheader.c_str());
if(canv=="1"){
   legend->AddEntry(h1,"trk incl.","pl");
   legend->AddEntry(h2,"trk 3<pt(GeV)<4","pl");
   legend->AddEntry(h3,"trk 14<pt(GeV)<15","pl");
} else if(canv=="2"){
   legend->AddEntry(h1,"3<pt<4","pl");
   legend->AddEntry(h2,"3<pt<4, |#eta|>1.5","pl");
   legend->AddEntry(h3,"3<pt<4, |#eta|<1.5","pl");
} else if(canv=="3"){
   legend->AddEntry(h1,"3<pt<4","pl");
   legend->AddEntry(h2,"3<pt<4, #phi>0","pl");
   legend->AddEntry(h3,"3<pt<4, #phi<0","pl");
}
   legend->Draw();

   pad1->Modified(); pad1->Update();

   c->cd();          // Go back to the main canvas before defining pad2
   TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
   pad2->SetTopMargin(0);
   pad2->SetBottomMargin(0.2);
   pad2->SetGridx(); // vertical grid
   pad2->Draw();
   pad2->cd();       // pad2 becomes the current pad

   TH1F *r1 = (TH1F*)h1->Clone("r1");
   TH1F *r2 = (TH1F*)h1->Clone("r2");
   r1->Divide(h2);
   r1->GetListOfFunctions()->Delete();
   r1->SetStats(kFALSE);
   r1->Draw("ep");
   r2->Divide(h3);
   r2->GetListOfFunctions()->Delete();
   r2->SetStats(kFALSE);
   //r2->Draw("ep same"); 

   line->Draw("same");

   //r1->SetMarkerColor(kBlue);
   r1->SetLineColor(kBlue);
   r1->SetMinimum(0.4);  // Define Y ..
   r1->SetMaximum(1.6); // .. range
   //r1->SetMarkerStyle(21);
   //r2->SetMarkerColor(kGreen);
   if(h3!=NULL) r2->SetLineColor(kGreen);

   r1->SetTitle(""); // Remove the ratio title
   r1->GetYaxis()->SetTitle("incl./cut.");

   r1->GetYaxis()->SetNdivisions(505);
   r1->GetYaxis()->SetTitleSize(20);
   r1->GetYaxis()->SetTitleFont(43);
   r1->GetYaxis()->SetTitleOffset(1.55);
   r1->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   r1->GetYaxis()->SetLabelSize(15);

   r1->GetXaxis()->SetTitle(xtitle.c_str());
   r1->GetXaxis()->SetTitleSize(20);
   r1->GetXaxis()->SetTitleFont(43);
   r1->GetXaxis()->SetTitleOffset(3.);
   r1->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   r1->GetXaxis()->SetLabelSize(15);

   pad2->Modified(); pad2->Update();

   return c;
}

void IP2Dptbinned(){
  gROOT->SetBatch(1);

  string inputnames[5] = {"QCD_HT1500to2000", "JetHT_UL2017B", "JetHT_Run2017B", "JetHT_UL2018A", "JetHT_Run2018A"};

  for(int ii=0; ii<5; ii++) {
    TFile* f2 = TFile::Open(("./data/"+inputnames[ii]+".root").c_str(), "READ");
    TTree* t2 = (TTree*)f2->Get("emj/seltrktree");

    double seltrk_ip2dsig, seltrk_pvdz;
    double pt, eta, phi;

    t2->SetBranchAddress("seltrk_ip2dsig", &seltrk_ip2dsig);  
    t2->SetBranchAddress("seltrk_pt", &pt);
    t2->SetBranchAddress("seltrk_eta", &eta);
    t2->SetBranchAddress("seltrk_phi", &phi);

    TH1F* ip2dsig_incl = new TH1F("ip2dsig_incl", "ip2dsig", 400, -2, 2);
    TH1F* ip2dsig_pt3 = new TH1F("ip2dsig_pt3", "ip2dsig", 400, -2, 2);
    TH1F* ip2dsig_pt14 = new TH1F("ip2dsig_pt14", "ip2dsig", 400, -2, 2);

    TH1F* ip2dsig_etaH_pt3 = new TH1F("ip2dsig_etaH_pt3", "ip2dsig", 400, -2, 2);
    TH1F* ip2dsig_etaL_pt3 = new TH1F("ip2dsig_etaL_pt3", "ip2dsig", 400, -2, 2);

    TH1F* ip2dsig_phiU_pt3 = new TH1F("ip2dsig_phiU_pt3", "ip2dsig", 400, -2, 2);
    TH1F* ip2dsig_phiD_pt3 = new TH1F("ip2dsig_phiD_pt3", "ip2dsig", 400, -2, 2);

    for(int i=0; i<t2->GetEntries(); i++){
      if(i%10000000==0) std::cout << (float) i*100./(float) t2->GetEntries() << "%% processed" << std::endl;
      t2->GetEntry(i);
      ip2dsig_incl->Fill(seltrk_ip2dsig);
      if(pt > 3. && pt < 4.) { 
        ip2dsig_pt3->Fill(seltrk_ip2dsig);
        if( abs(eta)>1.5 ) ip2dsig_etaH_pt3->Fill(seltrk_ip2dsig);
        if( abs(eta)<1.5 ) ip2dsig_etaL_pt3->Fill(seltrk_ip2dsig);
        if( phi>0. ) ip2dsig_phiU_pt3->Fill(seltrk_ip2dsig);
        if( phi<0. ) ip2dsig_phiD_pt3->Fill(seltrk_ip2dsig);
      }
      if(pt > 14. && pt < 15.) {
        ip2dsig_pt14->Fill(seltrk_ip2dsig);
      }
    }

    ip2dsig_incl->Sumw2();
    ip2dsig_pt3->Sumw2();
    ip2dsig_pt14->Sumw2();
    ip2dsig_etaH_pt3->Sumw2();
    ip2dsig_etaL_pt3->Sumw2();
    ip2dsig_phiU_pt3->Sumw2();
    ip2dsig_phiD_pt3->Sumw2();
    ip2dsig_incl->Scale(1./ip2dsig_incl->Integral());
    ip2dsig_pt3->Scale(1./ip2dsig_pt3->Integral());
    ip2dsig_pt14->Scale(1./ip2dsig_pt14->Integral());
    ip2dsig_etaH_pt3->Scale(1./ip2dsig_etaH_pt3->Integral());
    ip2dsig_etaL_pt3->Scale(1./ip2dsig_etaL_pt3->Integral());
    ip2dsig_phiU_pt3->Scale(1./ip2dsig_phiU_pt3->Integral());
    ip2dsig_phiD_pt3->Scale(1./ip2dsig_phiD_pt3->Integral());

    TCanvas *c1 = MakeCanvas(ip2dsig_incl, ip2dsig_pt3, ip2dsig_pt14, "IP2D Significance", "1", inputnames[ii]);
    c1->SaveAs(("IP2DSigptbinned_"+inputnames[ii]+"_seltrk_pt.pdf").c_str());
    TCanvas *c2 = MakeCanvas(ip2dsig_pt3, ip2dsig_etaH_pt3, ip2dsig_etaL_pt3, "IP2D Significance", "2", inputnames[ii]);
    c2->SaveAs(("IP2DSigptbinned_"+inputnames[ii]+"_seltrk_eta.pdf").c_str());
    TCanvas *c3 = MakeCanvas(ip2dsig_pt3, ip2dsig_phiU_pt3, ip2dsig_phiD_pt3, "IP2D Significance", "3", inputnames[ii]);
    c3->SaveAs(("IP2DSigptbinned_"+inputnames[ii]+"_seltrk_phi.pdf").c_str());
  }

}



