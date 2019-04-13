#include <iostream>
#include <algorithm>
#include "../include/Table.h"
#include "../include/Customer.h"

using namespace std;

Table::Table(int t_capacity) :capacity(t_capacity),open(false),customersList(),orderList(){
}

int Table::getCapacity() const {
    return capacity;
}

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer->clone());
}

void Table::removeCustomer(int id) {
    for(int i=0; i<(int)customersList.size();i++){
        if(customersList[i]->getId()==id) {
            customersList.erase(customersList.begin()+i);
        }
    }
}

Customer *Table::getCustomer(int id) {
    for(Customer *c:customersList){
        if(c->getId()==id)
            return c;
    }
    return nullptr;
}

vector<Customer *> &Table::getCustomers() {
    return customersList;
}

vector<OrderPair> &Table::getOrders() {
    return orderList;
}

void Table::order(const vector<Dish> &menu) {
    if(open) {
        for (Customer *c:customersList) {
            for (int id:c->order(menu)) {
                auto iterator = find_if(menu.begin(), menu.end(),
                       [id](const Dish &d) -> bool { return d.getId() == id; });
                if (iterator != menu.end()) {
                    const Dish &d = (*iterator);
                    orderList.push_back(OrderPair(c->getId(), d));
                }
            }
        }
    }
}

void Table::openTable() {
    open=true;
}

void Table::closeTable() {
    open = false;
    for(int i=0; i<(int)customersList.size();i++) {

            delete customersList[i];

        }

        customersList.clear();
        orderList.clear();
    }

int Table::getBill() {
    int bill=0;
    for(OrderPair o:orderList)
        bill=bill+o.second.getPrice();

    return bill;
}

bool Table::isOpen() {
    return open;
}


// RULE OF 5 >>>>>


//destructor
Table::~Table() {
    for(Customer *c:customersList) delete(c);
    customersList.clear();
    orderList.clear();
}
//copy constructor
Table::Table(const Table &other): capacity(other.capacity),open(other.open),customersList(),orderList() {
    for (Customer *c:other.customersList) {
        Customer* c1=c->clone();
        customersList.push_back(c1);
    }
    for (OrderPair O: other.orderList)
        orderList.push_back(O);
}
//copy assignment operator
Table &Table::operator=(const Table &other) {
    if(this != &other) {
        capacity=other.capacity;
        open = other.open;
        for (int i = 0; i < (int)customersList.size(); i++) delete customersList[i];
        customersList.clear();
        for (int i = 0; i < (int)other.customersList.size(); i++) customersList.push_back(other.customersList[i]->clone());
        orderList.clear();
        for (int i = 0; i < (int)other.orderList.size(); i++) orderList.push_back(other.orderList[i]);
    }
    return *this;
}
//move constructor
Table::Table(Table &&other) noexcept : capacity(other.capacity),open(other.open),
customersList(move(other.customersList)),orderList(move(other.orderList)){}
//move assignment operator
Table &Table::operator=(Table &&other) noexcept {
    if(this != &other) {
        capacity = other.capacity;
        other.capacity = 0;
        open = other.open;
        other.open = false;
        for (int i = 0; i < (int)customersList.size(); i++) delete customersList[i];
        customersList = move(other.customersList);
        orderList = move(other.orderList);
    }
    return *this;
}






