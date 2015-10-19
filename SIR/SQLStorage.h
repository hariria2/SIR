//
//  SQLStorage.h
//  SIR
//
//  Created by Sahand Hariri on 7/30/15.
//  Copyright (c) 2015 Sahand Hariri. All rights reserved.
//

#ifndef __SIR__SQLStorage__
#define __SIR__SQLStorage__

#include <iostream>
#include <mysql.h>

using namespace std;


class SQLStorage{
public:
    SQLStorage(const char *server, const char *user, const char *password, const char *database, string version);
    
    virtual ~SQLStorage();
    
    void CreateDB(string db);
    void CreateTable(string table);
    void InsertValue(string table, string vals, bool paren=false);
    
private:
    string timeStamp;
    MYSQL *conn, mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    
};

#endif /* defined(__SIR__SQLStorage__) */
