
TCanvas * MakeCanvas(TH1F* h1, TH1F* h2, TH1F* h3=NULL, std::string xtitle="xaxis title", std::string canv=""){
   TCanvas * c =  new TCanvas(("c"+canv).c_str(), "c", 800, 800);

   TLine *line = new TLine(h1->GetXaxis()->GetXmin(), 1, h1->GetXaxis()->GetXmax(), 1);
   line->SetLineColor(kRed);

   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   pad1->SetBottomMargin(0); // Upper and lower plot are joined
   pad1->SetGridx();         // Vertical grid
   pad1->Draw();             // Draw the upper pad: pad1
   pad1->cd();               // pad1 becomes the current pad
   //pad1->SetLogy();

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

if(h3!=NULL) {
   h3->Draw("sames");
   pad1->Update();
   TPaveStats *st3 = (TPaveStats*)h3->GetListOfFunctions()->FindObject("stats");
   st3->SetName("MC scaled");
   st3->SetX1NDC(0.8);
   st3->SetX2NDC(0.95);
   st3->SetY1NDC(0.5);
   st3->SetY2NDC(0.65);
}

   h1->SetMinimum(0);
   h1->SetMaximum(0.018);
   h1->SetLineColor(kBlack);
   h1->SetLineWidth(2);
   h1->GetYaxis()->SetTitle("A.U.");
   h1->GetYaxis()->SetTitleSize(20);
   h1->GetYaxis()->SetTitleFont(43);
   h1->GetYaxis()->SetTitleOffset(1.55);
   h2->SetLineColor(kBlue);
   h2->SetLineWidth(2);
if(h3!=NULL) {
   h3->SetLineColor(kGreen);
   h3->SetLineWidth(2);
}
   auto legend = new TLegend(0.55,0.7,0.75,0.85);
   legend->AddEntry(h1,"track inclusive","pl");
   legend->AddEntry(h2,"track 2.0<pt<3.0","pl");
   if(h3!=NULL) legend->AddEntry(h3,"track 14.0<pt<15.0","pl");
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
if(h3!=NULL){
   r2->Divide(h3);
   r2->GetListOfFunctions()->Delete();
   r2->SetStats(kFALSE);
   r2->Draw("ep same"); 
}
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

void ptprofilelog(){
  gROOT->SetBatch(1);

  TFile* f1 = TFile::Open("data/JetHT.root", "READ");
  TFile* f2 = TFile::Open("data/aJetHT.root", "READ");

  TTree* t1 = (TTree*)f1->Get("emj/alltrktree");
  TTree* t2 = (TTree*)f2->Get("emj/alltrktree");

  double alltrk_ip2d_1, alltrk_pvdz_1;
  double alltrk_ip2d_2, alltrk_pvdz_2;
  double pt_1, eta_1;
  double pt_2, eta_2;
  double alltrk_ip2dfromcanddxy;

  t1->SetBranchAddress("alltrk_ip2dfromdxy", &alltrk_ip2d_1);
  //t1->SetBranchAddress("alltrk_pvdz", &alltrk_pvdz_1);
  t1->SetBranchAddress("alltrk_pt", &pt_1);
  t1->SetBranchAddress("alltrk_eta", &eta_1);

  t2->SetBranchAddress("alltrk_ip2dfromdxy", &alltrk_ip2d_2);  
  //t2->SetBranchAddress("alltrk_pvdz", &alltrk_pvdz_2);
  t2->SetBranchAddress("alltrk_pt", &pt_2);
  t2->SetBranchAddress("alltrk_eta", &eta_2);
  //t2->SetBranchAddress("alltrk_ip2dfromcanddxy", &alltrk_ip2dfromcanddxy);  

  double xbins1[20], xbins2[18];
  for(int ii=0; ii<20; ii++){
    xbins1[ii] = 1.*(double) ii;
    if(ii<18) xbins2[ii] = 20.+ 10.*(double) ii;
  }
  double * xbins = new double[39];
  std::copy(xbins1, xbins1+20, xbins);
  std::copy(xbins2, xbins2+18, xbins+20);
  xbins[38] = 200.;


  TH1F* h1_ip2d = new TH1F("h1_ip2d", "ip2d", 450, -6, 3);
  TH1F* h1_pvdz = new TH1F("h1_pvdz", "pvdz", 2000, 0.005, 10.005);
  TH1F* h2_ip2d = new TH1F("h2_ip2d", "ip2d", 450, -6, 3);
  TH1F* h2_pvdz = new TH1F("h2_pvdz", "pvdz", 2000, 0.005, 10.005);

  TH1F* h3_ip2d = new TH1F("h3_ip2d", "ip2d", 450, -6, 3);
  TH1F* h3_pvdz = new TH1F("h3_pvdz", "pvdz", 2000, 0.005, 10.005);

  /*for(int i=0; i<t1->GetEntries(); i++){
    if(i%10000000==0) std::cout << (float) i*100./(float) t1->GetEntries() << "%% processed" << std::endl;
    t1->GetEntry(i);
    if(fabs(eta_1)<1.5 && pt_1>2 && pt_1<3) {
        //if(abs(alltrk_ip2d_1)>=0.05) 
        h1_ip2d->Fill(log10(abs(alltrk_ip2d_1)));
        //h1_pvdz->Fill(fabs(alltrk_pvdz_1));

        h3_ip2d->Fill(log10(abs(alltrk_ip2d_1*0.78844)));
        //h3_pvdz->Fill(fabs(alltrk_pvdz_1*0.941401));
    }
  }*/

  for(int i=0; i<t2->GetEntries(); i++){
    if(i%10000000==0) std::cout << (float) i*100./(float) t2->GetEntries() << "%% processed" << std::endl;
    t2->GetEntry(i);
    if(fabs(eta_2)<3.5) 
        h1_ip2d->Fill(log10(abs(alltrk_ip2d_2)));
    if(fabs(eta_2)<3.5 && pt_2>2. && pt_2<3.) { 
        //if(abs(alltrk_ip2d_2)>=0.05) 
        h2_ip2d->Fill(log10(abs(alltrk_ip2d_2)));
        //h2_pvdz->Fill(fabs(alltrk_pvdz_2));
    }
    if(fabs(eta_2)<3.5 && pt_2>14. && pt_2<15.) 
        h3_ip2d->Fill(log10(abs(alltrk_ip2d_2)));
  }

  h1_ip2d->Sumw2();
  h2_ip2d->Sumw2();
  h3_ip2d->Sumw2();
  h1_ip2d->Scale(1./h1_ip2d->Integral());
  h2_ip2d->Scale(1./h2_ip2d->Integral());
  h3_ip2d->Scale(1./h3_ip2d->Integral());

  TCanvas *c1 = MakeCanvas(h1_ip2d, h2_ip2d, h3_ip2d, "log_{10}(IP_{2D}/cm)", "1");
  c1->SaveAs("ptprofilelog_ip2d.pdf");
  c1->SaveAs("ptprofilelog_ip2d.root");
  //TCanvas *c3 = MakeCanvas(h1_pvdz, h2_pvdz, h3_pvdz, "z_{PV}-z_{trk}/cm", "3");
  //c3->SaveAs("ptprofile_pvdz.pdf");


}



