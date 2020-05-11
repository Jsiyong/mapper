//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_ENTITYCOLUMN_HPP
#define MAPPER_ENTITYCOLUMN_HPP

#include <string>
#include <typeindex>

/**
 * 属性与数据库列的对应关系
 */
class EntityColumn {
public:
    void *pProperty = nullptr;//实体类字段地址
    std::type_index typeIndex;//实体类字段类型信息
    std::string column = "";//数据库列名
    std::string property = "";//实体类属性名

public:
    template<typename T>
    EntityColumn(T *pProperty, const std::string &property, const std::string &column):typeIndex(typeid(T)) {
        this->pProperty = pProperty;
        this->property = property;
        this->column = column;
    }

    const std::string &getColumn() const {
        return column;
    }

    const std::string &getProperty() const {
        return property;
    }

};

#endif //MAPPER_ENTITYCOLUMN_HPP
