#include"Settlement.h"
Settlement::Settlement(const string &name, SettlementType type):name(name),type(type){}
const string& Settlement ::getName() const {return name;}
SettlementType Settlement::getType() const {return type;}
const string Settlement::toString() const{ return getName();}
const int Settlement::typeInt() const{
    if (getType()==SettlementType::VILLAGE)
    return 1;
    else if(getType()==SettlementType::CITY)
    return 2;
    return 3;
}




