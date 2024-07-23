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



float triggerSystFactor(float eta, float beta, int syst) {
  float betaBins[7] = {0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
  if (syst > 0) {
  // Up systematics
    if (fabs(eta) < 0.3) {
      //EtaA
      float scaleBins[7] = {1.0,2.3,1.4,1.1,1.0,1.0,1.0};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    } else if(fabs(eta) < 0.6) {
      //EtaB
      float scaleBins[7] = {1.0,2.3,2.2,1.2,1.0,1.0,1.0};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    } else if(fabs(eta) < 0.9) {
      //EtaC
      float scaleBins[7] = {1.0,2.3,2.2,1.4,1.1,1.0,1.0};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    } else if(fabs(eta) < 1.2) {
      //EtaD
      float scaleBins[7] = {1.0,2.2,2.2,2.1,1.2,1.0,1.0};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    } else if(fabs(eta) < 2.1) {
      //EtaE
      float scaleBins[7] = {1.0,2.3,2.2,1.3,1.0,1.0,1.0};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    } else {
      //EtaF
      float scaleBins[7] = {1.0,2.3,2.2,1.1,1.0,1.0,1.0};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    }
  } else {
      // Down systematics
    if (fabs(eta) < 0.3) {
        //EtaA
      float scaleBins[7] = {0.0,0.3,0.64,0.86,0.94,1.0,1.0};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    } else if(fabs(eta) < 0.6) {
        //EtaB
      float scaleBins[7] = {0.0,0.34,0.34,0.74,0.96,1.0,1.0};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    } else if(fabs(eta) < 0.9) {
        //EtaC
      float scaleBins[7] = {0.0,0.4,0.4,0.59,0.85,0.96,1.0};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    } else if(fabs(eta) < 1.2) {
        //EtaD
      float scaleBins[7] = {0.0,0.37,0.37,0.37,0.7,0.95,1.0};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    } else if(fabs(eta) < 2.1) {
        //EtaE
      float scaleBins[7] = {0.0,0.37,0.38,0.72,0.95,0.98,0.98};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    } else {
        //EtaF
      float scaleBins[7] = {0.0,0.45,0.45,0.8,0.99,1.0,1.0};
      for (int i = 0; i < 7; i++) {
        if (beta < betaBins[i]) {
          return scaleBins[i];
        }
      }
    }
  }
  return 0;
}

// Return the mass as a function of momentum, dEdx, K and C. 
// It corresponds to the Bethe-Bloch parametrisation used in the Hscp analysis
float GetMass(float p, float ih, float k, float c)
{
    return (ih-c)<0?-1:sqrt((ih-c)/k)*p;
}


float GetMassPow(float p, float ih, float k, float c,float n_estim)
{
    return (ih-c)<0?-1:pow((ih-c)/k,1/n_estim)*p;
}



float GetMassBeta(float p, float beta)
{
    float gamma = 1 / sqrt(1 - beta * beta);
    return (p / (beta * gamma));
}

float sigmaP(float pt, float eta,float sigma_pt)
{
    float cosh_eta = cosh(eta);
    float sigma_p = cosh_eta * sigma_pt;
    return sigma_p;
}

float errorMassBeta(float p, float beta, float sigma_beta)
{

    float gamma = 1 / sqrt(1 - beta * beta);
    //float partial_derivative = -p / (beta * beta * gamma * gamma * gamma);
    float partial_derivative = -p * (gamma + ( (beta*beta)/pow((1-(beta*beta)),3/2)))/(beta*beta*gamma*gamma);
    float sigma_mass = fabs(partial_derivative * sigma_beta);    
    return sigma_mass;
}

float errorMassBetaWithP(float p, float beta, float sigma_beta,float sigma_p)
{

    float gamma = 1 / sqrt(1 - beta * beta);
    float dmdbeta = (-p*1.0) / (beta*beta * sqrt(1-(beta*beta)));
    float dmdp = (sqrt(1-(beta*beta))/beta);
    float sigma_mass = sqrt( (dmdbeta*dmdbeta*sigma_beta*sigma_beta) + (dmdp*dmdp*sigma_p*sigma_p)  );
    return sigma_mass;
}
float errorMassDedx(float p,float ih,float k,float c,float sigma_dEdX)
{
    if (ih - c < 0) return -1;
    float partial_derivative = 0.5 * p / (sqrt(k * (ih - c)));
    float sigma_mass = partial_derivative * sigma_dEdX;
    return sigma_mass;
}
float errorMassWithP(float p,float ih,float k,float c,float sigma_dEdx,float sigmaP){
    if (ih-c < 0) return -1;
    float ErrMass = sqrt( ( (p*p*sigma_dEdx*sigma_dEdx) + (4*(ih-c)*(ih-c)*sigmaP*sigmaP) )/(4*(ih-c)*k) );
    return ErrMass;
}

double partial_df_dedx(double beta_gamma, double p1, double p2, double p3, double p4) {
    return p1 * pow((sqrt(pow(beta_gamma, 4) + 4 * pow(beta_gamma, 2)) - pow(beta_gamma, 2)) / 2, p2 / 2) * (p2 * (sqrt(pow(beta_gamma, 4) + 4 * pow(beta_gamma, 2)) * 2 * pow(beta_gamma, 3) + 4 * beta_gamma) / (2 * (sqrt(pow(beta_gamma, 4) + 4 * pow(beta_gamma, 2)) - pow(beta_gamma, 2)))) * log(1 + pow(p3 * beta_gamma, p4)) + p1 * pow((sqrt(pow(beta_gamma, 4) + 4 * pow(beta_gamma, 2)) - pow(beta_gamma, 2)) / 2, p2 / 2) * (p4 * pow(p3, p4) * pow(beta_gamma, p4 - 1))/ (1 + pow(p3 * beta_gamma, p4));
}

double calculateErrorOnMass(double df_dedx, double df_dp, double sigma_dedx, double sigma_dp) {
    double sigma_m = sqrt(pow(df_dedx * sigma_dedx, 2) + pow(df_dp * sigma_dp, 2));
    return sigma_m;
}


float deltaR(float eta1, float phi1, float eta2, float phi2) {
    float deta = eta1 - eta2;
    float dphi = phi1 - phi2;
    while (dphi > M_PI)
        dphi -= 2 * M_PI;
    while (dphi <= -M_PI)
        dphi += 2 * M_PI;
    return sqrt(deta * deta + dphi * dphi);
}

float GetCombMass(float mBeta, float mDeDx)
{
    return (mBeta + mDeDx)/2;
}
float GetCombMassWeighted(float mBeta, float mDeDx,float stdBeta, float stdDeDx)
{
    if(stdBeta <= 0) return -1;
    if(mBeta <= 0) return -1;
    if(mDeDx <= 0) return -1;
    if(stdDeDx <= 0) return -1;
    float weight1 = 1.0 / (stdBeta*stdBeta);
    float weight2 = 1.0 / (stdDeDx*stdDeDx);
    float newMass = ( (mBeta * weight1) + (mDeDx*weight2) ) / (weight1 + weight2);
    return newMass;
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

std::vector<double> readScaleFactors(const std::string& fileName) {
    std::vector<double> scaleFactors;
    std::ifstream file(fileName);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string binLabel, colon, scaleFactorStr;
        if (!(iss >> binLabel >> colon >> scaleFactorStr)) {
            std::cerr << "Error reading line from file: " << fileName << std::endl;
            continue;
        }
        std::cout << "Pushing SF = " << std::stod(scaleFactorStr) << " to txt file" << std::endl;
        scaleFactors.push_back(std::stod(scaleFactorStr));
    }
    return scaleFactors;
}

double findScaleFactor(double value, const std::vector<std::pair<double, double>>& binRanges, const std::vector<double>& scaleFactors) {
    for (size_t i = 0; i < binRanges.size(); ++i) {
        if (value >= binRanges[i].first && value < binRanges[i].second) {
            return scaleFactors[i];
        }
    }
    std::cerr << "Error: Value is outside of the bin ranges." << std::endl;
    return -1;
}
double SF_betaError(double beta){
    if(beta<0) return -1;   
    return 2.44128 - beta*0.637;
}

