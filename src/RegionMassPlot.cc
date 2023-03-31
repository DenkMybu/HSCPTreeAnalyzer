#include "inc/RegionMassPlot.h"

float K(2.30), C(3.17); //Data
float K_data2018(2.27), C_data2018(3.16); //Data 2018
float K_data2017(2.30), C_data2017(3.17);

RegionMassPlot::RegionMassPlot(){}

RegionMassPlot::RegionMassPlot(std::string suffix,int etabins,int ihbins,int pbins,int massbins)
{
    suffix_ = suffix;
    std::cout << "init"+suffix << std::endl;
    initHisto(etabins,ihbins,pbins,massbins);
} 

RegionMassPlot::~RegionMassPlot(){}

// Function which intializes the histograms with given binnings 
void RegionMassPlot::initHisto(int& etabins,int& ihbins,int& pbins,int& massbins)
{
    np = pbins;
    plow = 0;
    pup = 8000;

    npt = pbins;
    ptlow = 0;
    ptup = 10000; //instead of 4000

    //1overP
    //np = 2000;
    pup = 200;

    nih = ihbins;
    ihlow = 3;
    ihup = 8;

    nias = ihbins;
    iaslow = 0;
    iasup = 1;

    neta = etabins;
    etalow = -3;
    etaup = 3;

    nmass = massbins;
    masslow = 0;
    massup = 4000;

    std::string suffix = suffix_;
    
    c = new TCanvas(suffix.c_str(),"");

    ih_pt = new TH2F(("ih_pt"+suffix).c_str(),";pt [GeV];I_{h} [MeV/cm]",npt,ptlow,ptup,nih,ihlow,ihup); ih_pt->Sumw2();
    ias_pt = new TH2F(("ias_pt"+suffix).c_str(),";pt [GeV];I_{as}",npt,ptlow,ptup,nias,iaslow,iasup); ias_pt->Sumw2();
    ih_ias = new TH2F(("ias_ih"+suffix).c_str(),";I_{as};I_{h} [MeV/cm]",nias,iaslow,iasup,nih,ihlow,ihup); ih_ias->Sumw2();
    ih_nhits = new TH2F(("ih_nhits"+suffix).c_str(),";nhits;I_{h} [MeV/cm]",20,0,20,nih,ihlow,ihup); ih_nhits->Sumw2();
    ias_nhits = new TH2F(("ias_nhits"+suffix).c_str(),";nhits;I_{as}",20,0,20,nias,iaslow,iasup); ias_nhits->Sumw2();
    eta_pt = new TH2F(("eta_pt"+suffix).c_str(),";pt [GeV];#eta",npt,ptlow,ptup,neta,etalow,etaup); eta_pt->Sumw2();
    eta_p = new TH2F(("eta_p"+suffix).c_str(),";p [GeV];#eta",np,plow,pup,neta,etalow,etaup); eta_p->Sumw2();
    eta_p_rebinned = nullptr; 
    nhits_pt = new TH2F(("nhits_pt"+suffix).c_str(),";pt [GeV];nhits",npt,ptlow,ptup,20,0,20); nhits_pt->Sumw2();
    eta_nhits = new TH2F(("eta_nhits"+suffix).c_str(),";nhits;#eta",20,0,20,neta,etalow,etaup); eta_nhits->Sumw2();
    ih_eta = new TH2F(("ih_eta"+suffix).c_str(),";#eta;I_{h} [MeV/cm]",neta,etalow,etaup,nih,ihlow,ihup); ih_eta->Sumw2();
    ih_p = new TH2F(("ih_p"+suffix).c_str(),";p [GeV];I_{h} [MeV/cm]",np,plow,pup,nih,ihlow,ihup); ih_p->Sumw2();
    ias_p = new TH2F(("ias_p"+suffix).c_str(),";p [GeV];I_{as}",np,plow,pup,nias,iaslow,iasup); ias_p->Sumw2();
    pt_pterroverpt = new TH2F(("pt_pterroverpt"+suffix).c_str(),";p_{T} [GeV];#frac{#sigma_{pT}}{p_{T}}",npt,ptlow,ptup,100,0,1); pt_pterroverpt->Sumw2();
    ias_eta = new TH2F(("ias_eta"+suffix).c_str(),";#eta;I_{as}",neta,etalow,etaup,nias,iaslow,iasup); ias_eta->Sumw2();
    mass_eta = new TH2F(("mass_eta"+suffix).c_str(),";#eta;Mass [GeV]",neta,etalow,etaup,nmass,masslow,massup); mass_eta->Sumw2();
    eta_npv = new TH2F(("eta_npv"+suffix).c_str(),";npv;#eta",100,0,100,neta,etalow,etaup); eta_npv->Sumw2();
    p_npv = new TH2F(("p_npv"+suffix).c_str(),";npv;p [GeV]",100,0,100,np,plow,pup); p_npv->Sumw2();
    ih_npv = new TH2F(("ih_npv"+suffix).c_str(),";npv;I_{h} [MeV/cm]",100,0,100,nih,ihlow,ihup); ih_npv->Sumw2();
    is_ias = new TH2F(("is_ias"+suffix).c_str(),";I_{S};I_{as}",100,0,1,nias,iaslow,iasup); is_ias->Sumw2();
    is_ih = new TH2F(("is_ih"+suffix).c_str(),";I_{S};I_{h} [MeV/cm]",100,0,1,nih,ihlow,ihup); ih_ias->Sumw2();

    mass = new TH1F(("massFromTree"+suffix).c_str(),";Mass [GeV]",nmass,masslow,massup); mass->Sumw2();
    massFrom1DTemplates = new TH1F(("massFrom1DTemplates"+suffix).c_str(),";Mass [GeV]",nmass,masslow,massup); massFrom1DTemplates->Sumw2();
    massFrom1DTemplatesEtaBinning = new TH1F(("massFrom1DTemplatesEtaBinning"+suffix).c_str(),";Mass [GeV]",nmass,masslow,massup); massFrom1DTemplatesEtaBinning->Sumw2();
    pred_mass = new TH1F(("pred_mass"+suffix).c_str(),";Mass [GeV]",nmass,masslow,massup); pred_mass->Sumw2();
   
    mass->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
    massFrom1DTemplates->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
    massFrom1DTemplatesEtaBinning->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
    pred_mass->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);

    errMass = new TH1F(("errMass"+suffix).c_str(),";Mass error [GeV]",nmass,masslow,massup); errMass->Sumw2();
    Mass_errMass = new TH2F(("Mass_errMass"+suffix).c_str(),";Mass [GeV];Mass error [GeV]",nmass,masslow,massup,nmass,masslow,massup); Mass_errMass->Sumw2();
    cross1Dtemplates = new TH2F(("cross1Dtemplates_ih_p_"+suffix).c_str(),";p [GeV];I_{h} [MeV/cm]",np,plow,pup,nih,ihlow,ihup); cross1Dtemplates->Sumw2();

    ih_used         = new TH1F(("ih_used"+suffix).c_str(),";I_{h} [MeV/cm];",nih,ihlow,ihup); ih_used->Sumw2();
    mapM800         = new TH2F(("mapM800_ih_p_"+suffix).c_str(),";p [GeV];I_{h} [MeV/cm]",np,plow,pup,nih,ihlow,ihup); mapM800->Sumw2();

    momentumDistribM1000    = new TH1F(("momentumDistribM1000_"+suffix).c_str(),";p [GeV]",np,plow,pup); momentumDistribM1000->Sumw2();
    dedxDistribM1000    = new TH1F(("dedxDistribM1000_"+suffix).c_str(),";I_{h} [MeV/cm]",nih,ihlow,ihup); dedxDistribM1000->Sumw2();

    hTOF    = new TH1F(("hTOF_"+suffix).c_str(),";TOF",200,-10,10); hTOF->Sumw2();

}

// Function which fills histograms
void RegionMassPlot::fill(float eta, float nhits, float p, float pt, float pterr, float ih, float ias, float is, float m, float tof, float npv, float w)
{
   ih_pt->Fill(pt,ih,w);
   ias_pt->Fill(pt,ias,w);
   ih_ias->Fill(ias,ih,w);
   ih_nhits->Fill(nhits,ih,w);
   ias_nhits->Fill(nhits,ias,w);
   eta_pt->Fill(pt,eta,w);
   eta_p->Fill(p,eta,w);
   nhits_pt->Fill(pt,nhits,w);
   eta_nhits->Fill(nhits,eta,w);
   ih_eta->Fill(eta,ih,w);
   ih_p->Fill(p,ih,w);
   ias_p->Fill(p,ias,w);
   mass->Fill(m,w);
   hTOF->Fill(tof,w);
   pt_pterroverpt->Fill(pt,pterr/pt,w);
   ias_eta->Fill(eta,ias,w);
   mass_eta->Fill(eta,m,w);
   eta_npv->Fill(npv,eta,w);
   p_npv->Fill(npv,p,w);
   ih_npv->Fill(npv,ih,w);
   is_ias->Fill(is,ias,w);
   is_ih->Fill(is,ih,w);
}


    

// in order to compute properly the uncertainties we use the methods SetBinContent SetBinError instead of Fill
// as several couples of bins in (p,ih) can provide the same mass estimate we need to properly sum the entries and errors
// for a couple of bins in (p,ih) where the bin content were (N_p,N_ih) the associated quantities should be 
// content: (N_p * N_ih) / N_total, where N_total represents the total number of events in the RegionMassPlot (integral of p, ih & mass distributions)
// error: content * sqrt( 1 / N_p + 1 / N_ih ) where we assume Poisson uncertainties in both distributions (independent distributions) and we neglect the uncertainty on N_total
// While combining the input for several couples leading to the same mass: 
// contents are added 
// errors: the sqrt of the squared uncertainties are added
// 
    

void RegionMassPlot::fillMassFrom1DTemplatesEtaBinning(float weight_=-1) 
{
    //errMass = new TH1F(("errMass"+suffix_).c_str(),";Mass error",200,0,2000);
    TH1F* eta = (TH1F*) ih_eta->ProjectionX();
    //ih_p_eta->GetYaxis()->SetRange(ih_p_eta->GetYaxis()->FindBin(0.),ih_p_eta->GetYaxis()->FindBin(200.)); //test did in order to see the impact to take ih at low p --> huge impact 
    for(int i=1;i<eta->GetNbinsX();i++)
    {
        TH1F* p = (TH1F*) eta_p->ProjectionX("proj_p",i,i);
        if(VectOfBins_P_.size()>1) p = (TH1F*)p->Rebin(VectOfBins_P_.size()-1,"",VectOfBins_P_.data());
        /*TH1F* ih = (TH1F*)((TH2F*)ih_p_eta->Project3D("zx"))->ProjectionY("proj_ih_pcut",i,i);
        for(int x=0;x<ih->GetNbinsX()+1;x++)
        {
            ih_used->Fill(ih->GetBinCenter(x),ih->GetBinContent(x));
        }*/
        TH1F* ih = (TH1F*) ih_eta->ProjectionY("proj_ih",i,i);
        scale(p); //only scale one of the two distributions ih or p --> keep the information of the normalisation 
        for(int j=1;j<p->GetNbinsX();j++)
        {
            for(int k=1;k<ih->GetNbinsX();k++)
            {
                if(p->GetBinContent(j)<=0) continue;
                if(ih->GetBinContent(k)<=0) continue;
                float mom = p->GetBinCenter(j);
                float dedx = ih->GetBinCenter(k);
                float prob = p->GetBinContent(j) * ih->GetBinContent(k);
                //float weight = prob*p->Integral();
                float weight = prob;
                if(weight_>0) weight = weight_;
                float err_weight = weight*sqrt((1./(ih->GetBinContent(k)))+(1./(p->GetBinContent(j)*ih->Integral())));
                float mass = GetMass(mom,dedx,K,C);
                int bin_mass = massFrom1DTemplatesEtaBinning->FindBin(mass);
                float mass_err = mass*GetMassErr(mom,p->GetBinWidth(j)/sqrt(12),dedx,ih->GetBinWidth(k)/sqrt(12),mass,K,C);
                std::cout << "mass: " << mass << std::endl;
                if(prob>=0)
                {
                    // first version : wrong --> bad computation of errors
                    //massFrom1DTemplatesEtaBinning->Fill(GetMass(mom,dedx,K,C),prob*p->Integral());
                    //massFrom1DTemplatesEtaBinning->Fill(mass,weight);
                    massFrom1DTemplatesEtaBinning->SetBinContent(bin_mass,massFrom1DTemplatesEtaBinning->GetBinContent(bin_mass)+weight);
                    massFrom1DTemplatesEtaBinning->SetBinError(bin_mass,sqrt(pow(massFrom1DTemplatesEtaBinning->GetBinError(bin_mass),2)+pow(err_weight,2)));
                    //errMass->Fill(mass_err);
                    //Mass_errMass->Fill(mass,mass_err);
                    //if(mass>800) mapM800->Fill(mom,dedx,prob);
                    /*if(mass>1000)
                    {
                        for(int itMom=0;itMom<p->GetBinContent(j);itMom++)momentumDistribM1000->Fill(mom);
                        for(int itDedx=0;itDedx<ih->GetBinContent(k);itDedx++)dedxDistribM1000->Fill(dedx);
                        for(int z=0;z<prob;z++) mapM800->Fill(mom,dedx);
                    }*/
                }
            }
        }
        delete p;
        delete ih;
    }
    //massFrom1DTemplatesEtaBinning->Sumw2();
}

void RegionMassPlot::cross1D()
{
    crossHistosEtaBinning(cross1Dtemplates,eta_p,ih_eta);
}

void RegionMassPlot::plotMass()
{
    c->Divide(1,2);
    c->cd(1);
    scale(mass); scale(massFrom1DTemplates); scale(massFrom1DTemplatesEtaBinning);
    mass->Draw();
    massFrom1DTemplatesEtaBinning->Draw("same");
    massFrom1DTemplatesEtaBinning->SetLineColor(2);
    c->SetLogy();
    c->cd(2);
    TH1F* rat2 = ratioIntegral(massFrom1DTemplatesEtaBinning,mass);
    rat2->Draw();
}

void RegionMassPlot::write()
{
    hTOF->Write();
    ih_pt->Write();
    ias_pt->Write();
    ih_ias->Write();
    ih_nhits->Write();
    ias_nhits->Write();
    eta_pt->Write();
    eta_p->Write();
    nhits_pt->Write();
    eta_nhits->Write();
    ih_eta->Write();
    ih_p->Write();
    cross1Dtemplates->Write();
    ias_p->Write();
    pt_pterroverpt->Write();
    ias_eta->Write();
    mass_eta->Write();
    eta_npv->Write();
    p_npv->Write();
    ih_npv->Write();
    is_ias->Write();
    is_ih->Write();
    mass->Write();
    massFrom1DTemplatesEtaBinning->Write();
    errMass->Write();
    Mass_errMass->Write();
    ih_pt->ProjectionY()->Write();
    ih_p->ProfileX()->Write();
    ias_p->ProfileX()->Write();
    ih_pt->ProfileX()->Write();
    ias_pt->ProfileX()->Write();
    momentumDistribM1000->Write();
    dedxDistribM1000->Write();
}

void RegionMassPlot::addToList(TList* list){
    list->Add(hTOF);
    list->Add(ih_pt);
    list->Add(ias_pt);
    list->Add(ih_ias);
    list->Add(ih_nhits);
    list->Add(ias_nhits);
    list->Add(eta_pt);
    list->Add(eta_p);
    list->Add(nhits_pt);
    list->Add(eta_nhits);
    list->Add(ih_eta);
    list->Add(ih_p);
    list->Add(cross1Dtemplates);
    list->Add(ias_p);
    list->Add(pt_pterroverpt);
    list->Add(ias_eta);
    list->Add(mass_eta);
    list->Add(eta_npv);
    list->Add(p_npv);
    list->Add(ih_npv);
    list->Add(is_ias);
    list->Add(is_ih);
    list->Add(mass);
    list->Add(massFrom1DTemplatesEtaBinning);
    list->Add(errMass);
    list->Add(Mass_errMass);
    list->Add(ih_pt->ProjectionY());
    list->Add(ih_p->ProfileX());
    list->Add(ias_p->ProfileX());
    list->Add(ih_pt->ProfileX());
    list->Add(ias_pt->ProfileX());
    list->Add(momentumDistribM1000);
    list->Add(dedxDistribM1000);
}

