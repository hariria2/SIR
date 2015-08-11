//
//  SQLStorage.cpp
//  SIR
//
//  Created by Sahand Hariri on 7/30/15.
//  Copyright (c) 2015 Sahand Hariri. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <vector>
#include <fstream>
#include <iomanip>
#include "SQLStorage.h"


using namespace std;

SQLStorage::SQLStorage(const char *server, const char *user, const char *password, const char *database, string version){
    
    //===========>>>Figure out the time stamp<<<======================//
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    timeStamp = to_string((now->tm_mon + 1)) + "_" +
    to_string(now->tm_mday) + "_" +
    to_string((now->tm_year + 1900));
    
    //================================================================//
    
    
    //=============>>>Start MySQL conection<<<========================//
    mysql_init(&mysql);
    conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
    if(conn==NULL)
    {
        cout<<mysql_error(&mysql)<<endl<<endl;
        
    }
    //=======================================================================//
    
    string dbname;
    dbname = "sim_v" + version + "_" + timeStamp;
    
    CreateDB(dbname);
    CreateTable("Location");
    CreateTable("HistoryData");
    CreateTable("People");
    CreateTable("PersonValues");
    
    
}

void SQLStorage::CreateDB(string db){
    string purgedb="DROP DATABASE IF EXISTS "+db+";";
    string mydb="CREATE DATABASE "+db+";";
    string sdb ="USE "+db+";";
    
    int query_statep;
    int query_statem;
    int query_states;
    
    query_statep=mysql_query(conn, purgedb.c_str());
    if(query_statep!=0){
        cout<<mysql_error(conn)<<endl<<endl;
    }
    query_statem=mysql_query(conn, mydb.c_str());
    if(query_statem!=0){
        cout<<mysql_error(conn)<<endl<<endl;
    }
    query_states=mysql_query(conn, sdb.c_str());
    if(query_states!=0){
        cout<<mysql_error(conn)<<endl<<endl;
    }
}
void SQLStorage::CreateTable(string table){
    
    string statement;
    int query_states;
    
    if (table == "Location"){
        statement = "CREATE TABLE " +table+ "(" +
        "ID INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY, "+
        "name VARCHAR(30) NOT NULL, " +
        "type VARCHAR(30) NOT NULL, " +
        "xmin INT NOT NULL, " +
        "xmax INT NOT NULL, " +
        "ymin INT NOT NULL, " +
        "ymax INT NOT NULL" +
        ");";
        
    }
    else if (table == "HistoryData"){
        statement = "CREATE TABLE " +table+ "(" +
        "ID INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY, " +
        "Time DOUBLE UNSIGNED NOT NULL, " +
        "Susceptible INT NOT NULL, " +
        "Exposed     INT UNSIGNED NOT NULL, " +
        "Infected    INT UNSIGNED NOT NULL, " +
        "Recovered   INT UNSIGNED NOT NULL, " +
        "Dead        INT UNSIGNED NOT NULL, " +
        "GDP         DOUBLE UNSIGNED NOT NULL" +
        ");";
        
    }
    else if (table == "People"){
        statement = "CREATE TABLE " +table+ "(" +
        "ID INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY, " +
        "name VARCHAR(20) NOT NULL, " +
        "age INT NOT NULL, " +
        "gender ENUM('M','F') NOT NULL, " +
        "home INT UNSIGNED NOT NULL REFERENCES Location(ID), " +
        "location INT UNSIGNED NOT NULL REFERENCES Location(ID)"
        ");";
        
    }
    else if ((table == "PersonValues")){
        statement = "CREATE TABLE " +table+ "(" +
        "INDX INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY, " +
        "PersonID INT UNSIGNED NOT NULL REFERENCES People(ID), " +
        "Time DOUBLE UNSIGNED NOT NULL, " +
        "x DOUBLE NOT NULL, " +
        "y DOUBLE NOT NULL, " +
        "LocationID INT UNSIGNED NOT NULL REFERENCES Location(ID), " +
        "State CHAR(1) NOT NULL, " +
        "HasBeenSick INT NOT NULL, " +
        "SusCells  DOUBLE NOT NULL DEFAULT 0, " +
        "InfCells  DOUBLE NOT NULL DEFAULT 0, " +
        "VirLoads  DOUBLE NOT NULL DEFAULT 0, " +
        "MaxInfLev DOUBLE NOT NULL DEFAULT 0"
        ");";
    }
    else if (table == "SIConnection") {
        statement = "CREATE TABLE " +table+ "(" +
        "INDX INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY, " +
        "Time DOUBLE UNSIGNED NOT NULL, " +
        "Host INT UNSIGNED NOT NULL REFERENCES People(ID), " +
        ");";
    }
    else {
        cout << "Wrong table name you dummy!!";
    }
    
    query_states = mysql_query(conn, statement.c_str());
    if(query_states!=0){
        cout<<mysql_error(conn)<<endl<<endl;
    }
}
void SQLStorage::InsertValue(string table, string vals, bool paren){
    string statement;
    int query_states;
    
    
    if (paren){
        statement = "INSERT INTO "+table+" VALUES " + vals +";";
    } else {
        statement = "INSERT INTO "+table+" VALUES (" + vals +");";
    }
    // cout << statement << endl;
    
    query_states = mysql_query(conn, statement.c_str());
    
    if(query_states!=0){
        cout<<mysql_error(conn)<<endl<<endl;
    }
}

SQLStorage::~SQLStorage(){
    mysql_close(conn);
}
