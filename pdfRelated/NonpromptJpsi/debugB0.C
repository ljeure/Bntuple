
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TMath.h>
#include <iostream>
#include <TNtuple.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <cmath>
#include "debugB0.h"

#define MUON_MASS   0.10565837
#define PION_MASS   0.13957018
#define KAON_MASS   0.493677
#define KSHORT_MASS 0.497614
#define KSTAR_MASS  0.89594
#define PHI_MASS    1.019455
#define JPSI_MASS   3.096916

void fillTree(TVector3* bP, TVector3* bVtx, TLorentzVector* b4P, int j, int typesize, float track_mass1, float track_mass2, int REAL)
{

  //Event Info
  Event = EvtInfo_EvtNo;
  Run = EvtInfo_RunNo;
  size = typesize+1;
  besttktkmass = 0;
  bestchi2 = 0;

  // Trigger Info
  HLT_PAL1DoubleMu0_v1 = Bfr_HLT_PAL1DoubleMu0_v1;
  HLT_PAL1DoubleMu0_v1_Prescl = Bfr_HLT_PAL1DoubleMu0_v1_Prescl;
  HLT_PADimuon0_NoVertexing_v1 = Bfr_HLT_PADimuon0_NoVertexing_v1;
  HLT_PADimuon0_NoVertexing_v1_Prescl = Bfr_HLT_PADimuon0_NoVertexing_v1_Prescl;
  HLT_PAL1DoubleMu0_HighQ_v1 = Bfr_HLT_PAL1DoubleMu0_HighQ_v1;
  HLT_PAL1DoubleMu0_HighQ_v1_Prescl = Bfr_HLT_PAL1DoubleMu0_HighQ_v1_Prescl;
  HLT_PAL1DoubleMuOpen_v1 = Bfr_HLT_PAL1DoubleMuOpen_v1;
  HLT_PAL1DoubleMuOpen_v1_Prescl = Bfr_HLT_PAL1DoubleMuOpen_v1_Prescl;
  HLT_PAL2DoubleMu3_v1 = Bfr_HLT_PAL2DoubleMu3_v1;
  HLT_PAL2DoubleMu3_v1_Prescl = Bfr_HLT_PAL2DoubleMu3_v1_Prescl;
  HLT_PAMu3_v1 = Bfr_HLT_PAMu3_v1;
  HLT_PAMu3_v1_Prescl = Bfr_HLT_PAMu3_v1_Prescl;
  HLT_PAMu7_v1 = Bfr_HLT_PAMu7_v1;
  HLT_PAMu7_v1_Prescl = Bfr_HLT_PAMu7_v1_Prescl;
  HLT_PAMu12_v1 = Bfr_HLT_PAMu12_v1;
  HLT_PAMu12_v1_Prescl = Bfr_HLT_PAMu12_v1_Prescl;

  //B Info
  bP->SetXYZ(BInfo_px[j],BInfo_py[j],BInfo_pz[j]*0);
  bVtx->SetXYZ(BInfo_vtxX[j]-EvtInfo_PVx,
	       BInfo_vtxY[j]-EvtInfo_PVy,
	       BInfo_vtxZ[j]*0-EvtInfo_PVz*0);
  b4P->SetXYZM(BInfo_px[j],BInfo_py[j],BInfo_pz[j],BInfo_mass[j]);

  bindex[typesize] = typesize;
  y[typesize] = b4P->Rapidity();
  dtheta[typesize] = bP->Angle(*bVtx);
  pt[typesize] = BInfo_pt[j];
  eta[typesize] = BInfo_eta[j];
  phi[typesize] = BInfo_phi[j];
  chi2cl[typesize] = TMath::Prob(BInfo_vtxchi2[j],BInfo_vtxdof[j]);
  d0[typesize] = sqrt((BInfo_vtxX[j]-EvtInfo_PVx)*(BInfo_vtxX[j]-EvtInfo_PVx)+(BInfo_vtxY[j]-EvtInfo_PVy)*(BInfo_vtxY[j]-EvtInfo_PVy));
  vx[typesize] = BInfo_vtxX[j] - EvtInfo_PVx;
  vy[typesize] = BInfo_vtxY[j] - EvtInfo_PVy;
  d0Err[typesize] = sqrt(BInfo_vtxXE[j]*BInfo_vtxXE[j]+BInfo_vtxYE[j]*BInfo_vtxYE[j]);
  mass[typesize] = BInfo_mass[j];
  tktkmass[typesize] = BInfo_tktk_mass[j];
  chi2ndf[typesize] = BInfo_vtxchi2[j]/BInfo_vtxdof[j];
  lxy[typesize] = ((BInfo_vtxX[j]-EvtInfo_PVx)*BInfo_px[j] + (BInfo_vtxY[j]-EvtInfo_PVy)*BInfo_py[j])/BInfo_pt[j];
  isbestchi2[typesize] = 0;
  isbesttktkmass[typesize] = 0;
  kstar[typesize] = 0;
  if(BInfo_type[j]==4) kstar[typesize]=1;
  else if(BInfo_type[j]==5) kstar[typesize]=2;

  //muon section
  mu1Striplayer[typesize] = MuonInfo_i_nStripLayer[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]];
  mu1Pixellayer[typesize] = MuonInfo_i_nPixelLayer[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]];
  mu1Chi2ndf[typesize] = MuonInfo_i_chi2[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]]/MuonInfo_i_ndf[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]];
  mu1dxy[typesize] = MuonInfo_dxyPV[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]];
  mu1dz[typesize] = MuonInfo_dzPV[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]];
  if(MuonInfo_muqual[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]]&16) mu1TrackerMuArbitrated[typesize] = 1;
  else mu1TrackerMuArbitrated[typesize] = 0;
  if(MuonInfo_muqual[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]]&4096) mu1StationTight[typesize] = 1;
  else mu1StationTight[typesize] = 0;

  mu2Striplayer[typesize] = MuonInfo_i_nStripLayer[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]];
  mu2Pixellayer[typesize] = MuonInfo_i_nPixelLayer[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]];
  mu2Chi2ndf[typesize] = MuonInfo_i_chi2[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]]/MuonInfo_i_ndf[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]];
  mu2dxy[typesize] = MuonInfo_dxyPV[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]];
  mu2dz[typesize] = MuonInfo_dzPV[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]];
  if(MuonInfo_muqual[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]]&16) mu2TrackerMuArbitrated[typesize] = 1;
  else mu2TrackerMuArbitrated[typesize] = 0;
  if(MuonInfo_muqual[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]]&4096) mu2StationTight[typesize] = 1;
  else mu2StationTight[typesize] = 0;
  
  float mu1px,mu1py,mu1pz,mu1E;
  float mu2px,mu2py,mu2pz,mu2E;

  mu1px = MuonInfo_pt[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]]*cos(MuonInfo_phi[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]]);
  mu1py = MuonInfo_pt[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]]*sin(MuonInfo_phi[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]]);
  mu1pz = MuonInfo_pt[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]]*sinh(MuonInfo_eta[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]]);
  b4P->SetXYZM(mu1px,mu1py,mu1pz,MUON_MASS);
  mu1E = b4P->E();
  mu1eta[typesize] = b4P->Eta();
  mu1phi[typesize] = b4P->Phi();
  mu1y[typesize] = b4P->Rapidity();
  mu1pt[typesize] = b4P->Pt();
  mu1p[typesize] = b4P->P();

  mu2px = MuonInfo_pt[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]]*cos(MuonInfo_phi[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]]);
  mu2py = MuonInfo_pt[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]]*sin(MuonInfo_phi[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]]);
  mu2pz = MuonInfo_pt[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]]*sinh(MuonInfo_eta[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]]);
  b4P->SetXYZM(mu2px,mu2py,mu2pz,MUON_MASS);
  mu2E = b4P->E();
  mu2eta[typesize] = b4P->Eta();
  mu2phi[typesize] = b4P->Phi();
  mu2y[typesize] = b4P->Rapidity();
  mu2pt[typesize] = b4P->Pt();
  mu2p[typesize] = b4P->P();

  b4P->SetPxPyPzE(mu1px+mu2px,
		  mu1py+mu2py,
		  mu1pz+mu2pz,
		  mu1E+mu2E);
  mumumass[typesize] = b4P->Mag();
  mumueta[typesize] = b4P->Eta();
  mumuphi[typesize] = b4P->Phi();
  mumuy[typesize] = b4P->Rapidity();
  mumupt[typesize] = b4P->Pt();


  //jpsi section
  ujmass[typesize] = BInfo_uj_mass[BInfo_rfuj_index[j]];
  ujvProb[typesize] = TMath::Prob(BInfo_uj_vtxchi2[BInfo_rfuj_index[j]],BInfo_uj_vtxdof[BInfo_rfuj_index[j]]);
  b4P->SetXYZM(BInfo_uj_px[BInfo_rfuj_index[j]],
	       BInfo_uj_py[BInfo_rfuj_index[j]],
	       BInfo_uj_pz[BInfo_rfuj_index[j]],
	       BInfo_uj_mass[BInfo_rfuj_index[j]]);
  ujpt[typesize] = b4P->Pt();
  ujeta[typesize] = b4P->PseudoRapidity();
  ujphi[typesize] = b4P->Phi();
  ujy[typesize] = b4P->Rapidity();
  ujlxy[typesize] = ((BInfo_uj_vtxX[BInfo_rfuj_index[j]]-EvtInfo_PVx)*BInfo_uj_px[BInfo_rfuj_index[j]] + (BInfo_uj_vtxY[BInfo_rfuj_index[j]]-EvtInfo_PVy)*BInfo_uj_py[BInfo_rfuj_index[j]])/ujpt[typesize];

  //track section
  float tk1px,tk1py,tk1pz,tk1E;
  float tk2px,tk2py,tk2pz,tk2E;

  b4P->SetPtEtaPhiM(TrackInfo_pt[BInfo_rftk1_index[j]],TrackInfo_eta[BInfo_rftk1_index[j]],TrackInfo_phi[BInfo_rftk1_index[j]],track_mass1);
  trk1Dxy[typesize] = TrackInfo_dxyPV[BInfo_rftk1_index[j]];
  trk1D0Err[typesize] = TrackInfo_d0error[BInfo_rftk1_index[j]];
  trk1PixelHit[typesize] = TrackInfo_pixelhit[BInfo_rftk1_index[j]];
  trk1StripHit[typesize] = TrackInfo_striphit[BInfo_rftk1_index[j]];
  trk1Pt[typesize] = TrackInfo_pt[BInfo_rftk1_index[j]];
  trk1Chi2ndf[typesize] = TrackInfo_chi2[BInfo_rftk1_index[j]]/TrackInfo_ndf[BInfo_rftk1_index[j]];
  trk1Eta[typesize] = TrackInfo_eta[BInfo_rftk1_index[j]];
  trk1Phi[typesize] = TrackInfo_phi[BInfo_rftk1_index[j]];
  trk1Y[typesize] = b4P->Rapidity();
  tk1px = b4P->Px();
  tk1py = b4P->Py();
  tk1pz = b4P->Pz();
  tk1E = b4P->E();

  b4P->SetPtEtaPhiM(TrackInfo_pt[BInfo_rftk2_index[j]],TrackInfo_eta[BInfo_rftk2_index[j]],TrackInfo_phi[BInfo_rftk2_index[j]],track_mass2);
  trk2Dxy[typesize] = TrackInfo_dxyPV[BInfo_rftk2_index[j]];
  trk2D0Err[typesize] = TrackInfo_d0error[BInfo_rftk2_index[j]];
  trk2PixelHit[typesize] = TrackInfo_pixelhit[BInfo_rftk2_index[j]];
  trk2StripHit[typesize] = TrackInfo_striphit[BInfo_rftk2_index[j]];
  trk2Pt[typesize] = TrackInfo_pt[BInfo_rftk2_index[j]];
  trk2Chi2ndf[typesize] = TrackInfo_chi2[BInfo_rftk2_index[j]]/TrackInfo_ndf[BInfo_rftk2_index[j]];
  trk2Eta[typesize] = TrackInfo_eta[BInfo_rftk2_index[j]];
  trk2Phi[typesize] = TrackInfo_phi[BInfo_rftk2_index[j]];
  trk2Y[typesize] = b4P->Rapidity();
  tk2px = b4P->Px();
  tk2py = b4P->Py();
  tk2pz = b4P->Pz();
  tk2E = b4P->E();

  b4P->SetPxPyPzE(tk1px+tk2px,
		      tk1py+tk2py,
		      tk1pz+tk2pz,
		      tk1E+tk2E);
  tktkmass[typesize] = b4P->Mag();
  tktketa[typesize] = b4P->Eta();
  tktkphi[typesize] = b4P->Phi();
  tktky[typesize] = b4P->Rapidity();
  tktkpt[typesize] = b4P->Pt();
  tktkvProb[typesize] = TMath::Prob(BInfo_tktk_vtxchi2[j],BInfo_tktk_vtxdof[j]);
  doubletmass[typesize] = BInfo_tktk_mass[j];
  b4P->SetXYZM(BInfo_tktk_px[j],BInfo_tktk_py[j],BInfo_tktk_pz[j],BInfo_tktk_mass[j]);
  doubletpt[typesize] = b4P->Pt();
  doubleteta[typesize] = b4P->PseudoRapidity();
  doubletphi[typesize] = b4P->Phi();
  doublety[typesize] = b4P->Rapidity();

  //gen info judgement

  if(!REAL){
  
    genBzeroToJpsiK0starKPi[typesize] = 0;//gen init
    genBplusToJpsiKstarX[typesize] = 0;//gen init
    
    int trk1geninfo=TrackInfo_geninfo_index[BInfo_rftk1_index[j]];
	int pdgtrk1=abs(GenInfo_pdgId[trk1geninfo]); 
	int mothertrk1geninfo=GenInfo_mo1[trk1geninfo];
	int pdgmothertrk1=abs(GenInfo_pdgId[mothertrk1geninfo]);
	int grandmothertrk1geninfo=GenInfo_mo1[mothertrk1geninfo];
	int pdggrandmothertrk1=abs(GenInfo_pdgId[grandmothertrk1geninfo]);
	
	
	int trk2geninfo=TrackInfo_geninfo_index[BInfo_rftk2_index[j]];
	int pdgtrk2=abs(GenInfo_pdgId[trk2geninfo]); 
	int mothertrk2geninfo=GenInfo_mo1[trk2geninfo];
	int pdgmothertrk2=abs(GenInfo_pdgId[mothertrk2geninfo]);
	int grandmothertrk2geninfo=GenInfo_mo1[mothertrk2geninfo];
	int pdggrandmothertrk2=abs(GenInfo_pdgId[grandmothertrk2geninfo]);

    
    int muon1geninfo=MuonInfo_geninfo_index[BInfo_uj_rfmu1_index[BInfo_rfuj_index[j]]];
    int pdgmuon1=abs(GenInfo_pdgId[muon1geninfo]);
    int mothermuon1geninfo=GenInfo_mo1[muon1geninfo];
    int pdgmothermuon1=abs(GenInfo_pdgId[mothermuon1geninfo]);
    int grandmothermuon1geninfo=GenInfo_mo1[mothermuon1geninfo];
    int pdggrandmothermuon1=abs(GenInfo_pdgId[grandmothermuon1geninfo]);

    int muon2geninfo=MuonInfo_geninfo_index[BInfo_uj_rfmu2_index[BInfo_rfuj_index[j]]];
    int pdgmuon2=abs(GenInfo_pdgId[muon2geninfo]);
    int mothermuon2geninfo=GenInfo_mo1[muon2geninfo];
    int pdgmothermuon2=abs(GenInfo_pdgId[mothermuon2geninfo]);
    int grandmothermuon2geninfo=GenInfo_mo1[mothermuon2geninfo];
    int pdggrandmothermuon2=abs(GenInfo_pdgId[grandmothermuon2geninfo]);

    bool isbzerotojpsik0starkpi=IsBzeroToJpsiK0starKPi(trk1geninfo,pdgtrk1,mothertrk1geninfo,pdgmothertrk1,grandmothertrk1geninfo,pdggrandmothertrk1,
                                    trk2geninfo,pdgtrk2,mothertrk2geninfo,pdgmothertrk2,grandmothertrk2geninfo,pdggrandmothertrk2,
                                    muon1geninfo,pdgmuon1,mothermuon1geninfo,pdgmothermuon1,grandmothermuon1geninfo,pdggrandmothermuon1,
                                    muon2geninfo,pdgmuon2,mothermuon2geninfo,pdgmothermuon2,grandmothermuon2geninfo,pdggrandmothermuon2);
    bool isbzerotojpsik0starpik=IsBzeroToJpsiK0starPiK(trk1geninfo,pdgtrk1,mothertrk1geninfo,pdgmothertrk1,grandmothertrk1geninfo,pdggrandmothertrk1,
                                    trk2geninfo,pdgtrk2,mothertrk2geninfo,pdgmothertrk2,grandmothertrk2geninfo,pdggrandmothertrk2,
                                    muon1geninfo,pdgmuon1,mothermuon1geninfo,pdgmothermuon1,grandmothermuon1geninfo,pdggrandmothermuon1,
                                    muon2geninfo,pdgmuon2,mothermuon2geninfo,pdgmothermuon2,grandmothermuon2geninfo,pdggrandmothermuon2);
    bool isbplustojpsikx=IsBplusToJpsiKX(trk1geninfo,pdgtrk1,mothertrk1geninfo,pdgmothertrk1,grandmothertrk1geninfo,pdggrandmothertrk1,
                                    trk2geninfo,pdgtrk2,mothertrk2geninfo,pdgmothertrk2,grandmothertrk2geninfo,pdggrandmothertrk2,
                                    muon1geninfo,pdgmuon1,mothermuon1geninfo,pdgmothermuon1,grandmothermuon1geninfo,pdggrandmothermuon1,
                                    muon2geninfo,pdgmuon2,mothermuon2geninfo,pdgmothermuon2,grandmothermuon2geninfo,pdggrandmothermuon2);
    bool isbplustojpsikstarx=IsBplusToJpsiKstarX(trk1geninfo,pdgtrk1,mothertrk1geninfo,pdgmothertrk1,grandmothertrk1geninfo,pdggrandmothertrk1,
                                    trk2geninfo,pdgtrk2,mothertrk2geninfo,pdgmothertrk2,grandmothertrk2geninfo,pdggrandmothertrk2,
                                    muon1geninfo,pdgmuon1,mothermuon1geninfo,pdgmothermuon1,grandmothermuon1geninfo,pdggrandmothermuon1,
                                    muon2geninfo,pdgmuon2,mothermuon2geninfo,pdgmothermuon2,grandmothermuon2geninfo,pdggrandmothermuon2);
   
    
    genBplusToJpsiKstarX[typesize]=(int)(isbplustojpsikstarx);
    genBzeroToJpsiK0starKPi[typesize]=(int)(isbzerotojpsik0starkpi);
    genBzeroToJpsiK0starPiK[typesize]=(int)(isbzerotojpsik0starpik);
    genBplusToJpsiKX[typesize]=(int)(isbplustojpsikx);
    

    //TString cut="(HLT_PAMu3_v1)&&abs(mumumass-3.096916)<0.15&&mass>5&&mass<6&& isbestchi2&&trk1Pt>0.7&&trk2Pt>0.7&&chi2cl>1.14e-01&&(d0/d0Err)>3.53&&cos(dtheta)>6.40e-01&&abs(tktkmass-0.89594)<2.33e-01";

    float mymumumass=mumumass[typesize];
    float mymass=mass[typesize];
    float myisbestchi2=isbestchi2[typesize];
    float mytrk1Pt=trk1Pt[typesize];
    float mytrk2Pt=trk2Pt[typesize];
    float mychi2cl=chi2cl[typesize];
    float myd0Err=d0Err[typesize];
    float myd0=d0[typesize];
    double mydtheta[typesize];
    float mytktkmass=tktkmass[typesize];

    int robaseria=(trk1geninfo>-1)&&(trk2geninfo>-1)&&(muon1geninfo>-1)&&(muon2geninfo>-1);
    int isnotsignal=!(isbzerotojpsik0starkpi||isbzerotojpsik0starpik);
    if(robaseria&&isnotsignal&&(abs(mymumumass-3.096916)<0.15)&&(mymass<5.1)&&(mymass>5.)&&(mytrk1Pt>0.7)&&(mytrk2Pt>0.7)&&(mychi2cl>1.14e-01)&&((myd0/myd0Err)>3.53)&&(abs(mytktkmass-0.89594)<2.33e-01)){
      std::cout<<"______________"<<std::endl;
      std::cout<<"mypdgtrk1="<<pdgtrk1<<",mypdgmothertrk1="<<pdgmothertrk1<<",mypdggrandmothertrk1="<<pdggrandmothertrk1<<std::endl;
      std::cout<<"mypdgtrk2="<<pdgtrk2<<",mypdgmothertrk2="<<pdgmothertrk2<<",mypdggrandmothertrk2="<<pdggrandmothertrk2<<std::endl;
      std::cout<<"mypdgmuon1="<<pdgmuon1<<",mypdgmothermuon1="<<pdgmothermuon1<<",mypdggrandmothermuon1="<<pdggrandmothermuon1<<std::endl;
      std::cout<<"mypdgmuon2="<<pdgmuon2<<",mypdgmothermuon2="<<pdgmothermuon2<<",mypdggrandmothermuon2="<<pdggrandmothermuon2<<std::endl;
      mycheck[typesize]=(int)(1);
    }
   }
}

bool IsBzeroToJpsiK0starKPi(int mytrk1geninfo,int mypdgtrk1,int mymothertrk1geninfo,int mypdgmothertrk1,int mygrandmothertrk1geninfo,int mypdggrandmothertrk1,
                   int mytrk2geninfo,int mypdgtrk2,int mymothertrk2geninfo,int mypdgmothertrk2,int mygrandmothertrk2geninfo,int mypdggrandmothertrk2,
                   int mymuon1geninfo,int mypdgmuon1,int mymothermuon1geninfo,int mypdgmothermuon1,int mygrandmothermuon1geninfo,int mypdggrandmothermuon1, 
                   int mymuon2geninfo,int mypdgmuon2,int mymothermuon2geninfo,int mypdgmothermuon2,int mygrandmothermuon2geninfo,int mypdggrandmothermuon2){
  
  int bGenIdxTk1=-1;
  int bGenIdxTk2=-1;
  int bGenIdxMu1=-1;
  int bGenIdxMu2=-1;
  int mGenIdxTk1=-1;
  int mGenIdxTk2=-1;
    
  int BmesonId = 511;//
  int trkpar1Id = 321;//kaon
  int trkpar2Id = 211;//pion
  int trkJpsiId = 443;//jpsi
  int trkk0starId = 313;//phi
  int trkMuon = 13;//muon
  
  bool okTrk1=false;
  bool okTrk2=false;
  bool okMuon1=false;
  bool okMuon2=false;
  bool okTotal=false;
  //
  okTrk1=IsFromBviaresonance(mytrk1geninfo,mypdgtrk1,mymothertrk1geninfo,mypdgmothertrk1,
                              mygrandmothertrk1geninfo,mypdggrandmothertrk1,trkpar1Id,trkk0starId,BmesonId,bGenIdxTk1);
  okTrk2=IsFromBviaresonance(mytrk2geninfo,mypdgtrk2,mymothertrk2geninfo,mypdgmothertrk2,
                              mygrandmothertrk2geninfo,mypdggrandmothertrk2,trkpar2Id,trkk0starId,BmesonId,bGenIdxTk2);                                                   
  okMuon1=IsFromBviaresonance(mymuon1geninfo,mypdgmuon1,mymothermuon1geninfo,mypdgmothermuon1,
                              mygrandmothermuon1geninfo,mypdggrandmothermuon1,trkMuon,trkJpsiId,BmesonId,bGenIdxMu1);
  okMuon2=IsFromBviaresonance(mymuon2geninfo,mypdgmuon2,mymothermuon2geninfo,mypdgmothermuon2,
                              mygrandmothermuon2geninfo,mypdggrandmothermuon2,trkMuon,trkJpsiId,BmesonId,bGenIdxMu2);
  
  //std::cout<<okTrk1<<okTrk2<<okMuon1<<okMuon2<<std::endl;
  //std::cout<<bGenIdxMu1<<"-"<<bGenIdxMu2<<"-"<<bGenIdxTk1<<"-"<<bGenIdxTk2<<std::endl;
  //  std::cout<<"//////"<<std::endl;

  if(okMuon2&&okMuon1&&okTrk1&&okTrk2){
    if(bGenIdxMu1!=-1 && bGenIdxMu2!=-1 && bGenIdxMu1==bGenIdxMu2 && bGenIdxMu1==bGenIdxTk1 && bGenIdxTk2==bGenIdxTk1){
      okTotal=true;
    }
  }
  //std::cout<<"TOTAL"<<okTotal<<std::endl;
  return okTotal;
  
}

bool IsBzeroToJpsiK0starPiK(int mytrk1geninfo,int mypdgtrk1,int mymothertrk1geninfo,int mypdgmothertrk1,int mygrandmothertrk1geninfo,int mypdggrandmothertrk1,
                   int mytrk2geninfo,int mypdgtrk2,int mymothertrk2geninfo,int mypdgmothertrk2,int mygrandmothertrk2geninfo,int mypdggrandmothertrk2,
                   int mymuon1geninfo,int mypdgmuon1,int mymothermuon1geninfo,int mypdgmothermuon1,int mygrandmothermuon1geninfo,int mypdggrandmothermuon1, 
                   int mymuon2geninfo,int mypdgmuon2,int mymothermuon2geninfo,int mypdgmothermuon2,int mygrandmothermuon2geninfo,int mypdggrandmothermuon2){
  
  int bGenIdxTk1=-1;
  int bGenIdxTk2=-1;
  int bGenIdxMu1=-1;
  int bGenIdxMu2=-1;
  int mGenIdxTk1=-1;
  int mGenIdxTk2=-1;
    
  int BmesonId = 511;//
  int trkpar1Id = 211;//kaon
  int trkpar2Id = 321;//pion
  int trkJpsiId = 443;//jpsi
  int trkk0starId = 313;//phi
  int trkMuon = 13;//muon
  
  bool okTrk1=false;
  bool okTrk2=false;
  bool okMuon1=false;
  bool okMuon2=false;
  bool okTotal=false;
  //
  okTrk1=IsFromBviaresonance(mytrk1geninfo,mypdgtrk1,mymothertrk1geninfo,mypdgmothertrk1,
                              mygrandmothertrk1geninfo,mypdggrandmothertrk1,trkpar1Id,trkk0starId,BmesonId,bGenIdxTk1);
  okTrk2=IsFromBviaresonance(mytrk2geninfo,mypdgtrk2,mymothertrk2geninfo,mypdgmothertrk2,
                              mygrandmothertrk2geninfo,mypdggrandmothertrk2,trkpar2Id,trkk0starId,BmesonId,bGenIdxTk2);                                                   
  okMuon1=IsFromBviaresonance(mymuon1geninfo,mypdgmuon1,mymothermuon1geninfo,mypdgmothermuon1,
                              mygrandmothermuon1geninfo,mypdggrandmothermuon1,trkMuon,trkJpsiId,BmesonId,bGenIdxMu1);
  okMuon2=IsFromBviaresonance(mymuon2geninfo,mypdgmuon2,mymothermuon2geninfo,mypdgmothermuon2,
                              mygrandmothermuon2geninfo,mypdggrandmothermuon2,trkMuon,trkJpsiId,BmesonId,bGenIdxMu2);
  
  //std::cout<<okTrk1<<okTrk2<<okMuon1<<okMuon2<<std::endl;
  //std::cout<<bGenIdxMu1<<"-"<<bGenIdxMu2<<"-"<<bGenIdxTk1<<"-"<<bGenIdxTk2<<std::endl;
  //  std::cout<<"//////"<<std::endl;

  if(okMuon2&&okMuon1&&okTrk1&&okTrk2){
    if(bGenIdxMu1!=-1 && bGenIdxMu2!=-1 && bGenIdxMu1==bGenIdxMu2 && bGenIdxMu1==bGenIdxTk1 && bGenIdxTk2==bGenIdxTk1){
      okTotal=true;
    }
  }
  //std::cout<<"TOTAL"<<okTotal<<std::endl;
  return okTotal;
  
}


bool IsBplusToJpsiKX(int mytrk1geninfo,int mypdgtrk1,int mymothertrk1geninfo,int mypdgmothertrk1,int mygrandmothertrk1geninfo,int mypdggrandmothertrk1,
                   int mytrk2geninfo,int mypdgtrk2,int mymothertrk2geninfo,int mypdgmothertrk2,int mygrandmothertrk2geninfo,int mypdggrandmothertrk2,
                   int mymuon1geninfo,int mypdgmuon1,int mymothermuon1geninfo,int mypdgmothermuon1,int mygrandmothermuon1geninfo,int mypdggrandmothermuon1, 
                   int mymuon2geninfo,int mypdgmuon2,int mymothermuon2geninfo,int mypdgmothermuon2,int mygrandmothermuon2geninfo,int mypdggrandmothermuon2){
  
  int bGenIdxTk1=-1;
  int bGenIdxTk2=-1;
  int bGenIdxMu1=-1;
  int bGenIdxMu2=-1;
  int mGenIdxTk1=-1;
  int mGenIdxTk2=-1;
    
  int BmesonId = 521;//
  int trkpar1Id = 321;//kaon
  int trkpar2Id = 211;//pion
  int trkJpsiId = 443;//jpsi
  int trkMuon = 13;//muon
  
  bool okTrk1=false;
  bool okTrk2=false;
  bool okMuon1=false;
  bool okMuon2=false;
  bool okTotal=false;
    // 
  okTrk1=IsTrackfromBdirect(mytrk1geninfo,mypdgtrk1,mymothertrk1geninfo,mypdgmothertrk1,trkpar1Id,BmesonId,bGenIdxTk1);
  okTrk2=IsTrackfromBdirect(mytrk2geninfo,mypdgtrk2,mymothertrk2geninfo,mypdgmothertrk2,trkpar1Id,BmesonId,bGenIdxTk2);

  okMuon1=IsFromBviaresonance(mymuon1geninfo,mypdgmuon1,mymothermuon1geninfo,mypdgmothermuon1,
                              mygrandmothermuon1geninfo,mypdggrandmothermuon1,trkMuon,trkJpsiId,BmesonId,bGenIdxMu1);
  okMuon2=IsFromBviaresonance(mymuon2geninfo,mypdgmuon2,mymothermuon2geninfo,mypdgmothermuon2,
                              mygrandmothermuon2geninfo,mypdggrandmothermuon2,trkMuon,trkJpsiId,BmesonId,bGenIdxMu2);
  
  //std::cout<<(okTrk1||okTrk2)<<okMuon1<<okMuon2<<std::endl;
  //std::cout<<bGenIdxMu1<<"-"<<bGenIdxMu2<<"-"<<bGenIdxTk1<<"-"<<bGenIdxTk2<<std::endl;
  //std::cout<<"//////"<<std::endl;

  if(okMuon2&&okMuon1&&(okTrk1||okTrk2)){
    if(bGenIdxMu1!=-1 && bGenIdxMu2!=-1 && bGenIdxMu1==bGenIdxMu2 && bGenIdxMu1==bGenIdxTk1 ){
      okTotal=true;
    }
    if(bGenIdxMu1!=-1 && bGenIdxMu2!=-1 && bGenIdxMu1==bGenIdxMu2 && bGenIdxMu1==bGenIdxTk2 ){
      okTotal=true;
    }
  }
  return okTotal;
  
}

bool IsBplusToJpsiKstarX(int mytrk1geninfo,int mypdgtrk1,int mymothertrk1geninfo,int mypdgmothertrk1,int mygrandmothertrk1geninfo,int mypdggrandmothertrk1,
                   int mytrk2geninfo,int mypdgtrk2,int mymothertrk2geninfo,int mypdgmothertrk2,int mygrandmothertrk2geninfo,int mypdggrandmothertrk2,
                   int mymuon1geninfo,int mypdgmuon1,int mymothermuon1geninfo,int mypdgmothermuon1,int mygrandmothermuon1geninfo,int mypdggrandmothermuon1, 
                   int mymuon2geninfo,int mypdgmuon2,int mymothermuon2geninfo,int mypdgmothermuon2,int mygrandmothermuon2geninfo,int mypdggrandmothermuon2){
  
  int bGenIdxTk1=-1;
  int bGenIdxTk2=-1;
  int bGenIdxMu1=-1;
  int bGenIdxMu2=-1;
  int mGenIdxTk1=-1;
  int mGenIdxTk2=-1;
    
  int BmesonId = 521;//
  int trkpar1Id = 321;//kaon
  int trkpar2Id = 211;//pion
  int trkJpsiId = 443;//jpsi
  int trkk0starId = 323;//phi
  int trkMuon = 13;//muon
  
  bool okTrk1=false;
  bool okTrk2=false;
  bool okMuon1=false;
  bool okMuon2=false;
  bool okTotal=false;
  
    // 
  okTrk1=IsFromBviaresonance(mytrk1geninfo,mypdgtrk1,mymothertrk1geninfo,mypdgmothertrk1,
                              mygrandmothertrk1geninfo,mypdggrandmothertrk1,trkpar2Id,trkk0starId,BmesonId,bGenIdxTk1);
  okTrk2=IsFromBviaresonance(mytrk2geninfo,mypdgtrk2,mymothertrk2geninfo,mypdgmothertrk2,
                              mygrandmothertrk2geninfo,mypdggrandmothertrk2,trkpar2Id,trkk0starId,BmesonId,bGenIdxTk2);
  okMuon1=IsFromBviaresonance(mymuon1geninfo,mypdgmuon1,mymothermuon1geninfo,mypdgmothermuon1,
                              mygrandmothermuon1geninfo,mypdggrandmothermuon1,trkMuon,trkJpsiId,BmesonId,bGenIdxMu1);
  okMuon2=IsFromBviaresonance(mymuon2geninfo,mypdgmuon2,mymothermuon2geninfo,mypdgmothermuon2,
                              mygrandmothermuon2geninfo,mypdggrandmothermuon2,trkMuon,trkJpsiId,BmesonId,bGenIdxMu2);
  
/*
  if ((mytrk1geninfo>-1)&&(mypdgmothertrk1==323)&&(mypdgmothertrk2==323)){
  std::cout<<"mypdgtrk1="<<mypdgtrk1<<",mypdgmothertrk1"<<mypdgmothertrk1<<",mypdggrandmothertrk1"<<mypdggrandmothertrk1<<std::endl;
  std::cout<<"mypdgtrk2="<<mypdgtrk2<<",mypdgmothertrk2"<<mypdgmothertrk2<<",mypdggrandmothertrk2"<<mypdggrandmothertrk2<<std::endl;
  std::cout<<"mypdgmuon1="<<mypdgmuon1<<",mypdgmothermuon1"<<mypdgmothermuon1<<",mypdggrandmothermuon1"<<mypdggrandmothermuon1<<std::endl;
  std::cout<<"mypdgmuon2="<<mypdgmuon2<<",mypdgmothermuon2"<<mypdgmothermuon2<<",mypdggrandmothermuon2"<<mypdggrandmothermuon2<<std::endl;

}
*/
    //std::cout<<"okMuon1="<<okMuon1<<",okMuon2"<<okMuon2<<",(okTrk1||okTrk2)"<<(okTrk1||okTrk2)<<std::endl;
    //std::cout<<"bGenIdxMu1="<<bGenIdxMu1<<",bGenIdxMu2"<<bGenIdxMu2<<",bGenIdxTk1"<<bGenIdxTk1<<",bGenIdxTk2"<<bGenIdxTk2<<std::endl;

  if(okMuon2&&okMuon1&&(okTrk1||okTrk2)){
    if(bGenIdxMu1!=-1 && bGenIdxMu2!=-1 && bGenIdxMu1==bGenIdxMu2 && bGenIdxMu1==bGenIdxTk1 ){
      okTotal=true;
    }
    if(bGenIdxMu1!=-1 && bGenIdxMu2!=-1 && bGenIdxMu1==bGenIdxMu2 && bGenIdxMu1==bGenIdxTk2 ){
      okTotal=true;
    }
  }
  return okTotal;
  
}




bool IsTrackfromBdirect(int mytrkgeninfo,int mypdgtrk,int myBmesongeninfo,int mypdgBmeson,
                        int pdgtrk,int pdgBmeson,int &Bindex){
                        
  bool okTrk=false;
                        
  if(mytrkgeninfo>-1){
	if(mypdgtrk==pdgtrk){
	  if(myBmesongeninfo>-1){
		if(mypdgBmeson==pdgBmeson){
		  Bindex=myBmesongeninfo;
		  okTrk=true;
		}//if compatible with Bid	  
	  }//if GenInfo_mo1>-1
	}//is trk1d==pdg
  }//end trk1geninfo  
  return okTrk;
}

bool IsFromBviaresonance(int myparticlegeninfo,int mypdgparticle,int myresonancegeninfo,int mypdgresonance,
                         int myBmesongeninfo,int mypdgBmeson,
                         int pdgparticle,int pdgresonance, int pdgBmeson, int &Bindex){
                        
  bool okparticle=false;
                        
  if(myparticlegeninfo>-1){  
	if(mypdgparticle==pdgparticle){
	  if(myresonancegeninfo>-1){
	    if(mypdgresonance==pdgresonance){
	      if(myBmesongeninfo>-1){
		    if(mypdgBmeson==pdgBmeson){
		      Bindex=myBmesongeninfo;
		      okparticle=true;	    
		    }//if compatible with Bid	  
		  }//if GenInfo_mo1_mo1>-1
		}//is resonance==pdg
	  }//if GenInfo_mo1>-1
	}//is particle==pdg
  }//end particlegeninfo 
  return okparticle;
}



void debugB0(string infile="/mnt/hadoop/cms/store/user/jwang/Bfinder_BoostedMC_20140418_Hijing_PPb502_MinimumBias_HIJINGemb_inclBtoPsiMuMu_5TeV.root", string outfile="../../output/debugB0.root", bool REAL=0){
//////////////////////////////////////////////////////////Phi
//   This file has been automatically generated 
//     (Thu Nov 21 13:34:42 2013 by ROOT version5.27/06b)
//   from TTree root/root
//   found on file: merged_pPbData_20131114.root
//////////////////////////////////////////////////////////

  const char* infname;
  const char* outfname;

  if(REAL) cout<<"--- REAL DATA ---"<<endl;
  else cout<<"--- MC ---"<<endl;


  infname = infile.c_str();
  outfname = outfile.c_str();

  //File type
  TFile *f = new TFile(infname);
  TTree *root = (TTree*)f->Get("demo/root");
  TTree *hlt = (TTree*)f->Get("hltanalysis/HltTree");
  if (root->GetEntries()!=hlt->GetEntries()) {
     cout <<"Inconsistent number of entries!!!"<<endl;
     cout <<"HLT tree: "<<hlt->GetEntries()<<endl;
     cout <<"Bfinder tree: "<<root->GetEntries()<<endl;
  }
  
  //Chain type
  //TChain* root = new TChain("demo/root");
  //root->Add("/mnt/hadoop/cms/store/user/wangj/HI_Btuple/20140213_PAMuon_HIRun2013_PromptReco_v1/Bfinder_all_100_1_dXJ.root");
  //root->Add("/mnt/hadoop/cms/store/user/wangj/HI_Btuple/20140213_PAMuon_HIRun2013_PromptReco_v1/Bfinder_all_101_1_kuy.root");
  //root->Add("/mnt/hadoop/cms/store/user/wangj/HI_Btuple/20140213_PAMuon_HIRun2013_PromptReco_v1/Bfinder_all_10_1_ZkX.root");
  //root->Add("/mnt/hadoop/cms/store/user/wangj/HI_Btuple/20140213_PAMuon_HIRun2013_PromptReco_v1/Bfinder_all_102_1_NyI.root");
  
  TFile *outf = new TFile(outfname,"recreate");

  setBranch(root);
  setHltBranch(hlt);
    
  int ifchannel[7];
  ifchannel[3] = 1; //jpsi+K*(K+,pi-)
  ifchannel[4] = 1; //jpsi+K*(K-,pi+)
  
  TTree* nt3 = new TTree("ntKstar","");
  buildBranch(nt3);

  cout<<"--- Tree building finished ---"<<endl;
  
  Long64_t nentries = root->GetEntries();
  nentries = 100000;
  Long64_t nbytes = 0;
  TVector3* bP = new TVector3;
  TVector3* bVtx = new TVector3;
  TLorentzVector* b4P = new TLorentzVector;
  TLorentzVector* b4Pout = new TLorentzVector;
  TLorentzVector bGen;
  int type,flag;
  int flagEvt=0;  
  int offsetHltTree=0;

  int testevent=0,testcand=0;
  
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += root->GetEntry(i);
    flagEvt=0;
    while (flagEvt==0)
    {
       hlt->GetEntry(i+offsetHltTree);
       //cout <<offsetHltTree<<" "<<Bfr_HLT_Event<<" "<<EvtInfo_EvtNo<<endl;
       if (Bfr_HLT_Event==EvtInfo_EvtNo && Bfr_HLT_Run==EvtInfo_RunNo) flagEvt=1; else offsetHltTree++;
    } 

    if (i%10000==0) cout <<i<<" / "<<nentries<<"   offset HLT:"<<offsetHltTree<<endl;

    int type1size=0,type2size=0,type3size=0,type4size=0,type5size=0,type6size=0,type7size=0;
    float best,best2,temy;
    int bestindex,best2index;

    
    size=0;
    best=-1;
    bestindex=-1;
    best2=10000.;
    best2index=-1;
    for (int j=0;j<BInfo_size;j++) 
      {
	if(BInfo_type[j]>7) continue;
	if (ifchannel[BInfo_type[j]-1]!=1) continue;
	//skim{{{
	b4Pout->SetXYZM(BInfo_px[j],BInfo_py[j],BInfo_pz[j],BInfo_mass[j]);
	temy = b4Pout->Rapidity();
	if(REAL)
	  {
	    if(!(((EvtInfo_RunNo>=210498&&EvtInfo_RunNo<=211256&&abs(temy+0.465)<1.93)||(EvtInfo_RunNo>=211313&&EvtInfo_RunNo<=211631&&abs(temy-0.465)<1.93)))) continue;
	  }
	else
	  {
	    if(abs(temy+0.465)>=1.93) continue;
	  }
	if(BInfo_mass[j]<5 || BInfo_mass[j]>6) continue;
	if(BInfo_pt[j]<10.) continue;
	//}}}
	if(BInfo_type[j]==4 || BInfo_type[j]==5)
	  {
	    fillTree(bP,bVtx,b4P,j,type4size,KAON_MASS,PION_MASS,REAL);
	    if(chi2cl[type4size]>best)
	      {
		best = chi2cl[type4size];
		bestindex = type4size;
	      }
	    if(abs(tktkmass[type4size]-KSTAR_MASS)<best2)
	      {
		best2 = abs(tktkmass[type4size]-KSTAR_MASS);
		best2index = type4size;
	      }
	    type4size++;
	  }
      }
    if(size>0)
      {
	bestchi2 = bestindex;
	isbestchi2[bestindex] = 1;
	besttktkmass = best2index;
	isbesttktkmass[best2index] = 1;
      }
    nt3->Fill();
    
  }

  outf->Write();
  outf->Close();
}


