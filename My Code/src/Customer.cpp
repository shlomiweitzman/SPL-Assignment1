#include <utility>
#include "../include/Customer.h"

using namespace std;

//constructor
Customer::Customer(std::string c_name, int c_id) :name(move(c_name)),id(c_id) {}
//methods
string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

Customer::~Customer() {

}


//constructor - 1
VegetarianCustomer::VegetarianCustomer(string name, int id) : Customer(name, id) {}
//methods - 1
vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    vector<int> vegi;
    int tmpid=-1;
    int tmpprice=-1;
    int tmpidByprice=-1;
        for (const Dish &d:menu) {
            if (tmpid == -1 && d.getType() == VEG)
                tmpid = d.getId();
            if (d.getType() == VEG && d.getId() < tmpid)
                tmpid = d.getId();
        }
        if(tmpid!=-1) {
            for (const Dish &d:menu) {
                if (d.getType() == BVG && d.getPrice() > tmpprice) {
                    tmpprice = d.getPrice();
                    tmpidByprice = d.getId();
                }
                if (d.getType() == BVG && d.getPrice() == tmpprice && d.getId() < tmpidByprice) { // Note 2
                    tmpprice = d.getPrice();
                    tmpidByprice = d.getId();
                }
            }
        }
    if(tmpid!=-1&&tmpidByprice!=-1){  // Note 1
        vegi.push_back(tmpid);
    }
    if(tmpidByprice!=-1&&tmpid!=-1){ // Note 1
        vegi.push_back(tmpidByprice);
    }
    return vegi;
}

string VegetarianCustomer::toString() const {
    return getName()+",veg";
}

Customer *VegetarianCustomer::clone() {
    return new VegetarianCustomer(this->getName(),this->getId());
}

VegetarianCustomer::~VegetarianCustomer() {

}

//constructor - 2
CheapCustomer::CheapCustomer(string name, int id) : Customer(name, id),hasordered(false) {}
//methods - 2
vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    vector<int> cheap;
    int tmpprice=-1;
    int tmpid=-1;
    if(!hasordered) {
        for (const Dish &d:menu) {
            if (tmpprice == -1) {
                tmpprice = d.getPrice();
                tmpid = d.getId();
            }

            if (d.getPrice() < tmpprice) {
                tmpprice = d.getPrice();
                tmpid = d.getId();
            }
            if (d.getPrice() == tmpprice && d.getId() < tmpid) { // Note 2
                tmpprice = d.getPrice();
                tmpid = d.getId();
            }
        }
        if (tmpid != -1) { // Note 1
            cheap.push_back(tmpid);
        }
    }
    hasordered=true;
    return cheap;
}

string CheapCustomer::toString() const {
    return getName()+",chp";
}

Customer *CheapCustomer::clone() {
    CheapCustomer* customer = new CheapCustomer(this->getName(),this->getId());
    customer->hasordered=hasordered;
    return customer;
}

CheapCustomer::~CheapCustomer() {

}

//constructor - 3
SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id),alreadyEat(false) {}
//methods - 3
vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> spc;
    int foodPrice = -1;
    int tmpidByfood = -1;
    int drinkPrice = -1;
    int tmpidBydrink = -1;
    bool exist=false;
    if (!alreadyEat) {
        for (const Dish &d:menu) {
            if (foodPrice == -1 && d.getType() == SPC) {
                foodPrice = d.getPrice();
                tmpidByfood = d.getId();
                exist=true;
            }
            if (d.getType() == SPC && d.getPrice() > foodPrice) {
                foodPrice = d.getPrice();
                tmpidByfood = d.getId();
            }
            if (d.getType() == SPC && d.getPrice() == foodPrice && d.getId() < tmpidByfood) { // Note 2
                foodPrice = d.getPrice();
                tmpidByfood = d.getId();
            }
        }
        alreadyEat=true;
    }else {
        for (const Dish &d:menu) {
            if(d.getType()==SPC)
                  exist=true;
            if (drinkPrice == -1 && d.getType() == BVG) {
                drinkPrice = d.getPrice();
                tmpidBydrink = d.getId();
            }
            if (d.getType() == BVG && d.getPrice() < drinkPrice) {
                drinkPrice = d.getPrice();
                tmpidBydrink = d.getId();
            }
            if (d.getType() == BVG && d.getPrice() == drinkPrice && d.getId() < tmpidBydrink) { // Note 2
                drinkPrice = d.getPrice();
                tmpidBydrink = d.getId();
            }
        }
    }
    if(tmpidByfood!=-1){ // Note 1
        spc.push_back(tmpidByfood);
    }
    if((exist)&&(tmpidBydrink!=-1)){ // Note 1
        spc.push_back(tmpidBydrink);
    }
    return spc;
}

string SpicyCustomer::toString() const {
    return getName()+",spc";
}

Customer *SpicyCustomer::clone() {
    SpicyCustomer* customer = new SpicyCustomer(this->getName(),this->getId());
    customer->alreadyEat=this->alreadyEat;
    return customer;
}

SpicyCustomer::~SpicyCustomer() {

}

//constructor - 4
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name, id),prevPrice(-1),prevId(-1) {}
//methods - 4
vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    vector<int> Alco;
    int alcPrice = -1;
    int alcId = -1;
    for(const Dish &d:menu){
        if(d.getType() == ALC && d.getPrice() == prevPrice && d.getId() > prevId){
            alcPrice = d.getPrice();
            alcId = d.getId();
        }
    }
    if(alcId != -1) {
        for (const Dish &d:menu) {
            if (d.getType() == ALC && d.getPrice() == prevPrice) {
                if (d.getId() < alcId && d.getId() > prevId) {
                    alcPrice = d.getPrice();
                    alcId = d.getId();
                }
            }
        }
    }else{
        for (const Dish &d:menu) {
            if (d.getType() == ALC && d.getPrice() > prevPrice) {
                alcPrice = d.getPrice();
                alcId = d.getId();
            }
        }
        for (const Dish &d:menu) {
            if (d.getType() == ALC && d.getPrice() < alcPrice && d.getPrice() > prevPrice) {
                alcPrice = d.getPrice();
                alcId = d.getId();
            }
        }
        for (const Dish &d:menu) {
            if (d.getPrice() == alcPrice && d.getId() < alcId) { // Note 2
                alcPrice = d.getPrice();
                alcId = d.getId();
            }
        }
    }
    if(alcId!=-1) {
        prevPrice = alcPrice;
        prevId = alcId;
    }
    if(alcId != -1) { // Note 1
        Alco.push_back(alcId);
    }
    return Alco;
}

string AlchoholicCustomer::toString() const {
    return getName()+",alc";
}

Customer *AlchoholicCustomer::clone() {
    AlchoholicCustomer* customer = new AlchoholicCustomer(this->getName(),this->getId());
    customer->prevPrice=this->prevPrice;
    customer->prevId=this->prevId;
    return customer;
}

AlchoholicCustomer::~AlchoholicCustomer() {

}

