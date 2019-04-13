#include <utility>
#include "../include/Action.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"

using namespace std;

extern Restaurant* backup;
using namespace std;

BaseAction::BaseAction():errorMsg(),status(PENDING) {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
status=COMPLETED;
}

void BaseAction::error(std::string errorMsg1) {
    status=ERROR;
    errorMsg=errorMsg1;
    cout << errorMsg<<endl;

}


string BaseAction::getStatusString(ActionStatus status) const {
    string s;
    if(status==PENDING)
        s="Pending";
    else if(status==COMPLETED)
        s="Completed";
    else if(status==ERROR)
        s=errorMsg;
    return s;
}

BaseAction::~BaseAction() {}

void BaseAction::setStatus(ActionStatus status) {
    BaseAction::status = status;
}

OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):tableId(id), customers(customersList) {}

BaseAction *OpenTable::clone() const {
    return new OpenTable(*this);
}

void OpenTable::act(Restaurant &restaurant) {
    Table *t=restaurant.getTable(tableId);
    if(t != nullptr &&!(t->isOpen())&& t->getCapacity()>=(int)customers.size()) {
        t->openTable();
        for (Customer *c:customers) {
            t->addCustomer(c);
        }
        complete();
    }
    else error("Error: Table does not exist or is already open");
}

std::string OpenTable::toString() const {
    string s="open "+to_string(tableId);
    for(Customer *c:customers) s=s+" "+c->toString();
    s=s+" "+getStatusString(getStatus());
    return s;
}


// Rule of 5 >>> (without the assignments)

//destructor
OpenTable::~OpenTable() {
    for(Customer *c:customers)
        delete(c);
    customers.clear();
}

//copy constructor
OpenTable::OpenTable(const OpenTable &other) :tableId(other.tableId){
    setStatus(other.getStatus());
    for(Customer* customer : other.customers)
        customers.push_back(customer->clone());
}
//move constructor
OpenTable::OpenTable(OpenTable &&other) noexcept : tableId(other.tableId), customers(move(other.customers)) {}

Order::Order(int id) :tableId(id) {}

void Order::act(Restaurant &restaurant) {
    if(restaurant.getTable(tableId)!= nullptr&&restaurant.getTable(tableId)->isOpen()) {
        Table *t = restaurant.getTable(tableId);
        int index=0;
        if((int)t->getOrders().size()>0)
           index=(int)t->getOrders().size();
        t->order(restaurant.getMenu());
        for(int i=index;i<(int)t->getOrders().size();i++) {
            cout<<  t->getCustomer(t->getOrders()[i].first)->getName() +
                " ordered " + (t->getOrders()[i].second).getName() << endl;
        }
        complete();
    }
    else error("Error: Table does not exist or is not open");
}

std::string Order::toString() const {
    string s="order "+to_string(tableId)+" "+getStatusString(getStatus());
    return s;
}

BaseAction *Order::clone() const {
    return new Order(*this);
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) :
srcTable(src), dstTable(dst), id(customerId){}
void MoveCustomer::act(Restaurant &restaurant) {
    Table *tsrc = restaurant.getTable(srcTable);
    Table *tdst = restaurant.getTable(dstTable);
    if(((tsrc==nullptr)|
    (tdst== nullptr))||(
    (!tsrc->isOpen())|
    (!tdst->isOpen())|
    (tdst->getCapacity()<=(int)tdst->getCustomers().size())|
    (tsrc->getCustomer(id)== nullptr))) {
        error("Error: Cannot move customer");
    }
    else {
        Customer *c = (tsrc->getCustomer(id));
        vector<OrderPair>::iterator iterOrder = tsrc->getOrders().begin();
        vector<OrderPair> orders=vector<OrderPair>();
        for (;iterOrder != tsrc->getOrders().end();++iterOrder){
            if (iterOrder->first == id) {
                OrderPair pair = *iterOrder;
                tdst->getOrders().push_back(pair);
            }
            else
                orders.push_back(*iterOrder);
        }
        tsrc->getOrders().clear();
        for(OrderPair o:orders)
              tsrc->getOrders().push_back(o);

        tsrc->removeCustomer(id);
        tdst->addCustomer(c);
        delete(c);
        if (tsrc->getCustomers().empty()) {
            tsrc->closeTable();

        }
        complete();
    }
}

std::string MoveCustomer::toString() const {
    return "move "+to_string(srcTable)+" "+to_string(dstTable)+" "+to_string(id)+" "+
    getStatusString(getStatus());
}

BaseAction *MoveCustomer::clone() const {
    return new MoveCustomer(*this);
}

Close::Close(int id): tableId(id){}

void Close::act(Restaurant &restaurant) {
    if(restaurant.getTable(tableId)== nullptr|| !restaurant.getTable(tableId)->isOpen())
        error("Error: Table does not exist or is not open");
    else {
        cout << "Table "+to_string(tableId)+" was closed. Bill "
        +to_string(restaurant.getTable(tableId)->getBill())+"NIS"<<endl;
        restaurant.getTable(tableId)->closeTable();
        complete();
    }
}

std::string Close::toString() const {
    return "close "+to_string(tableId)+" "+getStatusString(getStatus());
}

BaseAction *Close::clone() const {
    return new Close(*this);
}

CloseAll::CloseAll() {

}

void CloseAll::act(Restaurant &restaurant) {
for(int i=0;i<restaurant.getNumOfTables();i++) {
    if (restaurant.getTable(i)->isOpen()) {
        cout << "Table " + to_string(i) + " was closed. Bill " +
            to_string(restaurant.getTable(i)->getBill()) + "NIS"<<endl;
        restaurant.getTable(i)->closeTable();
    }
}
    complete();
}

std::string CloseAll::toString() const {
    return "closeall "+getStatusString(getStatus());
}

BaseAction *CloseAll::clone() const {
    return new CloseAll(*this);
}

PrintMenu::PrintMenu() {}

void PrintMenu::act(Restaurant &restaurant) {
    string type;
    for (Dish d:restaurant.getMenu()) {
        if (d.getType() == VEG)
            type = "VEG";
        if (d.getType() == ALC)
            type = "ALC";
        if (d.getType() == BVG)
            type = "BVG";
        if (d.getType() == SPC)
            type = "SPC";
        cout<< d.getName() + " " + type + " " + to_string(d.getPrice())+"NIS"<<endl;
      }
  complete();
}

std::string PrintMenu::toString() const {
    return "menu "+getStatusString(getStatus());
}

BaseAction *PrintMenu::clone() const {
    return new PrintMenu(*this);
}

PrintTableStatus::PrintTableStatus(int id): tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant) {
    Table *t=restaurant.getTable(tableId);
    if(t!= nullptr){
        if(!t->isOpen()){
            cout << "Table "+to_string(tableId)+" status: closed"<<endl;
        }
        else{
            string s="Table "+to_string(tableId)+" status: open\nCustomers:\n";
            for(Customer *c:t->getCustomers()){
                s=s+to_string(c->getId())+" "+c->getName()+"\n";
            }
            s=s+"Orders:\n";
            for(OrderPair o:t->getOrders()){
                s=s+o.second.getName()+" "+to_string(o.second.getPrice())+"NIS "
                        +to_string(o.first)+"\n";
            }
            s=s+"Current Bill: "+to_string(t->getBill())+"NIS";
            cout << s <<endl;

        }
    }
    complete();
}

std::string PrintTableStatus::toString() const {
    return "status "+to_string(tableId)+" "+getStatusString(getStatus());
}

BaseAction *PrintTableStatus::clone() const {
    return new PrintTableStatus(*this);
}


PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Restaurant &restaurant) {
    string s="";
    for(BaseAction *b:restaurant.getActionsLog()){
        if(b->toString()!="")
            s=s+b->toString()+"\n";
    }
    cout <<s ;
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log "+getStatusString(getStatus());
}

BaseAction *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

BackupRestaurant::BackupRestaurant() {}

void BackupRestaurant::act(Restaurant &restaurant) {
    if(backup== nullptr)
        backup = new Restaurant(restaurant);
    else {
        *backup = restaurant;
    }
    complete();
}

std::string BackupRestaurant::toString() const {
    return "backup "+getStatusString(getStatus());;
}

BaseAction *BackupRestaurant::clone() const {
    return new BackupRestaurant(*this);
}

RestoreResturant::RestoreResturant() {}

void RestoreResturant::act(Restaurant &restaurant) {
    if (backup == nullptr)
        error("Error: No backup available");
    else {
        restaurant=*backup;
        complete();
    }

}

std::string RestoreResturant::toString() const {
    return "restore "+getStatusString(getStatus());
}

BaseAction *RestoreResturant::clone() const {
    return new RestoreResturant(*this);
}

