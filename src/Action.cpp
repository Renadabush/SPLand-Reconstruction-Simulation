#include"Action.h"
extern Simulation* backup;
BaseAction::BaseAction():errorMsg(),status(){}
ActionStatus BaseAction::getStatus()const{
    return status;
}
void BaseAction::complete(){
    status=ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg){
    status=ActionStatus::ERROR;
    this->errorMsg=errorMsg;
    std::cout<<"Error: "<<getErrorMsg()<<std::endl;

}
const string& BaseAction::getErrorMsg()const{
    return errorMsg;
}
SimulateStep::SimulateStep(const int numOfSteps):numOfSteps(numOfSteps){}
void SimulateStep::act(Simulation &simulation){
    int index=0;
    while(index<numOfSteps){
    simulation.step();
    index=index+1;
    }

}
const string SimulateStep::toString()const{
    if(getStatus()==ActionStatus::COMPLETED)
    return "step "+std::to_string(numOfSteps)+" COMPLETED"+"\n";
    else
    return "step "+std::to_string(numOfSteps)+" ERROR"+"\n";

}
SimulateStep* SimulateStep::clone()const{
    return new SimulateStep(*this);
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName),selectionPolicy(selectionPolicy){}
void AddPlan::act(Simulation &simulation){
    SelectionPolicy* answer;
    if((simulation.isSettlementExists(settlementName))&&(selectionPolicy=="nve"||selectionPolicy=="eco"||selectionPolicy=="env"||selectionPolicy=="bal")){
        if(selectionPolicy=="nve")
        answer=new NaiveSelection();
        else if(selectionPolicy=="eco")
        answer=new EconomySelection();
        else if(selectionPolicy=="env")
        answer=new SustainabilitySelection();
        else answer=new BalancedSelection(0,0,0);
        simulation.addPlan(simulation.getSettlement(settlementName),answer);
        complete();
        }
        else
        error("Cannot create this plan");

}
const string AddPlan::toString()const{
    if(getStatus()==ActionStatus::COMPLETED)
    return "plan "+ settlementName+" "+selectionPolicy+" COMPLETED"+"\n";
    else
    return "plan "+ settlementName+" "+selectionPolicy+" ERROR"+"\n";
}
AddPlan* AddPlan::clone()const{
    return new AddPlan(*this);
}
AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName),settlementType(settlementType){}
void AddSettlement::act (Simulation &simulation){
    bool output=simulation.isSettlementExists(settlementName);
    if(output==true)
    error("Settlement already exists");
    else {
        bool a=simulation.addSettlement(new Settlement(settlementName,settlementType));
        if(a)
        complete();}
}
const string AddSettlement::toString()const{
    int num;
    if(settlementType==SettlementType::VILLAGE)
    num=0;
    else if(settlementType==SettlementType::CITY)
    num=1;
    else num=2;
    if(getStatus()==ActionStatus::COMPLETED)
    return "settlement "+ settlementName+" "+std::to_string(num)+" COMPLETED"+"\n";
    else
    return "settlement "+ settlementName+" "+std::to_string(num)+" ERROR"+"\n";
}
AddSettlement* AddSettlement::clone()const{
    return new AddSettlement(*this);
}
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){}
void AddFacility::act(Simulation &simulation){
    bool output =simulation.addFacility(FacilityType(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore));
    if(output==false)
    error("Facility already exists");
    else
    complete();}
const string AddFacility::toString()const{
    int num;
    if(facilityCategory==FacilityCategory::LIFE_QUALITY)
    num=0;
    else if(facilityCategory==FacilityCategory::ECONOMY)
    num=1;
    else num=2;
    if(getStatus()==ActionStatus::COMPLETED)
    return "facility "+ facilityName+" "+std::to_string(num)+" "+std::to_string(price)+" "+std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore)+" COMPLETED"+"\n";
    else
    return "facility "+ facilityName+" "+std::to_string(num)+" "+std::to_string(price)+" "+std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore)+" ERRORS"+"\n";
}
AddFacility* AddFacility::clone()const{
    return new AddFacility(*this);
}

PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){}
void PrintPlanStatus::act(Simulation &simulation){
    if(planId>=0 &&planId<simulation.getPlanID()){
    //std::cout<<(simulation.getPlan(planId)).toString();
    (simulation.getPlan(planId)).printStatus();
    std::vector<string> output=(simulation.getPlan(planId)).planStatus();
    for(string s:output){
    std::cout<<s;}
    complete();}
    else error ("Plan doesn't exist");
}
const string PrintPlanStatus::toString()const{
    if(getStatus()==ActionStatus::COMPLETED)
    return "planStatus "+std::to_string(planId)+" COMPLETED"+"\n";
    else
    return "planStatus "+std::to_string(planId)+" ERROR"+"\n";
}
PrintPlanStatus* PrintPlanStatus::clone()const{
    return new PrintPlanStatus(*this);
}
PrintActionsLog::PrintActionsLog(){}
void PrintActionsLog::act(Simulation &simulation){
    std::vector<string> output=simulation.actionsLogVector();
    for(string a:output)
    std::cout<<a;
    complete();
}
const string PrintActionsLog::toString()const{
    return "log COMPLETED\n";
}
PrintActionsLog* PrintActionsLog::clone()const{
    return new PrintActionsLog(*this);
}
Close::Close(){}
void Close::act(Simulation &simulation){
    for(int i=0;i<simulation.getPlanID();i=i+1){
        std::cout<<(simulation.getPlan(i)).toString();
    }
    complete();
    simulation.close();

}
const string Close::toString()const{
    return "close COMPLETED\n";
}
Close* Close::clone()const{
    return new Close(*this);
}
BackupSimulation::BackupSimulation(){}
void BackupSimulation::act(Simulation &simulation){
    if(backup)
    delete backup;
    backup=new Simulation(simulation);
    complete();
}
const string BackupSimulation::toString()const{
    return "backup COMPLETED\n";
}
BackupSimulation* BackupSimulation::clone()const{
    return new BackupSimulation(*this);
}
 RestoreSimulation:: RestoreSimulation(){}
 void RestoreSimulation::act(Simulation &simulation){
    if(backup==nullptr)
    error("No backup available");
    else{
    simulation=(*backup);
    complete();}
 }
 const string RestoreSimulation::toString()const{
   if(getStatus()==ActionStatus::COMPLETED)
    return "restore COMPLETED \n";
    else
    return "restore ERROR \n";
}
 RestoreSimulation* RestoreSimulation::clone()const{
    return new RestoreSimulation(*this);
}
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):planId(planId),newPolicy(newPolicy){}
void ChangePlanPolicy::act(Simulation &simulation){
    if((planId>=simulation.getPlanID()||planId<0)||((simulation.getPlan(planId)).getPolicyName()==newPolicy))
    error("Cannot change selection policy");
    else{
        if(newPolicy=="nve")
        (simulation.getPlan(planId)).setSelectionPolicy(new NaiveSelection());
        else if(newPolicy=="eco")
        (simulation.getPlan(planId)).setSelectionPolicy(new EconomySelection());
        else if(newPolicy=="env")
        (simulation.getPlan(planId)).setSelectionPolicy(new SustainabilitySelection());
        
        else {
            int lifeS=(simulation.getPlan(planId)).getlifeQualityScore()+(simulation.getPlan(planId)).underConstructionLife();
            int ecoS=(simulation.getPlan(planId)).getEconomyScore()+(simulation.getPlan(planId)).underConstructionEco();
            int envS=(simulation.getPlan(planId)).getEnvironmentScore()+(simulation.getPlan(planId)).underConstructionEnv();
           (simulation.getPlan(planId)).setSelectionPolicy(new BalancedSelection(lifeS,ecoS,envS));}
           complete();
    }

}
ChangePlanPolicy* ChangePlanPolicy::clone()const{
    return new ChangePlanPolicy(*this);
}
const string ChangePlanPolicy::toString()const{
    if(getStatus()==ActionStatus::COMPLETED)
    return "changePolicy "+std::to_string(planId)+" COMPLETED \n";
    else
    return "changePolicy "+std::to_string(planId)+" ERROR \n";

}




