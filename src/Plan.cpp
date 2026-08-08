#include"Plan.h"
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):plan_id(planId),settlement(settlement),selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),facilities(),underConstruction(),facilityOptions(facilityOptions),life_quality_score(0),economy_score(0),environment_score(0){}
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions,vector<Facility*>facility,vector<Facility*> under,int life_quality_score,int economy_score,int environment_score):Plan(planId,settlement,selectionPolicy,facilityOptions){
facilities=facility;
underConstruction=under;
this->life_quality_score=life_quality_score;
this->economy_score=economy_score;
this->environment_score=environment_score;
}
const int Plan::getlifeQualityScore()const{return life_quality_score;}
const int Plan::getEconomyScore()const{return economy_score;}
const int Plan::getEnvironmentScore()const{return environment_score;}
void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    if(selectionPolicy)
    delete this->selectionPolicy;
    this->selectionPolicy=selectionPolicy;
    
}
void Plan::step(){
if(status==PlanStatus::AVALIABLE){
    size_t elim=settlement.typeInt();
    while(underConstruction.size()<elim){
    const FacilityType& temp=selectionPolicy->selectFacility(facilityOptions);
    Facility* output=new Facility(temp,settlement.getName());
    underConstruction.push_back(output);}

}
for (vector<Facility*>::iterator it = underConstruction.begin(); it != underConstruction.end(); ) {
    (*it)->step();
    if ((*it)->getStatus() == FacilityStatus::OPERATIONAL) {
        facilities.push_back(*it);
        life_quality_score=getlifeQualityScore()+(*it)->getLifeQualityScore();
        economy_score=getEconomyScore()+(*it)->getEconomyScore();
         environment_score=getEnvironmentScore()+(*it)->getEnvironmentScore();
        it = underConstruction.erase(it); 
    } else {
        ++it; 
    }
}
size_t sizeT=settlement.typeInt();
if(underConstruction.size()==sizeT)
    status=PlanStatus::BUSY;
    else
    status=PlanStatus::AVALIABLE;
}
void Plan::printStatus(){
std::cout<<toString();

}


const vector<Facility*>& Plan::getFacilities()const{return facilities;}
void Plan::addFacility(Facility* facility){
    facilities.push_back(facility);
}
const string Plan:: toString() const {
    string s= "PlanId: "+std::to_string(plan_id)+"\n"+"SettlementName: "+ settlement.getName()+"\n"+"PlanStatus: "+swithStatus(status)+"\n"+"SelectionPolicy: "+selectionPolicy->toString()+"\n"+"LifeQualityScore: "+std::to_string(life_quality_score)+"\n"+"EconomyScore: "+std::to_string(economy_score)+"\n"+"EnvironmentScore: "+std::to_string(environment_score)+"\n";
    return s;

}
const string Plan::swithStatus(PlanStatus p)const{
    string result;
    switch (p)
    {
    case PlanStatus::AVALIABLE:
        result="AVALIABLE";
        break;
    
    case PlanStatus::BUSY:
    result="BUSY";
        break;
    }
    return result;
}


const std::vector<string> Plan::planStatus()const{
    std::vector<string> output;
    for(Facility* facility:underConstruction){
    output.push_back(facility->toString());}
    for(Facility* facility:facilities){
    output.push_back(facility->toString());}
    return output;
}
const string Plan::getPolicyName()const{
    return selectionPolicy->toString();
}
Plan::~Plan(){
    if(selectionPolicy)
    delete selectionPolicy;
    for(Facility* facility:facilities){
        if (facility)
        delete facility;
    }
    facilities.clear();
    for(Facility* facility:underConstruction){
        if (facility)
        delete facility;
    }
    underConstruction.clear();
}
Plan::Plan(const Plan &other):plan_id(other.plan_id),settlement(other.settlement),selectionPolicy(),status(other.status),facilities(),underConstruction(),facilityOptions(other.facilityOptions),life_quality_score(other.life_quality_score),economy_score(other.economy_score),environment_score(other.environment_score){
    selectionPolicy=(other.selectionPolicy)->clone();
    for(Facility* facility:other.facilities){
        facilities.push_back(new Facility(*facility));
    }
   for(Facility* facility:other.underConstruction){
    underConstruction.push_back(new Facility(*facility));
   } 
}
Plan::Plan(Plan &&other):plan_id(other.plan_id),settlement(other.settlement),selectionPolicy(),status(other.status),facilities(),underConstruction(),facilityOptions(other.facilityOptions),life_quality_score(other.life_quality_score),economy_score(other.economy_score),environment_score(other.environment_score){
    selectionPolicy=other.selectionPolicy;
    other.selectionPolicy=nullptr;
    for(Facility* facility:other.facilities){
        facilities.push_back(facility);
        facility=nullptr;
    }
    other.facilities.clear();
   for(Facility* facility:underConstruction){
    underConstruction.push_back(facility);
        facility=nullptr;
   }
   other.underConstruction.clear();
}
const string Plan::getSettlementName()const{
    return settlement.getName();
}
vector<Facility*> Plan::cloneFacility(){
    vector<Facility*>out;
    for(Facility* fac:facilities){
    out.push_back(new Facility(*fac));}
    return out;
}
vector<Facility*> Plan::cloneFacilityUnder(){
    vector<Facility*>out;
    for(Facility* fac:underConstruction){
    out.push_back(new Facility(*fac));}
    return out;
}
SelectionPolicy* Plan::getSelectionPolicy(){
    return selectionPolicy;
}
int Plan::getPlanId()const{
    return plan_id;
}
const int Plan::underConstructionLife()const{
    int output=0;
    for(Facility* facility:underConstruction){
        output=output+facility->getLifeQualityScore();
    }
    return output;
}
const int Plan::underConstructionEco()const{
    int output=0;
    for(Facility* facility:underConstruction){
        output=output+facility->getEconomyScore();
    }
    return output;
}
const int Plan::underConstructionEnv()const{
    int output=0;
    for(Facility* facility:underConstruction){
        output=output+facility->getEnvironmentScore();
    }
    return output;
}





    









