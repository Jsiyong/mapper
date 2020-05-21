//
// Created by Jsiyong on 2020-05-05.
//

#ifndef MAPPER_RESULTBINDER_HPP
#define MAPPER_RESULTBINDER_HPP

#include <vector>
#include <map>
#include <mysql.h>
#include <functional>
#include <iostream>
#include <deque>

/**
 * 结果集的绑定
 */
class ResultBinder {
private:
    const static int STRING_MAX_LENGTH = 65535;

    std::vector<MYSQL_BIND> resultBinds;

    std::vector<Object> bindValues;//保存的任意类型的缓存

//    std::deque<bool> nulls = {false};//是不是空的

    //类型与处理函数映射
    std::map<int, std::function<void(int)>> typeProcessMap = {
            {MYSQL_TYPE_LONG,       std::bind(&ResultBinder::bindInt, this, std::placeholders::_1)},
            {MYSQL_TYPE_VAR_STRING, std::bind(&ResultBinder::bindString, this, std::placeholders::_1)},
            {MYSQL_TYPE_LONGLONG,   std::bind(&ResultBinder::bindInt, this, std::placeholders::_1)}
    };
private:
    void bindInt(int index) {
        bindValues[index] = std::type_index(typeid(int));

        resultBinds[index].buffer_type = MYSQL_TYPE_LONG;
        resultBinds[index].buffer = bindValues[index].getValuePtr();

        resultBinds[index].is_null = bindValues[index].isNullPtr();
    }

    void bindString(int index) {
        bindValues[index] = std::type_index(typeid(std::string));
        bindValues[index].resize(STRING_MAX_LENGTH);
        resultBinds[index].buffer_type = MYSQL_TYPE_VAR_STRING;
        resultBinds[index].buffer = bindValues[index].getValuePtr();
        resultBinds[index].buffer_length = STRING_MAX_LENGTH;
        resultBinds[index].is_null =  bindValues[index].isNullPtr(); //是不是空的
    }

public:

    /**
     * 绑定的结果集的大小,即指列的数量
     * @param resultBindNum
     */
    explicit ResultBinder(int resultBindNum) {
        resultBinds.resize(resultBindNum);
        bindValues.resize(resultBindNum);
//        nulls.resize(resultBindNum);
    }


    /**
     * 绑定数据库查询结果集
     * @param fieldTypes
     * @param index
     */
    void bindValue(enum_field_types fieldTypes, int index) {
        if (typeProcessMap.find(fieldTypes) == typeProcessMap.end()) {
            std::cerr << "[unsupported type] " << fieldTypes << std::endl;
            return;
        }
        //调用绑定函数
        typeProcessMap[fieldTypes](index);
    }

    /**
    * 提取结果集中的结果
    * @param index
    * @return
    */
    Object value(int index) {
        return bindValues[index];
    }

    std::vector<MYSQL_BIND> &getBindResult() {
        return resultBinds;
    }
};

#endif //MAPPER_RESULTBINDER_HPP
