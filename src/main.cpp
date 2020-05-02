//
// Created by Jsiyong on 2020-05-02.
//


#include <sql/MySql.hpp>

int main() {

    MySql mySql;
    mySql.setOption(DatabaseOption());
    bool res = mySql.connect();
    bool isConnect = mySql.ping();
    mySql.query();

    return 0;
}