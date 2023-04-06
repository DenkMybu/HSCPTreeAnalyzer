#include "PlotTools.h"



// Function returning an 1D-histogram which contains the ratio between two histograms   
TH1F* ratioHist(TH1F* h1, TH1F* h2)
{
    TH1F* res = (TH1F*) h1->Clone();
    for(int i=0;i<h1->GetNbinsX();i++)
    {
        double ratio = h2->GetBinContent(i)>0 ? h1->GetBinContent(i)/h2->GetBinContent(i) : 0;
        res->SetBinContent(i,ratio);
    }
    res->Divide(h2);
    res->Sumw2();
    return res;
}


// Scale the 1D-histogram given to the unit 
void scale(TH1F* h)
{
    h->Scale(1./h->Integral(0,h->GetNbinsX()+1));
}

// Inverse the scaling ; scale the 1D-histogram to its number of entries
void invScale(TH1F* h)
{
    h->Sumw2();
    h->Scale(h->GetEntries());
}

// Function returning the ratio of right integer (from x to infty) for two 1D-histograms
// This function is used in the Hscp data-driven background estimate to test the mass shape prediction
// The argument to use this type of ratio is that we're in case of cut & count experiment 
TH1F* ratioIntegral(TH1F* h1, TH1F* h2)
{    
    //float SystError = 0.2;
    TH1F* res = (TH1F*) h1->Clone(); res->Reset();
    for(int i=0;i<h1->GetNbinsX()+1;i++)
    {   
        double Perr=0, Derr=0;
        double P=h1->IntegralAndError(i,h1->GetNbinsX()+1,Perr); if(P<=0) continue;
        double D=h2->IntegralAndError(i,h2->GetNbinsX()+1,Derr);
        //Perr = sqrt(Perr*Perr + pow(P*SystError,2));
        res->SetBinContent(i,D/P);
        res->SetBinError(i,sqrt(pow(Derr*P,2)+pow(Perr*D,2))/pow(P,2));
    }
    return res;
}

// Function returning chi2/ndof compatibility test for two 1D-histograms
// As a reference, we've access to the number of degrees of freedom 
float chi2test(TH1F* h1, TH1F* h2,int& dof)
{
    float res=0;
    int ndof=0;
    for(int i=1;i<h1->GetNbinsX();i++)
    {
        res += h2->GetBinContent(i)>0 ? pow((h1->GetBinContent(i)-h2->GetBinContent(i)),2)/h2->GetBinContent(i) : 0 ;
        if(h2->GetBinContent(i)>0) ndof++;
    }
    dof=ndof;
    return res/ndof;
}

void overflowLastBin(TH1F* h){
    h->SetBinContent(h->GetNbinsX(),h->GetBinContent(h->GetNbinsX())+h->GetBinContent(h->GetNbinsX()+1));
    h->SetBinContent(h->GetNbinsX()+1,0);
}

void overflowLastBin(TH1F* h, const float &x){
    for(int i=h->FindBin(x);i<=h->GetNbinsX()+1;i++){
        h->SetBinContent(h->FindBin(x)-1,h->GetBinContent(h->FindBin(x)-1)+h->GetBinContent(i));
        h->SetBinContent(i,0);
    }
}


TH1F* rebinHisto(TH1F* h){
    overflowLastBin(h);
    //double xbins[20] = {0,50,100,150,200,250,300,350,400,450,500,600,700,800,1000,1500,2000,3000,4000,6000};
    double xbins[12] = {0,100,200,300,400,500,600,700,800,1000,1500,2000};
    //double xbins[17] = {0,50,100,150,200,250,300,350,400,450,500,600,700,800,1000,1500,2000};
    //double xbins[13] = {0.0,0.05,0.1,0.15,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};
    std::vector<double> xbins_v;
    for(double i=0.0;i<=1000.0;i+=50) xbins_v.push_back(i);
    std::string newname = h->GetName(); 
    newname += "_rebinned";
    TH1F* hres = (TH1F*) h->Rebin(11,newname.c_str(),xbins);
    //TH1F* hres = (TH1F*) h->Rebin(xbins_v.size()-1,newname.c_str(),xbins_v.data());
    overflowLastBin(hres);
    return hres;
}

// Function returning a canvas divide in two windows
// The first one contains the two 1D-histograms, given as arguments, superposed.
// There also is a legend associated to this window where the names are defined as arguments.
// The second window contains the ratio of these 1D-histograms or the ratio of right integers of them. 
// We define which kind of ratio we want with tha 'ratioSimple' boolean.
// The 'name' given corresponds to the name of the canvas 
TCanvas* plotting(TH1F* h1, TH1F* h2, bool ratioSimple, std::string name, std::string leg1, std::string leg2, bool rebin)
{

    h1->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
    h2->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
    //h1->Sumw2(); h2->Sumw2();
    if(rebin) h1=rebinHisto(h1);
    if(rebin) h2=rebinHisto(h2);
    //overflowLastBin(h1);
    //overflowLastBin(h2);
    TCanvas* c1 = new TCanvas(("plotting_"+name).c_str(),"");
    c1->Divide(1,2);
    gStyle->SetOptStat(0);
    c1->cd(1);
    TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);
    leg->AddEntry(h1,leg1.c_str(),"lep");
    leg->AddEntry(h2,leg2.c_str(),"lep");
    h1->Draw();
    h2->SetLineColor(2);
    h2->Draw("esame");
    leg->Draw("same");
    c1->SetLogy();
    c1->cd(2);
    TH1F* tmp = (TH1F*) h1->Clone(); tmp->Reset();
    if(ratioSimple)
    {
        //h1->Scale(1./h1->Integral());
        //h2->Scale(1./h2->Integral());
        tmp = (TH1F*)h1->Clone();
        tmp->Divide(h2);
        tmp->GetYaxis()->SetTitle("#frac{N_{obs}}{N_{pred}}");
        tmp->GetYaxis()->SetTitleSize(0.06);
    }
    else
    {
        //h1->Scale(1./h1->Integral());
        //h2->Scale(1./h2->Integral());
        tmp=ratioIntegral(h2,h1);
        tmp->GetYaxis()->SetTitle("#int_{M}^{#infty} dm_{obs} / #int_{M}^{#infty} dm_{pred}");
        tmp->GetYaxis()->SetTitleSize(0.06);
    }

    int dof;
    float chi2=chi2test(h1,h2,dof);
    tmp->GetYaxis()->SetRangeUser(0,2);
    tmp->Draw();
    return c1;
}

// Function returning a 1D-histogram with variable binning.
// To determine it, we set a minimum value 'min' corresponding to the number of entries wanted in each bins of the distribution.
// If a given bin has a number of entries below than 'min' then this bin is merged with the next one and then there is another test, etc. 
// Finally, as a reference argument, the function gives a vector composed by the binning previously determined 
TH1F* rebinning(TH1F* h1, float min, std::vector<double>& vect)
{
    std::vector<double> v_val;
    int i=0;
    while(i<h1->GetNbinsX()+1)
    {
        float cont=0;
        while(cont<min && i<h1->GetNbinsX()+1)
        {
            cont += h1->GetBinContent(i);
            i++;
        }
        v_val.push_back(h1->GetBinLowEdge(i));
    }
    std::string tit = "Rebinned_"; tit+=h1->GetName();
    TH1F* h2 = new TH1F(tit.c_str(),"",v_val.size()-1,v_val.data());
    for(int j=0;j<h1->GetNbinsX()+1;j++)
    {
        for(int c=0;c<h1->GetBinContent(j);c++)
        {
            h2->Fill(h1->GetBinLowEdge(j));
        }
    }
    h2->Sumw2();
    vect = v_val;
    return h2;
}

TH1F* rebinningGraph(TH1F* h1, float min, std::vector<float>& vect1, std::vector<double>& vect2)
{
    std::vector<float> v_val;
    std::vector<float> v_cont;
    std::vector<float> v_err;
    std::vector<double> v_bin;
    int i=0;
    while(i<=h1->GetNbinsX()+1)
    {
        float cont=0;
        float div=0;
        float err=0;
        while(div<min && i<=h1->GetNbinsX()+1)
        {
            cont += h1->GetBinCenter(i)*h1->GetBinContent(i);
            div += h1->GetBinContent(i);
            err += pow(h1->GetBinError(i),2);
            i++;
        }
        v_val.push_back(div>0?cont/div:0);
        v_cont.push_back(div);
        v_err.push_back(sqrt(err));
        v_bin.push_back(h1->GetBinLowEdge(i));
    }

    TH1F* h2 = new TH1F("Rebinned","",v_bin.size()-1,v_bin.data());

    for(int j=0;j<h1->GetNbinsX()+1;j++)
    {
        for(int c=0;c<h1->GetBinContent(j);c++)
        {
            h2->Fill(h1->GetBinLowEdge(j));
        }
    }

    vect1 = v_val;
    vect2 = v_bin;

    TGraphErrors* gr = new TGraphErrors(v_val.size()-1,v_val.data(),v_cont.data(),0,v_err.data());
    return h2;
}

// Function doing the eta reweighing between two 2D-histograms as done in the Hscp background estimate method,
// because of the correlation between variables (momentum & transverse momentum). 
// The first given 2D-histogram is weighted in respect to the second one. 
void etaReweighingP(TH2F* eta_p_1, TH2F* eta_p_2)
{
    TH1F* eta1 = (TH1F*) eta_p_1->ProjectionY(); eta1->Scale(1./eta1->Integral());
    TH1F* eta2 = (TH1F*) eta_p_2->ProjectionY(); eta2->Scale(1./eta2->Integral());
    eta2->Divide(eta1);
    for(int i=0;i<eta_p_1->GetNbinsX()+1;i++)
    {
        for(int j=0;j<eta_p_1->GetNbinsY()+1;j++)
        {
            float val_ij = eta_p_1->GetBinContent(i,j);
            float err_ij = eta_p_1->GetBinError(i,j);
            eta_p_1->SetBinContent(i,j,val_ij*eta2->GetBinContent(j));
            eta_p_1->SetBinError(i,j,err_ij*eta2->GetBinContent(j));
        }
    }
    eta_p_1->Sumw2();
}


// Function returning a 2D-histogram with the relative difference of two 1D-histograms 
// given as arguments. 
void mapOfDifferences(TH2F* res, TH2F* h1, TH2F* h2)
{
    for(int i=0;i<h1->GetNbinsX();i++)
    {
        for(int j=0;j<h1->GetNbinsY();j++)
        {
            double err=h1->GetBinError(i,j);
            double diff=h1->GetBinContent(i,j)>0?abs((h2->GetBinContent(i,j)-h1->GetBinContent(i,j))/h1->GetBinContent(i,j)):0;
            if(diff>=0) res->SetBinContent(i,j,diff);
        }
    }
}

