#include <fstream>
#include<iostream>
#include"Simulation.h"
#include"Auxiliary.h"
Simulation::Simulation(const string &configFilePath):isRunning(false),planCounter(0),actionsLog(),plans(),settlements(),facilitiesOptions(){
    std::ifstream file(configFilePath);
    string line;
    while (std::getline(file, line)) {
        std::vector<string> p=Auxiliary::parseArguments(line);
        /*for(size_t i=0;i<p.size();i=i+1)
        std::cout<<p.at(i)<<std::endl;*/
        if((!p.empty())&&(p.at(0)!="#")){
            if(p.at(0)=="settlement")
            addSettlement(new Settlement((p.at(1)),settlementType(p.at(2))));
            else if (p.at(0)=="facility")
            addFacility(FacilityType(p.at(1),facilityCat(p.at(2)),std::stoi(p.at(3)),std::stoi(p.at(4)),std::stoi(p.at(5)),std::stoi(p.at(6))));
            else if(p.at(0)=="plan")
            addPlan(getSettlement(p.at(1)),select(p.at(2)));
             }
    }
}

Simulation::Simulation(const Simulation &other):isRunning(other.isRunning),planCounter(other.planCounter),actionsLog(),plans(),settlements(),facilitiesOptions(other.facilitiesOptions){
    for(BaseAction* action:other.actionsLog){
        actionsLog.push_back(action->clone());
    }
    for(Settlement* sett:other.settlements){
        settlements.push_back(new Settlement(*sett));
    }
    
    for(Plan p: other.plans){
        const Settlement& settlement=getSettlement(p.getSettlementName());
        int id=p.getPlanId();
        vector<Facility*>facilities=p.cloneFacility();
        vector<Facility*>underConstruction=p.cloneFacilityUnder();
        Plan out=Plan(id,settlement,(p.getSelectionPolicy())->clone(),facilitiesOptions,facilities,underConstruction,p.getlifeQualityScore(),p.getEconomyScore(),p.getEnvironmentScore());
        plans.push_back(out);
        }
        }
Simulation::Simulation( Simulation &&other):isRunning(other.isRunning),planCounter(other.planCounter),actionsLog(),plans(other.plans),settlements(),facilitiesOptions(other.facilitiesOptions){
    for(BaseAction* action:other.actionsLog){
        actionsLog.push_back(action);
        action=nullptr;
    }
    (other.actionsLog).clear();
    for(Settlement* sett:other.settlements){
        settlements.push_back(sett);
        sett=nullptr;
    }
    (other.settlements).clear();


}

const Simulation& Simulation::operator=(const Simulation &other){
    if(&other!=this){
    isRunning=other.isRunning;
    planCounter=other.planCounter;
    for(BaseAction* action:actionsLog){
        if(action)
        delete action;
    }
    
    actionsLog.clear();
    for(BaseAction* action:other.actionsLog){
        actionsLog.push_back(action->clone());
    }
   for(Settlement* sett: settlements){
    if(sett)
    delete sett;
   }
   settlements.clear();
    for(Settlement* sett:other.settlements){
        settlements.push_back(new Settlement(*sett));
    }
    facilitiesOptions.clear();
    for(FacilityType fac: other.facilitiesOptions){
        facilitiesOptions.push_back(fac);
    }
    plans.clear();
     for(Plan p: other.plans){
        const Settlement& settlement=getSettlement(p.getSettlementName());
        int id=p.getPlanId();
        vector<Facility*>facilities=p.cloneFacility();
        vector<Facility*>underConstruction=p.cloneFacilityUnder();
        Plan out=Plan(id,settlement,(p.getSelectionPolicy())->clone(),facilitiesOptions,facilities,underConstruction,p.getlifeQualityScore(),p.getEconomyScore(),p.getEnvironmentScore());
        plans.push_back(out);
        }
    }
     return *this;
}
const Simulation& Simulation::operator=(const Simulation &&other){
    if(&other!=this){
    isRunning=other.isRunning;
    planCounter=other.planCounter;
     for(BaseAction* action:actionsLog){
        if(action)
        delete action;
    }
    
    actionsLog.clear();
     for(BaseAction* action:other.actionsLog){
        actionsLog.push_back(action);
        action=nullptr;
    }
    for(Settlement* sett: settlements){
    if(sett)
    delete sett;
   }
    settlements.clear();
     for(Settlement* sett:other.settlements){
        settlements.push_back(sett);
        sett=nullptr;
    }
    plans.clear();
    for(Plan p:other.plans){
        plans.push_back(p);
    }
    
   
    facilitiesOptions.clear();
     for(FacilityType fac: other.facilitiesOptions){
        facilitiesOptions.push_back(fac);
    }
    }
     return *this;
}
Simulation::~Simulation(){
    for(BaseAction* action:actionsLog){
        if(action)
        delete action;
    }
    for(Settlement* sett:settlements){
        if(sett)
        delete sett;
    }
    
}

 vector<string> Simulation::actionsLogVector()const{
    vector<string> output;
    for(BaseAction* a:actionsLog)
    output.push_back(a->toString());
    return output;

}
Plan& Simulation::getPlan(const int planID){
    return plans.at(planID);
}
void Simulation::start(){
    std::cout<<"The simulation has started"<<std::endl;
    open();
    string inputLine;
    
    while(isRunning){
        getline(std::cin,inputLine);
        vector<string>temp=Auxiliary::parseArguments(inputLine);
       if(temp.at(0)=="step"){
        SimulateStep* a=new SimulateStep(std::stoi(temp.at(1)));
        a->act(*this);
        addAction(a);
       }
       else if(temp.at(0)=="plan"){
        AddPlan* a=new AddPlan(temp.at(1),temp.at(2));
        a->act(*this);
        addAction(a);
       }
       else if(temp.at(0)=="settlement"){
        SettlementType aux=settlementType(temp.at(2));
        AddSettlement* a=new AddSettlement(temp.at(1),aux);
        a->act(*this);
        addAction(a);
       }
       else if(temp.at(0)=="facility"){
        
        AddFacility* a=new AddFacility(temp.at(1),facilityCat(temp.at(2)),std::stoi(temp.at(3)),stoi(temp.at(4)),stoi(temp.at(5)),stoi(temp.at(6)));
        a->act(*this);
        addAction(a);
       }
       else if(temp.at(0)=="planStatus"){
        PrintPlanStatus* a=new PrintPlanStatus(std::stoi(temp.at(1)));
        a->act(*this);
        addAction(a);

       }
       else if(temp.at(0)=="log"){
        PrintActionsLog*a=new PrintActionsLog();
        a->act(*this);
        addAction(a);
       }
       else if(temp.at(0)=="close"){
        Close* a=new Close();
        a->act(*this);
        addAction(a);

       }
       else if(temp.at(0)=="backup"){
        BackupSimulation* a=new BackupSimulation();
        a->act(*this);
        addAction(a);
       }
       else if(temp.at(0)=="restore"){
        RestoreSimulation* a=new RestoreSimulation();
        a->act(*this);
        addAction(a);
       }
       else{
        ChangePlanPolicy*a=new ChangePlanPolicy(std::stoi(temp.at(1)),temp.at(2));
        a->act(*this);
        addAction(a);


       }
    }
    
}
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
  plans.push_back(Plan(planCounter,settlement,selectionPolicy,facilitiesOptions));
   planCounter=planCounter+1;
}
bool Simulation::addFacility(FacilityType facility){
for(FacilityType fac:facilitiesOptions){
    if((fac.getName())==facility.getName())
    return false;
}
facilitiesOptions.push_back(facility);
return true;
}
bool Simulation::addSettlement(Settlement *settlement){
    if (isSettlementExists(settlement->getName()))
    return false;
    else{
    settlements.push_back(settlement);
    return true;}

}
void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}
bool Simulation::isSettlementExists(const string &settlementName){
    for(Settlement* sett:settlements){
        if(sett->getName()==settlementName)
        return true;
    }
    return false;
}
Settlement& Simulation::getSettlement(const string &settlementName){
    for(Settlement* sett:settlements){
        if(sett->getName()==settlementName)
        return (*(sett));}
return (*(settlements.at(0)));

}
void Simulation::close(){
    isRunning=false;

}
void Simulation::open(){
    isRunning=true;

}
SettlementType Simulation::settlementType(string settlementT)const{
    if (settlementT=="0")
    return SettlementType::VILLAGE;
    else if (settlementT=="1")
    return SettlementType::CITY;
    return SettlementType::METROPOLIS;
}
FacilityCategory Simulation::facilityCat(string facilityC)const{
    if (facilityC=="0")
    return FacilityCategory::LIFE_QUALITY;
    else if (facilityC=="1")
    return FacilityCategory::ECONOMY;
    return FacilityCategory::ENVIRONMENT;
}
SelectionPolicy* Simulation::select(string policy){
    SelectionPolicy* answer;
    if(policy=="nve")
    answer=new NaiveSelection();
    else if(policy=="bal")
    answer=new BalancedSelection(0,0,0);
    else if(policy=="eco")
    answer=new EconomySelection();
    else
    answer=new SustainabilitySelection();
    return answer;
}
const int Simulation::getPlanID(){
    return planCounter;
}
void Simulation::step(){
    for(int i=0;i<planCounter;i=i+1){
        getPlan(i).step();
    }
}