void IP2Dprofile(int scale=1){

string inputnames[5] = {"QCD_HT1500to2000", "JetHT_UL2017B", "JetHT_Run2017B", "JetHT_UL2018A", "JetHT_Run2018A"};

for(int ii=0; ii<5; ii++){
  TChain *ch = new TChain("emj/seltrktree");
  ch->Add(("./data/"+inputnames[ii]+".root").c_str());

  double seltrk_ip3d, pt, eta, phi;

  ch->SetBranchAddress("seltrk_ip3d", &seltrk_ip3d);
  ch->SetBranchAddress("seltrk_pt", &pt);
  ch->SetBranchAddress("seltrk_eta", &eta);
  ch->SetBranchAddress("seltrk_phi", &phi);

  TFile *f = new TFile(("IP3Dprofile_"+inputnames[ii]+"_seltrk.root").c_str(), "recreate");

  TH3D* h_eta_pt_ip3d = new TH3D("h_eta_pt_ip3d", "h_eta_pt_ip3d; #eta; PT (GeV); IP3D (cm)", 60, -3, 3, 1900, 1, 20, 400, -2, 2);
  TH3D* h_phi_pt_ip3d = new TH3D("h_phi_pt_ip3d", "h_phi_pt_ip3d; #phi; PT (GeV); IP3D (cm)", 64, -3.2, 3.2, 1900, 1, 20, 400, -2, 2);

  TH2D* h_ip3d_pt  = new TH2D("h_ip3d_pt", "h_ip3d_pt; IP3D (cm); PT (GeV)", 400, -2, 2, 1900, 1, 20);
  TH2D* h_ip3d_eta = new TH2D("h_ip3d_eta", "h_ip3d_eta; IP3D (cm); #eta", 400, -2, 2, 60, -3, 3);
  TH2D* h_ip3d_phi = new TH2D("h_ip3d_phi", "h_ip3d_phi; IP3D (cm); #phi", 400, -2, 2, 64, -3.2, 3.2);

  TH2D* h_ip3d_eta_ptcut3to4GeV = new TH2D("h_ip3d_eta_ptcut3to4GeV", "h_ip3d_eta_ptcut3to4GeV; IP3D (cm); #eta", 400, -2, 2, 60, -3, 3);
  TH2D* h_ip3d_phi_ptcut3to4GeV = new TH2D("h_ip3d_phi_ptcut3to4GeV", "h_ip3d_phi_ptcut3to4GeV; IP3D (cm); #phi", 400, -2, 2, 64, -3.2, 3.2);
  TH2D* h_ip3d_eta_ptcut14to15GeV = new TH2D("h_ip3d_eta_ptcut14to15GeV", "h_ip3d_eta_ptcut14to15GeV; IP3D (cm); #eta", 400, -2, 2, 60, -3, 3);
  TH2D* h_ip3d_phi_ptcut14to15GeV = new TH2D("h_ip3d_phi_ptcut14to15GeV", "h_ip3d_phi_ptcut14to15GeV; IP3D (cm); #phi", 400, -2, 2, 64, -3.2, 3.2);

  for(int i=0; i<ch->GetEntries()/scale; i++){
    if(i%10000000==0) std::cout << (float) i*100./(float) ch->GetEntries() << "%% processed" << std::endl;
    ch->GetEntry(i);
    if( abs(seltrk_ip3d)>0.05 ){
      h_eta_pt_ip3d->Fill(eta, pt, seltrk_ip3d);
      h_phi_pt_ip3d->Fill(phi, pt, seltrk_ip3d);

      h_ip3d_pt->Fill(seltrk_ip3d, pt);
      h_ip3d_eta->Fill(seltrk_ip3d, eta);
      h_ip3d_phi->Fill(seltrk_ip3d, phi);
      if(pt>3. && pt<4.) {
        h_ip3d_eta_ptcut3to4GeV->Fill(seltrk_ip3d, eta);
        h_ip3d_phi_ptcut3to4GeV->Fill(seltrk_ip3d, phi);
      }
      if(pt>14. && pt<15.) {
        h_ip3d_eta_ptcut14to15GeV->Fill(seltrk_ip3d, eta);
        h_ip3d_phi_ptcut14to15GeV->Fill(seltrk_ip3d, phi);
      }
    }
  }

  f->cd();
  h_eta_pt_ip3d->Write();
  h_phi_pt_ip3d->Write();
  h_ip3d_pt->Write();
  h_ip3d_eta->Write();
  h_ip3d_phi->Write();
  h_ip3d_eta_ptcut3to4GeV->Write();
  h_ip3d_phi_ptcut3to4GeV->Write();
  h_ip3d_eta_ptcut14to15GeV->Write();
  h_ip3d_phi_ptcut14to15GeV->Write();
  f->Close();

}

}



