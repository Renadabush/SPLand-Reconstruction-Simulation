#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include <iostream>
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions,vector<Facility*>facility,vector<Facility*>,int life_quality_score,int economy_score,int environment_score);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const string swithStatus(PlanStatus p) const;
        const std::vector<string> planStatus()const;
        const string getPolicyName()const;
        const string getSettlementName()const;
        vector<Facility*> cloneFacility();
        vector<Facility*> cloneFacilityUnder();
        SelectionPolicy* getSelectionPolicy();
        ~Plan();
        int getPlanId ()const;
        Plan(const Plan &other);
        Plan(Plan &&other);
        Plan& operator=(const Plan& other) = delete;
        const int underConstructionLife()const;
        const int underConstructionEco() const;
        const int underConstructionEnv() const;

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};