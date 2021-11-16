void IP2Derr(int scale=1){

string inputnames[5] = {"QCD_HT1500to2000", "JetHT_UL2017B", "JetHT_Run2017B", "JetHT_UL2018A", "JetHT_Run2018A"};

for(int ii=0; ii<5; ii++){
  TChain *ch = new TChain("emj/seltrktree");
  ch->Add(("./data/"+inputnames[ii]+".root").c_str());

  double seltrk_ip2d, pt, eta, phi;
  double seltrk_ip2dsig;

  ch->SetBranchAddress("seltrk_ip2d", &seltrk_ip2d);
  ch->SetBranchAddress("seltrk_ip2dsig", &seltrk_ip2dsig);
  ch->SetBranchAddress("seltrk_pt", &pt);
  ch->SetBranchAddress("seltrk_eta", &eta);
  ch->SetBranchAddress("seltrk_phi", &phi);

  TFile *f = new TFile(("IP2Derrprofile_"+inputnames[ii]+"_seltrk.root").c_str(), "recreate");

  TH3D* h_eta_pt_ip2derr = new TH3D("h_eta_pt_ip2derr", "h_eta_pt_ip2derr; #eta; PT (GeV); IP2Derr (cm)", 60, -3, 3, 1900, 1, 20, 400, -2, 2);
  TH3D* h_phi_pt_ip2derr = new TH3D("h_phi_pt_ip2derr", "h_phi_pt_ip2derr; #phi; PT (GeV); IP2Derr (cm)", 64, -3.2, 3.2, 1900, 1, 20, 400, -2, 2);

  TH2D* h_ip2derr_pt  = new TH2D("h_ip2derr_pt", "h_ip2derr_pt; IP2Derr (cm); PT (GeV)", 400, -2, 2, 1900, 1, 20);
  TH2D* h_ip2derr_eta = new TH2D("h_ip2derr_eta", "h_ip2derr_eta; IP2Derr (cm); #eta", 400, -2, 2, 60, -3, 3);
  TH2D* h_ip2derr_phi = new TH2D("h_ip2derr_phi", "h_ip2derr_phi; IP2Derr (cm); #phi", 400, -2, 2, 64, -3.2, 3.2);

  TH2D* h_ip2derr_eta_ptcut3to4GeV = new TH2D("h_ip2derr_eta_ptcut3to4GeV", "h_ip2derr_eta_ptcut3to4GeV; IP2Derr (cm); #eta", 400, -2, 2, 60, -3, 3);
  TH2D* h_ip2derr_phi_ptcut3to4GeV = new TH2D("h_ip2derr_phi_ptcut3to4GeV", "h_ip2derr_phi_ptcut3to4GeV; IP2Derr (cm); #phi", 400, -2, 2, 64, -3.2, 3.2);
  TH2D* h_ip2derr_eta_ptcut14to15GeV = new TH2D("h_ip2derr_eta_ptcut14to15GeV", "h_ip2derr_eta_ptcut14to15GeV; IP2Derr (cm); #eta", 400, -2, 2, 60, -3, 3);
  TH2D* h_ip2derr_phi_ptcut14to15GeV = new TH2D("h_ip2derr_phi_ptcut14to15GeV", "h_ip2derr_phi_ptcut14to15GeV; IP2Derr (cm); #phi", 400, -2, 2, 64, -3.2, 3.2);

  for(int i=0; i<ch->GetEntries()/scale; i++){
    if(i%10000000==0) std::cout << (float) i*100./(float) ch->GetEntries() << "%% processed" << std::endl;
    ch->GetEntry(i);
    if( abs(seltrk_ip2d)>0.05 ){
      h_eta_pt_ip2derr->Fill(eta, pt, seltrk_ip2dsig==0? 0 : seltrk_ip2d/seltrk_ip2dsig);
      h_phi_pt_ip2derr->Fill(phi, pt, seltrk_ip2dsig==0? 0 : seltrk_ip2d/seltrk_ip2dsig);

      h_ip2derr_pt->Fill(seltrk_ip2dsig==0? 0 : seltrk_ip2d/seltrk_ip2dsig, pt);
      h_ip2derr_eta->Fill(seltrk_ip2dsig==0? 0 : seltrk_ip2d/seltrk_ip2dsig, eta);
      h_ip2derr_phi->Fill(seltrk_ip2dsig==0? 0 : seltrk_ip2d/seltrk_ip2dsig, phi);
      if(pt>3. && pt<4.) {
        h_ip2derr_eta_ptcut3to4GeV->Fill(seltrk_ip2dsig==0? 0 : seltrk_ip2d/seltrk_ip2dsig, eta);
        h_ip2derr_phi_ptcut3to4GeV->Fill(seltrk_ip2dsig==0? 0 : seltrk_ip2d/seltrk_ip2dsig, phi);
      }
      if(pt>14. && pt<15.) {
        h_ip2derr_eta_ptcut14to15GeV->Fill(seltrk_ip2dsig==0? 0 : seltrk_ip2d/seltrk_ip2dsig, eta);
        h_ip2derr_phi_ptcut14to15GeV->Fill(seltrk_ip2dsig==0? 0 : seltrk_ip2d/seltrk_ip2dsig, phi);
      }
    }
  }

  f->cd();
  h_eta_pt_ip2derr->Write();
  h_phi_pt_ip2derr->Write();
  h_ip2derr_pt->Write();
  h_ip2derr_eta->Write();
  h_ip2derr_phi->Write();
  h_ip2derr_eta_ptcut3to4GeV->Write();
  h_ip2derr_phi_ptcut3to4GeV->Write();
  h_ip2derr_eta_ptcut14to15GeV->Write();
  h_ip2derr_phi_ptcut14to15GeV->Write();
  f->Close();

}

}



