void makescaledcanv(string tag, string region, string var, string dataset, string era){
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);

   string histname = "p_"+tag+"jet_"+region+"eta_"+var+"_"+dataset+"_"+era;
   string histqcd = "p_"+tag+"jet_"+region+"eta_"+var+"_QCD";
   string yname = "mean tracks "+var+" (cm)";

   TFile* f1 = TFile::Open("histogram_PTprofile.root", "READ");
   TFile* f2 = TFile::Open("histogram_PTscaled.root", "READ");
   TProfile *h0 = (TProfile *)f1->Get((histname+"B").c_str());
   TProfile *h1 = (TProfile *)f1->Get(histqcd.c_str());
   TProfile *h2 = (TProfile *)f2->Get(histqcd.c_str());

   TCanvas * c1 =  new TCanvas("c1", "c1", 800, 800);

   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   pad1->SetBottomMargin(0); // Upper and lower plot are joined
   pad1->SetGridx();         // Vertical grid
   pad1->Draw();             // Draw the upper pad: pad1
   pad1->cd();               // pad1 becomes the current pad  

   h0->SetStats(0);
   h0->Draw("ep");
   h1->Draw("ep same");
   h2->Draw("ep same");

   auto legend = new TLegend(0.6,0.6,0.85,0.85);
   //auto legend = new TLegend(0.5,0.2,0.7,0.5);
   legend->AddEntry(h0,"2017B Data","epl");
   legend->AddEntry(h1,"2017 MC","epl");
   legend->AddEntry(h2,"2017 MC Scaled","epl");
   legend->Draw();

   if(tag=="l") {
     h0->SetMaximum(1.5*h0->GetBinContent(51));
   } else if(tag=="b") {
     h0->SetMaximum(2.2*h0->GetBinContent(51));
   }
   h0->SetLineColor(kBlack);
   h0->SetLineWidth(2);
   h0->SetMarkerColor(kBlack);
   h0->SetMarkerSize(0.5);
   h0->SetMarkerStyle(21);
   h0->GetYaxis()->SetTitle(yname.c_str());
   h0->SetTitle("");
   h0->GetXaxis()->SetTitle("PT (GeV)");
   h1->SetLineColor(kBlue);
   h1->SetLineWidth(2);
   h1->SetMarkerColor(kBlue);
   h1->SetMarkerSize(0.5);
   h1->SetMarkerStyle(21);
   h2->SetLineColor(kGreen);
   h2->SetLineWidth(2);
   h2->SetMarkerColor(kGreen);
   h2->SetMarkerSize(0.5);
   h2->SetMarkerStyle(21);


   c1->cd();          // Go back to the main canvas before defining pad2
   TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
   pad2->SetTopMargin(0);
   pad2->SetBottomMargin(0.2);
   pad2->SetGridx(); // vertical grid
   pad2->Draw();
   pad2->cd();       // pad2 becomes the current pad

   TLine *line = new TLine(h1->GetXaxis()->GetXmin(), 1, h1->GetXaxis()->GetXmax(), 1);
   line->SetLineColor(kRed);

   TH1D *r1 = (TH1D*)h0->ProjectionX("h0", "E")->Clone("r1");
   TH1D *r2 = (TH1D*)h0->ProjectionX("h0", "E")->Clone("r2");

   r1->Sumw2();
   r2->Sumw2();

   r1->Divide(h1);
   r2->Divide(h2);

   r1->Draw("ep");
   r2->Draw("ep same");
   line->Draw("same");

   r1->SetLineColor(kBlue);
   r1->SetLineWidth(2);
   r1->SetMarkerColor(kBlue);
   r1->SetMarkerSize(0.5);
   r1->SetMarkerStyle(21);
   r1->SetMinimum(0.4);
   r1->SetMaximum(1.6);
   r1->SetTitle("");
   r1->GetYaxis()->SetTitle(("Mean "+var+" Data/MC ratio").c_str());
   r1->GetXaxis()->SetTitle("PT (GeV)");
   r1->GetXaxis()->SetTitleSize(0.1);
   r1->GetXaxis()->SetLabelSize(0.1);
   r1->GetYaxis()->SetTitleSize(0.1);
   r1->GetYaxis()->SetLabelSize(0.1);
   r1->GetYaxis()->SetTitleOffset(0.37);

   r2->SetLineColor(kGreen);
   r2->SetLineWidth(2);
   r2->SetMarkerColor(kGreen);
   r2->SetMarkerSize(0.5);
   r2->SetMarkerStyle(21);

   c1->SaveAs(("scaled_"+histname+".pdf").c_str());

}




