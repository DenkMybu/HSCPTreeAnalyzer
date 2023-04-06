#include "MassTools.h"
#include "PlotTools.h"

#include <string>
#include <sstream>



// Function
TH2F* BetheBlochForMass(float mass)
{
float K(2.30), C(3.17); //Data
float K_data2018(2.27), C_data2018(3.16); //Data 2018
float K_data2017(2.30), C_data2017(3.17);
    std::stringstream ss;
    ss<<mass;
    //std::string strmass = std::to_string(mass);
    std::string strmass = ss.str();
    TH2F* tmp = new TH2F(strmass.c_str(),";p [GeV];I_{h} [MeV/cm]",200,0,2000,100,0,10);
    for(int i=1;i<tmp->GetNbinsX();i++)
    {
        float mom = tmp->GetXaxis()->GetBinCenter(i);
        float dedx = K*pow(mass/mom,2)+C;
        for(int j=1;j<tmp->GetNbinsY();j++)
        {
            if(tmp->GetYaxis()->GetBinLowEdge(j)<=dedx && tmp->GetYaxis()->GetBinLowEdge(j+1)>dedx)
            {
                tmp->SetBinContent(i,j,0.001);
            }   
        }
    }
    return tmp;
}

// Function returning the MassErr as function momentum, dEdx, and errors on momentum and dEdx
// Not take into account any erros coming from K&C factors because this function is used to see the impact of binning in p and dedx on mass error
double GetMassErr (double P, double PErr, double dEdx, double dEdxErr, double M, double dEdxK, double dEdxC)
{
   if (M < 0) return -1;

   double Criteria = dEdx - dEdxC;
   double MassErr = sqrt(pow(P*dEdxErr,2)/(4*dEdxK*Criteria)+(Criteria/dEdxK)*pow(PErr,2));

   if (std::isnan(MassErr) || std::isinf(MassErr)) MassErr = -1;

   return MassErr/M;
}


// Return the mass as a function of momentum, dEdx, K and C. 
// It corresponds to the Bethe-Bloch parametrisation used in the Hscp analysis
float GetMass(float p, float ih, float k, float c)
{
    return (ih-c)<0?-1:sqrt((ih-c)/k)*p;
}


void crossHistos(TH2F* res, TH1F* h1, TH1F* h2)
{
    scale(h1); 
    for(int i=0;i<h1->GetNbinsX()+1;i++)
    {
        for(int j=0;j<h2->GetNbinsX()+1;j++)
        {   

            float mom = h1->GetBinCenter(i);
            float dedx = h2->GetBinCenter(j);
            double prob = h1->GetBinContent(i)*h2->GetBinContent(j);
            if(prob>=0)
            {
                res->Fill(mom,dedx,prob);
            }
        }
    }
    res->Sumw2();
}

// Function doing the crossing between 1D-histograms of dEdx and momentum and returning a 2D-histogram (p,ih),
// and respecting the eta binning as in the mass distribution calculation 
void crossHistosEtaBinning(TH2F* res, TH2F* eta_p, TH2F* ih_eta)
{
    TH1F* eta = (TH1F*) ih_eta->ProjectionX();
    for(int i=0;i<eta->GetNbinsX()+1;i++)
    {
        TH1F* p = (TH1F*) eta_p->ProjectionX("proj_p",i,i+1);
        TH1F* ih = (TH1F*) ih_eta->ProjectionY("proj_ih",i,i+1);
        scale(p);
        for(int j=0;j<p->GetNbinsX()+1;j++)
        {
            for(int k=0;k<ih->GetNbinsX()+1;k++)
            {
                float mom = p->GetBinCenter(j);
                float dedx = ih->GetBinCenter(k);
                float prob = p->GetBinContent(j) * ih->GetBinContent(k);
                float err_prob = prob*sqrt((1./(ih->GetBinContent(k)))+(1./(p->GetBinContent(j)*ih->Integral())));
                if(prob>=0)
                {
                    res->SetBinContent(j,k,res->GetBinContent(j,k)+prob);
                    res->SetBinError(j,k,sqrt(pow(res->GetBinError(j,k),2)+pow(err_prob,2)));
                }
            }
        }
        delete p;
        delete ih;
    }
    res->Sumw2();
}

