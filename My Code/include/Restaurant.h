#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"

using namespace std;

class Restaurant{		
public:
	Restaurant(); // constructor
    Restaurant(const std::string &configFilePath);
	virtual ~ Restaurant(); // destructor
    Restaurant(const Restaurant &other); // copy constructor
    Restaurant& operator=(const Restaurant& other); // copy assignment operator
    Restaurant(Restaurant&& other) noexcept; // move constructor
    Restaurant& operator=(Restaurant&& other) noexcept; // move assignment operator
    void clean();
    void copy(const Restaurant& other);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
	const vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();

private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif