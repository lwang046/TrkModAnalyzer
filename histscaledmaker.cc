void histscaledmaker(string inputname, string outputname){
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);


  float sf_ll_ip2d[39] = {0, 0.75635, 0.899452, 0.91121, 0.914313, 0.914899, 0.918594, 0.922696, 0.913374, 0.92588, 0.918731, 0.915605, 0.913777, 0.917256, 0.912404, 0.910011, 0.901569, 0.898286, 0.876294, 0.871656, 0.889072, 0.88905, 0.893545, 0.909068, 0.890361, 0.852013, 0.92251, 0.944651, 1.035, 0.975302, 0.981933, 1.01023, 0.970928, 0.954539, 0.952499, 1.00964, 1.0735, 1.03284, 1};
  float sf_ll_pvdz[39] = {0, 1.02881, 1.00913, 0.992666, 0.976358, 0.972267, 0.971678, 0.980371, 0.969241, 0.988535, 0.98382, 0.987006, 0.997779, 0.988694, 0.985275, 0.989192, 1.00214, 0.981479, 0.978485, 0.960685, 0.982452, 0.969375, 0.968902, 0.966227, 0.968719, 0.90385, 0.942818, 0.89964, 0.905451, 0.905396, 0.877346, 0.952003, 0.89736, 0.997747, 0.913721, 1.02794, 0.89459, 0.89904, 1};
  float sf_lh_ip2d[39] = {0, 0.956462, 0.977933, 0.979641, 0.980618, 0.982436, 0.965408, 0.953701, 0.957891, 0.928394, 0.948827, 0.939194, 0.936229, 0.926295, 0.902966, 0.912361, 0.881997, 0.905264, 0.910218, 0.867692, 0.889148, 0.832948, 0.862886, 0.815717, 0.855832, 0.942644, 0.902418, 0.886231, 0.861544, 1.02603, 0.890402, 1.14963, 1.13978, 0.768123, 1.15071, 1.14159, 1.30468, 1.0574, 1};
  float sf_lh_pvdz[39] = {0, 1.00075, 0.99615, 1.00302, 1.00863, 1.01242, 1.01836, 1.00781, 1.00103, 1.01549, 1.04466, 1.00858, 1.00581, 1.02252, 1.01662, 1.01654, 1.0184, 1.01817, 1.01202, 0.990164, 1.02617, 0.972871, 0.943999, 0.993177, 0.97164, 0.905459, 0.903232, 0.890585, 0.907539, 0.970238, 0.87184, 0.979748, 0.997869, 0.907125, 0.942051, 0.953737, 0.958067, 0.950983, 1};


  TChain *ch = new TChain("emj/seltrktree");
  ch->Add((inputname+".root").c_str());

  double seltrk_ip2d, seltrk_pvdz, btag, pt, eta;

  ch->SetBranchAddress("seltrk_ip2d", &seltrk_ip2d);
  ch->SetBranchAddress("seltrk_pvdz", &seltrk_pvdz);
  ch->SetBranchAddress("btagscore2", &btag);
  ch->SetBranchAddress("seltrk_pt", &pt);
  ch->SetBranchAddress("seltrk_eta", &eta);

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
    int nn = (pt>200)? 38 : (pt<20? (int) pt/1 : 20 + (int) (pt-20)/10);
    if(btag<0.1){
      if(fabs(eta)>1.5){
        h_ljet_heta_ip2d->Fill(pt, seltrk_ip2d*sf_lh_ip2d[nn]);
        h_ljet_heta_pvdz->Fill(pt, fabs(seltrk_pvdz*sf_lh_pvdz[nn]));
      } else if(fabs(eta)<1.5) {
        h_ljet_leta_ip2d->Fill(pt, seltrk_ip2d*sf_ll_ip2d[nn]);
        h_ljet_leta_pvdz->Fill(pt, fabs(seltrk_pvdz*sf_ll_pvdz[nn]));
      }
    } else if(btag>0.4506) {
      if(fabs(eta)>1.5){
        h_bjet_heta_ip2d->Fill(pt, seltrk_ip2d*sf_lh_ip2d[nn]);
        h_bjet_heta_pvdz->Fill(pt, fabs(seltrk_pvdz*sf_lh_pvdz[nn]));
      } else if(fabs(eta)<1.5) {
        h_bjet_leta_ip2d->Fill(pt, seltrk_ip2d*sf_ll_ip2d[nn]);
        h_bjet_leta_pvdz->Fill(pt, fabs(seltrk_pvdz*sf_ll_pvdz[nn]));
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



