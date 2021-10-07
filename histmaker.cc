void histmaker(string inputname, string outputname){
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  TChain *ch = new TChain("emj/seltrktree");
  ch->Add((inputname+".root").c_str());

  double seltrk_ip2d, seltrk_pvdz, btag, pt, eta, phi;
  ROOT::Math::XYZPoint *PVposition=0;

  ch->SetBranchAddress("seltrk_ip2d", &seltrk_ip2d);
  ch->SetBranchAddress("seltrk_pvdz", &seltrk_pvdz);
  ch->SetBranchAddress("btagscore2", &btag);
  ch->SetBranchAddress("seltrk_pt", &pt);
  ch->SetBranchAddress("seltrk_eta", &eta);
  ch->SetBranchAddress("seltrk_phi", &phi);
  ch->SetBranchAddress("PVposition", &PVposition);

  TFile *f = new TFile((outputname+".root").c_str(), "update");

  double xbins1[20], xbins2[18];
  for(int ii=0; ii<20; ii++){
    xbins1[ii] = 1.*(double) ii;
    if(ii<18) xbins2[ii] = 20.+ 10.*(double) ii;
  }
  double * xbins = new double[39];
  std::copy(xbins1, xbins1+20, xbins);
  std::copy(xbins2, xbins2+18, xbins+20);
  xbins[38] = 200.;

  TH2F* h_ljet_heta_ip2d = new TH2F(("h_ljet_heta_ip2d_"+inputname).c_str(), "h_ljet_heta_ip2d; PT (GeV); IP2D (cm)", 38, xbins, 2000, 0, 10.0);
  TH2F* h_ljet_heta_pvdz = new TH2F(("h_ljet_heta_pvdz_"+inputname).c_str(), "h_ljet_heta_pvdz; PT (GeV); PVdz (cm)", 38, xbins, 1000, 0, 5.0);
  TH2F* h_ljet_leta_ip2d = new TH2F(("h_ljet_leta_ip2d_"+inputname).c_str(), "h_ljet_leta_ip2d; PT (GeV); IP2D (cm)", 38, xbins, 2000, 0, 10.0);
  TH2F* h_ljet_leta_pvdz = new TH2F(("h_ljet_leta_pvdz_"+inputname).c_str(), "h_ljet_leta_pvdz; PT (GeV); PVdz (cm)", 38, xbins, 1000, 0, 5.0);

  TH2F* h_bjet_heta_ip2d = new TH2F(("h_bjet_heta_ip2d_"+inputname).c_str(), "h_bjet_heta_ip2d; PT (GeV); IP2D (cm)", 38, xbins, 2000, 0, 10.0);
  TH2F* h_bjet_heta_pvdz = new TH2F(("h_bjet_heta_pvdz_"+inputname).c_str(), "h_bjet_heta_pvdz; PT (GeV); PVdz (cm)", 38, xbins, 1000, 0, 5.0);
  TH2F* h_bjet_leta_ip2d = new TH2F(("h_bjet_leta_ip2d_"+inputname).c_str(), "h_bjet_leta_ip2d; PT (GeV); IP2D (cm)", 38, xbins, 2000, 0, 10.0);
  TH2F* h_bjet_leta_pvdz = new TH2F(("h_bjet_leta_pvdz_"+inputname).c_str(), "h_bjet_leta_pvdz; PT (GeV); PVdz (cm)", 38, xbins, 1000, 0, 5.0);

  for(int i=0; i<ch->GetEntries(); i++){
    if(i%10000000==0) std::cout << (float) i*100./(float) ch->GetEntries() << "%% processed" << std::endl;
    ch->GetEntry(i);
    if(btag<0.1){
      if(fabs(eta<1.5)){
        h_ljet_heta_ip2d->Fill(pt, seltrk_ip2d);
        h_ljet_heta_pvdz->Fill(pt, fabs(seltrk_pvdz));
      } else if(fabs(eta)>1.5) {
        h_ljet_leta_ip2d->Fill(pt, seltrk_ip2d);
        h_ljet_leta_pvdz->Fill(pt, fabs(seltrk_pvdz));
      }
    } else if(btag>0.4506) {
      if(fabs(eta<1.5)){
        h_bjet_heta_ip2d->Fill(pt, seltrk_ip2d);
        h_bjet_heta_pvdz->Fill(pt, fabs(seltrk_pvdz));
      } else if(fabs(eta)>1.5) {
        h_bjet_leta_ip2d->Fill(pt, seltrk_ip2d);
        h_bjet_leta_pvdz->Fill(pt, fabs(seltrk_pvdz));
      }
    }
  }


  TProfile *p_ljet_heta_pvdz = h_ljet_heta_pvdz->ProfileX(("p_ljet_heta_pvdz_"+inputname).c_str());
  TProfile *p_ljet_heta_ip2d = h_ljet_heta_ip2d->ProfileX(("p_ljet_heta_ip2d_"+inputname).c_str());
  TProfile *p_ljet_leta_pvdz = h_ljet_leta_pvdz->ProfileX(("p_ljet_leta_pvdz_"+inputname).c_str());
  TProfile *p_ljet_leta_ip2d = h_ljet_leta_ip2d->ProfileX(("p_ljet_leta_ip2d_"+inputname).c_str());

  TProfile *p_bjet_heta_ip2d = h_bjet_heta_ip2d->ProfileX(("p_bjet_heta_ip2d_"+inputname).c_str());
  TProfile *p_bjet_heta_pvdz = h_bjet_heta_pvdz->ProfileX(("p_bjet_heta_pvdz_"+inputname).c_str());
  TProfile *p_bjet_leta_ip2d = h_bjet_leta_ip2d->ProfileX(("p_bjet_leta_ip2d_"+inputname).c_str());
  TProfile *p_bjet_leta_pvdz = h_bjet_leta_pvdz->ProfileX(("p_bjet_leta_pvdz_"+inputname).c_str());


  f->cd();
  h_ljet_heta_ip2d->Write();
  h_ljet_heta_pvdz->Write();
  h_ljet_leta_ip2d->Write();
  h_ljet_leta_pvdz->Write();

  h_bjet_heta_ip2d->Write();
  h_bjet_heta_pvdz->Write();
  h_bjet_leta_ip2d->Write();
  h_bjet_leta_pvdz->Write();

  p_ljet_heta_ip2d->Write();
  p_ljet_heta_pvdz->Write();
  p_ljet_leta_ip2d->Write();
  p_ljet_leta_pvdz->Write();

  p_bjet_heta_ip2d->Write();
  p_bjet_heta_pvdz->Write();
  p_bjet_leta_ip2d->Write();
  p_bjet_leta_pvdz->Write();
  f->Close();

}



