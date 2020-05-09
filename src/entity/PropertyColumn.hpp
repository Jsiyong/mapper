//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_PROPERTYCOLUMN_HPP
#define MAPPER_PROPERTYCOLUMN_HPP

#include <string>
#include <typeindex>

/**
 * 属性与数据库列的对应关系
 */
class PropertyColumn {
public:
    void *pProperty = nullptr;//实体类字段地址
    std::type_index typeIndex;//实体类字段类型信息


    std::string columnName = "";//数据库列名
    std::string propertyName = "";//实体类属性名

public:
    template<typename T>
    PropertyColumn(T *pProperty, const std::string &propertyName, const std::string &columnName):typeIndex(typeid(T)) {
        this->pProperty = pProperty;
        this->propertyName = propertyName;
        this->columnName = columnName;
    }

};

#endif //MAPPER_PROPERTYCOLUMN_HPP
