#include <iostream>
#include <fstream>
#include "../include/Action.h"
#include "../include/Dish.h"
#include <vector>
#include "../include/Restaurant.h"


using namespace std;
//constructor
Restaurant::Restaurant() :open(false){}

Restaurant::Restaurant(const std::string &configFilePath): open(false),tables(),menu(),actionsLog(){
    ifstream myFile(configFilePath);
    string line;
    int numOftable=0;
    int id=0;
    string tmpline;
    int count=1;

    while(getline(myFile,line)) {

        if(line[0]=='#'||line[0]==' '||line=="") {

        }
        else if((count==1)&&(line[0]>='0')&(line[0]<='9')){
            numOftable=stoi(line);
            count++;
        }
        else if((count==2)&&(line[0]>='0')&(line[0]<='9')) {
            for (int i = 0; i < numOftable; i++) {
                int p = (int)line.find(',');
                Table *t = new Table(stoi(line.substr(0, p)));
                tables.push_back(t);
                line = line.substr(p+1);
            }
            count++;
        }

        else if((count>=3)&&(((line[0]>='a')&(line[0]<='z'))|((line[0]>='A')&(line[0]<='z')))){
            id++;
            int price;
            string name;
            DishType dtype;

            int p = (int)line.find(',');
            name=line.substr(0,p);
            line = line.substr(p+1);
            p=(int)line.find(',');
            string type=line.substr(0,p);
            if(type=="VEG")
                dtype=VEG;
            if(type=="ALC")
                dtype=ALC;
            if(type=="SPC")
                dtype=SPC;
            if(type=="BVG")
                dtype=BVG;
            line=line.substr(p+1);
            price=stoi(line);

            Dish dish=Dish(id,name,price,dtype );
            menu.push_back(dish);
        }
    }
}

void Restaurant::start() {
    open=true;
    cout<<"Restaurant is now open!"<<endl;
    int id=0;
    string myLine;
    string myAction;
    while(myLine != "closeall") {
        getline(cin, myLine);
        int ind = myLine.find(" ");
        if (ind != -1) {
            myAction = myLine.substr(0, ind);
        } else {
            myAction = myLine;
        }
        if (myAction == "open") {
            myLine = myLine.substr(ind + 1);
            int nextInd = myLine.find(" ");
            int tableId = stoi(myLine.substr(0, nextInd));
            vector<Customer *> list;
            ind = nextInd;
            myLine = myLine.substr(ind);
            while (nextInd != -1) {
                nextInd = myLine.find(",");
                string name = myLine.substr(1, nextInd - 1);
                myLine = myLine.substr(nextInd + 1);
                ind = 0;
                string type = myLine.substr(0, 3);
                myLine = myLine.substr(3);
                nextInd = myLine.find(" ");
                if (myLine != "")
                    nextInd = 0;
                if (type == "veg") {
                    list.push_back(new VegetarianCustomer(name, id));
                }
                if (type == "spc") {
                    list.push_back(new SpicyCustomer(name, id));
                }
                if (type == "chp") {
                    list.push_back(new CheapCustomer(name, id));
                }
                if (type == "alc") {
                    list.push_back(new AlchoholicCustomer(name, id));
                }
                id++;
            }
            BaseAction *open = new OpenTable(tableId, list);
            open->act(*this);
            actionsLog.push_back(open);
        } else if (myAction == "order") {
            int tableId = stoi(myLine.substr(ind));
            BaseAction *order = new Order(tableId);
            order->act(*this);
            actionsLog.push_back(order);
        } else if (myAction == "move") {
            int originTableId = stoi(myLine.substr(5, 6));

            int destTableId = stoi(myLine.substr(7, 8));

            int customerId = stoi(myLine.substr(9, 10));
            BaseAction *move = new MoveCustomer(originTableId, destTableId, customerId);
            move->act(*this);
            actionsLog.push_back(move);
        } else if (myAction == "close") {
            int tableId = stoi(myLine.substr(ind, (int)myLine.size()));
            BaseAction *close = new Close(tableId);
            close->act(*this);
            actionsLog.push_back(close);
        } else if (myAction == "menu") {
            BaseAction *menu = new PrintMenu();
            menu->act(*this);
            actionsLog.push_back(menu);
        } else if (myAction == "status") {
            int tableId = stoi(myLine.substr(ind, (int)myLine.size()));
            BaseAction *status = new PrintTableStatus(tableId);
            status->act(*this);
            actionsLog.push_back(status);
        } else if (myAction == "log") {
            BaseAction *log = new PrintActionsLog();
            log->act(*this);
            actionsLog.push_back(log);
        } else if (myAction == "backup") {
            BaseAction *backup_action = new BackupRestaurant();
            backup_action->act(*this);
            actionsLog.push_back(backup_action);
        } else if (myAction == "restore") {
            BaseAction *restore = new RestoreResturant();
            restore->act(*this);
            actionsLog.push_back(restore);
        }
    }
    BaseAction *closeall = new CloseAll();
    closeall->act(*this);
    actionsLog.push_back(closeall);
}

int Restaurant::getNumOfTables() const {
    return (int)tables.size();
}

Table *Restaurant::getTable(int ind) {
    if(ind>(int)(tables.size()))
        return nullptr;
    return tables[ind];
}

const std::vector<BaseAction *> &Restaurant::getActionsLog() const {
   return actionsLog;
}

vector<Dish> &Restaurant::getMenu() {
    return menu;
}


// RULE OF 5 >>>>>


//destrcutor
Restaurant::~Restaurant() {
    for(Table *t:tables) delete t;
    tables.clear();
    menu.clear();
    for(BaseAction *b:actionsLog) delete b;
    actionsLog.clear();
}
//copy constructor
Restaurant::Restaurant(const Restaurant &other): open(other.open),tables(),menu(),actionsLog(){
    for(Table *t :other.tables)
        tables.push_back(new Table(*t));
    for(const Dish d: other.menu)
        menu.push_back(Dish(d));
    for(BaseAction *BA: other.actionsLog)
        actionsLog.push_back(BA->clone());
}
//copy assignment operator
Restaurant &Restaurant::operator=(const Restaurant &other) {
    if(this != &other) {
        clean();
        copy(other);
    }
    return *this;
}
//move constructor
Restaurant::Restaurant(Restaurant &&other) noexcept : open(other.open),tables(move(other.tables)),
menu(move(other.menu)),actionsLog(move(other.actionsLog)) {}
//move assignment operator
Restaurant &Restaurant::operator=(Restaurant &&other) noexcept {
    if(this != &other){
        open = other.open;
        for (int i = 0; i < (int)tables.size(); i++) delete tables[i];
        tables=move(other.tables);
        menu = move(other.menu);
        for (int i = 0; i < (int)actionsLog.size(); i++) delete actionsLog[i];
        actionsLog=move(actionsLog);
    }
    return *this;
}

void Restaurant::clean() {
    this->open=false;
    this->menu.clear();
    for (Table* t : tables)
        delete t;
    tables.clear();
    for (int i=0;i<(int)actionsLog.size();i++)
    {
        delete actionsLog.at(i);
    }
    actionsLog.clear();
}

void Restaurant::copy(const Restaurant &other) {
    this->open=other.open;
    for(Table* t : other.tables) {
        Table *t2 = new Table(*t);
        this->tables.push_back(t2);
    }
    for(Dish d : other.menu)
        this->menu.push_back(d);
    for(int i=0;i<(int)other.actionsLog.size();i++)
    {
        this->actionsLog.push_back(other.actionsLog[i]->clone());
    }
}





