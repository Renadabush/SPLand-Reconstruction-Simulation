#include"SelectionPolicy.h"
#include<cmath>
#include<climits>
NaiveSelection::NaiveSelection():lastSelectedIndex(0){}
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    const FacilityType& output=facilitiesOptions.at(lastSelectedIndex);
    size_t size=facilitiesOptions.size();
    lastSelectedIndex=(lastSelectedIndex+1)%size;
    return output;
}
const string NaiveSelection::toString()const{return "nve";}
NaiveSelection* NaiveSelection::clone()const{
    return new NaiveSelection(*this);
}
// from last to 0
EconomySelection::EconomySelection():lastSelectedIndex(0){}
const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    size_t size=facilitiesOptions.size();
    for(size_t i=lastSelectedIndex;i<facilitiesOptions.size() ;i=(i+1)%size){
        if((facilitiesOptions.at(i)).getCategory()==FacilityCategory::ECONOMY){
            lastSelectedIndex=(i+1)%size;
           return facilitiesOptions.at(i);
        }
    }
    return facilitiesOptions.at(0);// צריך לבדוק
}
const string EconomySelection::toString()const{return "eco";}
EconomySelection* EconomySelection::clone()const{
    return new EconomySelection(*this);
}
SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(0){}
const FacilityType& SustainabilitySelection ::selectFacility(const vector<FacilityType>& facilitiesOptions){
     size_t size=facilitiesOptions.size();
     for(size_t i=lastSelectedIndex;i<facilitiesOptions.size() ;i=(i+1)%size){
        if((facilitiesOptions.at(i)).getCategory()==FacilityCategory::ENVIRONMENT){
            lastSelectedIndex=(i+1)%size;
           return facilitiesOptions.at(i);
        }
    }
    return facilitiesOptions.at(0);// צריך לבדןק
}
const string SustainabilitySelection::toString()const{return "env";}
SustainabilitySelection* SustainabilitySelection::clone()const{
    return new SustainabilitySelection(*this);
}
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):LifeQualityScore(LifeQualityScore),EconomyScore(EconomyScore),EnvironmentScore(EnvironmentScore){}
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    int compare=INT_MAX;
    int index=0,i=0;
    for(FacilityType myFacility:facilitiesOptions){
    int life_d=LifeQualityScore+(myFacility).getLifeQualityScore();
    int economy_d=EconomyScore+(myFacility).getEconomyScore();
    int envir_d=EnvironmentScore+(myFacility).getEnvironmentScore();
    int min=std::min(life_d,std::min(envir_d,economy_d));
    int max=std::max(life_d,std::max(envir_d,economy_d));
    if(max-min<compare){
        compare=max-min;
        index=i;
        }
        i=i+1;
    }
    LifeQualityScore=LifeQualityScore+facilitiesOptions.at(index).getLifeQualityScore();
    EconomyScore=EconomyScore+facilitiesOptions.at(index).getEconomyScore();
    EnvironmentScore=EnvironmentScore+facilitiesOptions.at(index).getEnvironmentScore();
    return facilitiesOptions.at(index);
}
const string BalancedSelection::toString()const{return "bal";}
BalancedSelection* BalancedSelection::clone()const{
    return new BalancedSelection(*this);
}
