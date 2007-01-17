#!/bin/sh
#Dependencies: mysqlclient mysqlpp
 g++ -o test -L/opt/mysql-4.1.7/lib/mysql/ -lmysqlclient -L/opt/mysql++-1.7.28/lib/ -lmysqlpp main.cpp Database.cpp -I/opt/mysql-4.1.7/include/mysql/ -I/opt/mysql++-1.7.28/include/ 

