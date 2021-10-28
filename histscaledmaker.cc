void histscaledmaker(string inputname, string outputname){
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);


  float sf_ll_ip2d[39] = {0, 0.783033, 0.78844, 0.781401, 0.774014, 0.768408, 0.768686, 0.769525, 0.769764, 0.765762, 0.776978, 0.773348, 0.759998, 0.771366, 0.746096, 0.770318, 0.760393, 0.78256, 0.764118, 0.728642, 0.772433, 0.79272, 0.809415, 0.87715, 0.825473, 0.779437, 0.884148, 0.843282, 0.943304, 0.909999, 0.954674, 0.938259, 0.965419, 0.885519, 0.954632, 0.99997, 1.12014, 0.977807, 1};
  float sf_ll_pvdz[39] = {0, 1.01268, 0.941401, 0.882669, 0.848168, 0.830623, 0.821389, 0.817061, 0.81639, 0.821753, 0.812589, 0.810235, 0.815958, 0.822574, 0.817855, 0.821355, 0.837089, 0.814095, 0.829501, 0.814742, 0.839888, 0.840821, 0.864915, 0.877314, 0.881295, 0.882517, 0.889966, 0.851649, 0.86642, 0.892626, 0.883447, 0.933419, 0.929273, 0.880545, 0.888667, 0.976625, 0.954045, 0.910686, 1};
  float sf_lh_ip2d[39] = {0, 0.88693, 0.885873, 0.875067, 0.864609, 0.857571, 0.834686, 0.820467, 0.817331, 0.790731, 0.818261, 0.812654, 0.808739, 0.791168, 0.801192, 0.804791, 0.808047, 0.781703, 0.762795, 0.769264, 0.80157, 0.826612, 0.884756, 0.8387, 0.944927, 0.955762, 1.02813, 0.906709, 1.03037, 1.43421, 1.11948, 1.34619, 0.978969, 0.940722, 0.934054, 0.933185, 1.29342, 1.11327, 1};
  float sf_lh_pvdz[39] = {0, 1.01157, 0.973793, 0.942485, 0.935483, 0.91215, 0.904211, 0.884566, 0.881351, 0.861405, 0.88373, 0.878305, 0.866804, 0.866694, 0.873197, 0.874956, 0.878857, 0.901156, 0.868866, 0.842864, 0.884367, 0.883609, 0.913431, 0.916929, 0.888298, 0.884338, 0.935265, 0.818815, 0.881458, 1.11405, 0.983512, 1.10467, 0.973075, 0.975878, 1.00165, 0.89436, 0.986316, 1.38933, 1};


  TChain *ch = new TChain("emj/seltrktree");
  ch->Add(("data/"+inputname+".root").c_str());

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

  TH2F* h_ljet_heta_ip2d = new TH2F(("h_ljet_heta_ip2d_"+inputname).c_str(), "h_ljet_heta_ip2d; PT (GeV); IP2D (cm)", 38, xbins, 20000, 0.005, 100.005);
  TH2F* h_ljet_heta_pvdz = new TH2F(("h_ljet_heta_pvdz_"+inputname).c_str(), "h_ljet_heta_pvdz; PT (GeV); PVdz (cm)", 38, xbins, 2000, 0.005, 10.005);
  TH2F* h_ljet_leta_ip2d = new TH2F(("h_ljet_leta_ip2d_"+inputname).c_str(), "h_ljet_leta_ip2d; PT (GeV); IP2D (cm)", 38, xbins, 20000, 0.005, 100.005);
  TH2F* h_ljet_leta_pvdz = new TH2F(("h_ljet_leta_pvdz_"+inputname).c_str(), "h_ljet_leta_pvdz; PT (GeV); PVdz (cm)", 38, xbins, 2000, 0.005, 10.005);

  TH2F* h_bjet_heta_ip2d = new TH2F(("h_bjet_heta_ip2d_"+inputname).c_str(), "h_bjet_heta_ip2d; PT (GeV); IP2D (cm)", 38, xbins, 20000, 0.005, 100.005);
  TH2F* h_bjet_heta_pvdz = new TH2F(("h_bjet_heta_pvdz_"+inputname).c_str(), "h_bjet_heta_pvdz; PT (GeV); PVdz (cm)", 38, xbins, 2000, 0.005, 10.005);
  TH2F* h_bjet_leta_ip2d = new TH2F(("h_bjet_leta_ip2d_"+inputname).c_str(), "h_bjet_leta_ip2d; PT (GeV); IP2D (cm)", 38, xbins, 20000, 0.005, 100.005);
  TH2F* h_bjet_leta_pvdz = new TH2F(("h_bjet_leta_pvdz_"+inputname).c_str(), "h_bjet_leta_pvdz; PT (GeV); PVdz (cm)", 38, xbins, 2000, 0.005, 10.005);

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



