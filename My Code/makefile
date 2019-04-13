# All Targets
all: rest

# Tool invocations
# Executable "rest" depends on the files bin/Action.o  bin/Customer.o  bin/Dish.o  bin/Restaurant.o  bin/Table.o bin/Main.o.
rest: bin/Main.o bin/Restaurant.o bin/Customer.o bin/Table.o bin/Dish.o bin/Action.o   
	@echo 'Building target: Ass1'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/rest bin/Main.o bin/Restaurant.o bin/Customer.o bin/Table.o bin/Dish.o bin/Action.o
	@echo 'Finished building target: Ass1'
	@echo ' '

# Depends on the source and header files 
bin/Main.o: src/Main.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

# Depends on the source and header files 
bin/Restaurant.o: src/Restaurant.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

# Depends on the source and header files
bin/Action.o: src/Action.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

# Depends on the source and header files
bin/Dish.o: src/Dish.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Dish.o src/Dish.cpp

# Depends on the source and header files 
bin/Customer.o: src/Customer.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp

# Depends on the source and header files
bin/Table.o: src/Table.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Table.o src/Table.cpp

#Clean the build directory
clean: 
	rm -f bin/*
