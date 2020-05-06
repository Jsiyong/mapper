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

/**
 * 结果集的绑定
 */
class ResultBinder {
private:
    /**
     * 负责类型转发
     * @tparam T
     */
    template<typename T>
    struct IndexWrapper {
        int index = 0;//索引

        explicit IndexWrapper(int index) : index(index) {}
    };

private:
    const static int STRING_MAX_LENGTH = 65535;

    std::vector<MYSQL_BIND> resultBinds;

    //字符串缓存
    std::vector<std::vector<char>> stringBuffer;
    //整型的缓存
    std::vector<int> intBuffer;

    //类型与处理函数映射
    std::map<int, std::function<void(int)>> typeProcessMap = {
            {MYSQL_TYPE_LONG,       std::bind(&ResultBinder::bindInt, this, std::placeholders::_1)},
            {MYSQL_TYPE_VAR_STRING, std::bind(&ResultBinder::bindString, this, std::placeholders::_1)}
    };
private:
    void bindInt(int index) {
        resultBinds[index].buffer_type = MYSQL_TYPE_LONG;
        resultBinds[index].buffer = &intBuffer[index];
    }

    void bindString(int index) {
        stringBuffer[index].clear();
        stringBuffer[index].resize(STRING_MAX_LENGTH);
        resultBinds[index].buffer_type = MYSQL_TYPE_STRING;
        resultBinds[index].buffer = stringBuffer[index].data();
        resultBinds[index].buffer_length = STRING_MAX_LENGTH;
    }

    /**
    * 提取结果集中的字符串结果
    * @param index
    * @return
    */
    std::string value(IndexWrapper<std::string> wrapper) {
        return stringBuffer[wrapper.index].data();
    }

    /**
    * 提取结果集中的整型结果
    * @param index
    * @return
    */
    int value(IndexWrapper<int> wrapper) {
        return intBuffer[wrapper.index];
    }

public:

    /**
     * 绑定的结果集的大小,即指列的数量
     * @param resultBindNum
     */
    explicit ResultBinder(int resultBindNum) {
        resultBinds.resize(resultBindNum);
        stringBuffer.resize(resultBindNum);
        intBuffer.resize(resultBindNum);
    }


    /**
     * 绑定数据库查询结果集
     * @param fieldTypes
     * @param index
     */
    void bindValue(enum_field_types fieldTypes, int index) {
        if (typeProcessMap.find(fieldTypes) == typeProcessMap.end()) {
            std::cerr << "[unsupported type]" << fieldTypes << std::endl;
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
    template<typename T>
    T value(int index) {
        return value(std::move(IndexWrapper<T>(index)));
    }

    std::vector<MYSQL_BIND> &getBindResult() {
        return resultBinds;
    }
};

#endif //MAPPER_RESULTBINDER_HPP
