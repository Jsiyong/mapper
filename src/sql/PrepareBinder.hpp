//
// Created by Jsiyong on 2020-05-05.
//

#ifndef MAPPER_PREPAREBINDER_HPP
#define MAPPER_PREPAREBINDER_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <mysql.h>

/**
 * mysql的预处理绑定器
 */
class PrepareBinder {
private:

public:

    static void bindValue(MYSQL_BIND &mysqlBind, const int &value) {
        mysqlBind.buffer_type = MYSQL_TYPE_LONG;
        mysqlBind.buffer = (void *) &value;
        mysqlBind.is_unsigned = false;
    }

    static void bindValue(MYSQL_BIND &mysqlBind, const unsigned int &value) {
        mysqlBind.buffer_type = MYSQL_TYPE_LONG;
        mysqlBind.buffer = (void *) &value;
        mysqlBind.is_unsigned = true;
    }

    static void bindValue(MYSQL_BIND &mysqlBind, const std::string &value) {
        mysqlBind.buffer_type = MYSQL_TYPE_STRING;
        mysqlBind.buffer = (void*)value.c_str();
        mysqlBind.buffer_length = value.size();
    }
};

#endif //MAPPER_PREPAREBINDER_HPP
