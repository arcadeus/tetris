#pragma once
#include "simpleini/SimpleIni.h"
#include <mysql.h> 

class Db
{
    MYSQL* m_conn = nullptr;

public:
    Db(CSimpleIniA& config);
    virtual ~Db();
    void query(const std::string& sql);
    MYSQL_RES* get_result();
};
