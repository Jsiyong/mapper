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
#include <entity/Object.hpp>
#include <functional>
#include <map>

/**
 * mysql的预处理绑定器
 */
class PrepareBinder {
private:
    std::vector<MYSQL_BIND> paramBinds;//预处理的参数绑定

    //类型与处理函数映射
    std::map<std::type_index, std::function<void(int, const Object &)>> typeProcessMap = {
            {typeid(int),         std::bind(&PrepareBinder::bindIntValue, this, std::placeholders::_1,
                                            std::placeholders::_2)},
            {typeid(std::string), std::bind(&PrepareBinder::bindStringValue, this, std::placeholders::_1,
                                            std::placeholders::_2)}
    };
private:
    /**
     * 绑定整型
     * @param pos
     * @param value
     */
    void bindIntValue(int pos, const Object &value) {
        paramBinds[pos].buffer_type = MYSQL_TYPE_LONG;
        paramBinds[pos].buffer = value.getValuePtr();
        paramBinds[pos].is_unsigned = false;
    }

    /**
     * 绑定无符号整型
     * @param pos
     * @param value
     */
    void bindUIntValue(int pos, const Object &value) {
        paramBinds[pos].buffer_type = MYSQL_TYPE_LONG;
        paramBinds[pos].buffer = value.getValuePtr();
        paramBinds[pos].is_unsigned = true;
    }

    /**
     * 绑定字符串
     * @param pos
     * @param value
     */
    void bindStringValue(int pos, const Object &value) {
        paramBinds[pos].buffer_type = MYSQL_TYPE_STRING;
        paramBinds[pos].buffer = value.getValuePtr();
        paramBinds[pos].buffer_length = value.getValue<std::string>().size();
    }

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

    void bindValue(int pos, const Object &value) {
        if (typeProcessMap.find(value.getTypeIndex()) == typeProcessMap.end()) {
            std::cerr << "[unsupported type]" << value.getTypeIndex().name() << std::endl;
            return;
        }
        //调用绑定函数
        typeProcessMap[value.getTypeIndex()](pos, value);
    }
};

#endif //MAPPER_PREPAREBINDER_HPP
