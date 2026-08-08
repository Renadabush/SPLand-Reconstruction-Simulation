#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Action.h"
#include <fstream>
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        const int getPlanID();
        void step();
        void close();
        void open();
        vector<string> actionsLogVector() const;
        SettlementType settlementType(string settlementT)const;
        FacilityCategory facilityCat(string facilityC) const;
        SelectionPolicy* select(string policy);
        Simulation(const Simulation &other);
        const Simulation& operator=(const Simulation &other);
        ~Simulation();
        Simulation(Simulation&& other);
        const Simulation& operator=(const Simulation &&other);
        

    
    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};