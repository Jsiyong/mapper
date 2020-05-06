//
// Created by Jsiyong on 2020-05-05.
//

#ifndef MAPPER_PREPAREBINDER_HPP
#define MAPPER_PREPAREBINDER_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <mysql.h>
#include <iostream>

/**
 * mysql的预处理绑定器
 */
class PrepareBinder {
private:
    std::vector<MYSQL_BIND> paramBinds;//预处理的参数绑定
public:

    /**
     * 绑定的参数集的大小
     * @param resultBindNum
     */
    explicit PrepareBinder(int prepareBindNum) {
        paramBinds.resize(prepareBindNum);//初始化绑定参数列表
    }

    std::vector<MYSQL_BIND> &getParamBinds() {
        return paramBinds;
    }

    /**
     * 绑定整型
     * @param pos
     * @param value
     */
    void bindValue(int pos, const int &value) {
        paramBinds[pos].buffer_type = MYSQL_TYPE_LONG;
        paramBinds[pos].buffer = (void *) &value;
        paramBinds[pos].is_unsigned = false;
    }

    /**
     * 绑定无符号整型
     * @param pos
     * @param value
     */
    void bindValue(int pos, const unsigned int &value) {
        paramBinds[pos].buffer_type = MYSQL_TYPE_LONG;
        paramBinds[pos].buffer = (void *) &value;
        paramBinds[pos].is_unsigned = true;
    }

    /**
     * 绑定字符串
     * @param pos
     * @param value
     */
    void bindValue(int pos, const std::string &value) {
        paramBinds[pos].buffer_type = MYSQL_TYPE_STRING;
        paramBinds[pos].buffer = (void *) value.c_str();
        paramBinds[pos].buffer_length = value.size();
    }
};

#endif //MAPPER_PREPAREBINDER_HPP
