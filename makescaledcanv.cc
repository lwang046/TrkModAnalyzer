void makescaledcanv(string tag, string region, string var, string dataset, string era){
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);

   string histname = "p_"+tag+"jet_"+region+"eta_"+var+"_"+dataset+"_"+era;
   string histqcd = "p_"+tag+"jet_"+region+"eta_"+var+"_QCD";
   string yname = "mean tracks "+var+" (cm)";

   TFile* f1 = TFile::Open("histograms.root", "READ");
   TFile* f2 = TFile::Open("histogramscaled.root", "READ");
   TProfile *h0 = (TProfile *)f1->Get(histqcd.c_str());
   TProfile *h1 = (TProfile *)f1->Get((histname+"B").c_str());
   TProfile *h2 = (TProfile *)f2->Get(histqcd.c_str());

   TCanvas * c1 =  new TCanvas("c1", "c1", 900, 900);
   TCanvas * c2 =  new TCanvas("c2", "c2", 900, 900);

   c1->cd();

   h0->SetStats(0);
   h0->Draw("ep");
   h1->Draw("ep same");
   h2->Draw("ep same");

   auto legend = new TLegend(0.6,0.6,0.85,0.85);
   legend->AddEntry(h0,"2017 MC","epl");
   legend->AddEntry(h1,"2017B Data","epl");
   legend->AddEntry(h2,"2017 MC scaled","epl");
   legend->Draw();

   if(tag=="l") {
     //h0->SetMinimum(0.001);
     h0->SetMaximum(1.5*h0->GetBinContent(1));
   } else if(tag=="b") {
     //h0->SetMinimum(0.0055);
     h0->SetMaximum(2.2*h0->GetBinContent(1));
   }
   h0->SetLineColor(kBlack);
   h0->SetLineWidth(2);
   h0->SetMarkerColor(kBlack);
   h0->SetMarkerSize(0.5);
   h0->SetMarkerStyle(21);
   h0->GetYaxis()->SetTitle(yname.c_str());
   h0->SetTitle("");
   h0->GetXaxis()->SetTitle("pT (GeV)");
   h0->GetXaxis()->SetRangeUser(0,200);
   h1->SetLineColor(kRed);
   h1->SetLineWidth(2);
   h1->SetMarkerColor(kRed);
   h1->SetMarkerSize(0.5);
   h1->SetMarkerStyle(21);
   h2->SetLineColor(kGreen);
   h2->SetLineWidth(2);
   h2->SetMarkerColor(kGreen);
   h2->SetMarkerSize(0.5);
   h2->SetMarkerStyle(21);

   c1->SaveAs(("scale_"+histname+".pdf").c_str());


   TLine *line = new TLine(h1->GetXaxis()->GetXmin(), 1, h1->GetXaxis()->GetXmax(), 1);
   line->SetLineColor(kRed);

   c2->cd();          // Go back to the main canvas before defining pad2

   TH1D *r1 = (TH1D*)h1->ProjectionX("h1", "E")->Clone("r1");
   TH1D *r2 = (TH1D*)h1->ProjectionX("h1", "E")->Clone("r2");
/*
   r1->Rebin(5);
   r2->Rebin(5);
   r3->Rebin(5);
   r4->Rebin(5);
   r5->Rebin(5);
   h0->Rebin(5);
*/
   r1->Sumw2();
   r2->Sumw2();

   r1->Divide(h0);
   r2->Divide(h2);

   r1->Draw("ep");
   r2->Draw("ep same");
   line->Draw("same");

   r1->SetLineColor(kRed);
   r1->SetLineWidth(2);
   r1->SetMarkerColor(kRed);
   r1->SetMarkerSize(0.5);
   r1->SetMarkerStyle(21);
   r1->SetMinimum(0.4);
   r1->SetMaximum(1.6);
   r1->SetTitle("");
   r1->GetYaxis()->SetTitle(("Mean "+var+" Data/MC ratio").c_str());
   r1->GetXaxis()->SetTitle("pT (GeV)");
   r1->GetXaxis()->SetRangeUser(0,200);

   r2->SetLineColor(kGreen);
   r2->SetLineWidth(2);
   r2->SetMarkerColor(kGreen);
   r2->SetMarkerSize(0.5);
   r2->SetMarkerStyle(21);

   auto legend2 = new TLegend(0.2,0.2,0.45,0.45);
   legend2->AddEntry(r1,"Data/MC","epl");
   legend2->AddEntry(r2,"Data/MC scaled","epl");
   legend2->Draw();

   for(int ii=0; ii<r1->GetNbinsX(); ii++)
     std::cout << r1->GetBinContent(ii+1) << ", ";
   std::cout << std::endl;

   bool dofit = false;
   if(dofit){
     float N=3.5, bias=0;
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

   c2->SaveAs(("ratio_scale_"+histname+".pdf").c_str());

}




