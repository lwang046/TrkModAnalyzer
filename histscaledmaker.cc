void histscaledmaker(string inputname, string outputname){
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);


  float sf_ll_ip2d[40] = {0, 0.954437, 0.982722, 0.976605, 0.977314, 0.991134, 0.978813, 0.949165, 0.949382, 0.92692, 0.939051, 0.9371, 0.92061, 0.942483, 0.870573, 0.907867, 0.861894, 0.907211, 0.872308, 0.86322, 0.865968, 0.842095, 0.890404, 0.768298, 0.804297, 0.892434, 0.980858, 0.805239, 0.976068, 0.927401, 0.896811, 0.880994, 1.06216, 0.686474, 1.30167, 1.36107, 1.33748, 0.942785, 1};
  float sf_ll_pvdz[40] = {0, 1.00053, 0.996661, 1.00191, 1.00313, 1.01406, 1.02682, 1.01496, 1.0096, 1.01751, 1.04389, 1.01441, 0.997571, 1.02404, 1.01631, 1.03158, 1.0222, 1.02707, 1.01597, 0.998612, 1.0294, 0.986171, 0.93551, 1.00184, 0.966653, 0.959335, 0.875512, 0.974555, 0.868157, 0.929308, 0.864414, 1.02654, 1.18758, 0.980267, 0.9385, 1.01915, 1.03526, 1.16774, 1};
  float sf_lh_ip2d[40] = {0, 0.758381, 0.916829, 0.929424, 0.932543, 0.93212, 0.932828, 0.937354, 0.930218, 0.93758, 0.934587, 0.930322, 0.929186, 0.928835, 0.928469, 0.922895, 0.914194, 0.911597, 0.897108, 0.885367, 0.904942, 0.896473, 0.902294, 0.920599, 0.909911, 0.885635, 0.925279, 0.969393, 1.01422, 1.00975, 0.983354, 1.07741, 1.02189, 0.95423, 0.974223, 1.02011, 1.11633, 1.06614, 1};
  float sf_lh_pvdz[40] = {0, 1.02841, 1.01274, 1.00178, 0.990667, 0.988653, 0.989245, 0.996869, 0.986228, 1.00673, 1.00609, 1.00404, 1.0151, 1.0088, 1.00533, 1.00584, 1.01882, 0.999982, 0.996981, 0.977961, 1.00091, 0.980627, 0.978314, 0.978449, 0.979661, 0.907885, 0.95006, 0.900013, 0.917037, 0.926044, 0.8841, 0.957713, 0.898902, 0.98874, 0.921616, 1.01968, 0.90432, 0.889422, 1};


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
    int nn = (pt>200)? 40 : (pt<20? (int) pt/1 : 20 + (int) (pt-20)/10);
    if(btag<0.1){
      if(fabs(eta<1.5)){
        h_ljet_heta_ip2d->Fill(pt, seltrk_ip2d*sf_ll_ip2d[nn]);
        h_ljet_heta_pvdz->Fill(pt, fabs(seltrk_pvdz*sf_ll_pvdz[nn]));
      } else if(fabs(eta)>1.5) {
        h_ljet_leta_ip2d->Fill(pt, seltrk_ip2d*sf_lh_ip2d[nn]);
        h_ljet_leta_pvdz->Fill(pt, fabs(seltrk_pvdz*sf_lh_pvdz[nn]));
      }
    } else if(btag>0.4506) {
      if(fabs(eta<1.5)){
        h_bjet_heta_ip2d->Fill(pt, seltrk_ip2d*sf_ll_ip2d[nn]);
        h_bjet_heta_pvdz->Fill(pt, fabs(seltrk_pvdz*sf_ll_pvdz[nn]));
      } else if(fabs(eta)>1.5) {
        h_bjet_leta_ip2d->Fill(pt, seltrk_ip2d*sf_lh_ip2d[nn]);
        h_bjet_leta_pvdz->Fill(pt, fabs(seltrk_pvdz*sf_lh_pvdz[nn]));
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



