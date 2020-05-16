//
// Created by Jsiyong on 2020-05-16.
//

#ifndef MAPPER_ORDERBY_HPP
#define MAPPER_ORDERBY_HPP

#include <string>
#include <map>
#include <MapperException.hpp>
#include "EntityColumn.hpp"
#include "EntityHelper.hpp"
#include "SQLConstants.hpp"

/**
 * 排序用的
 */
class OrderBy : public std::enable_shared_from_this<OrderBy> {
private:
    std::map<std::string, EntityColumn> *propertyMap = nullptr;//属性和列对应
    std::vector<std::string> orderBys;

public:
    explicit OrderBy(std::map<std::string, EntityColumn> *propertyMap) : propertyMap(propertyMap) {}

    OrderBy() = default;

private:


    /**
     * 类的属性值转列名
     * @param property
     * @return
     */
    std::string column(const std::string &property) const {
        if (propertyMap->count(property) > 0) {
            return propertyMap->at(property).getColumnWithTableAlias();
        } else {
            //属性不存在,抛异常
            throw MapperException("[property]" + property + "is not exist!");
        }
    }

public:
    //降序
    template<typename T, typename Entity>
    std::shared_ptr<OrderBy> desc(T Entity::* property) {
        orderBys.emplace_back(column(EntityHelper::getProperty(property)) + " " + SQLConstants::DESC);
        return shared_from_this();
    }

    //升序
    template<typename T, typename Entity>
    std::shared_ptr<OrderBy> asc(T Entity::* property) {
        orderBys.emplace_back(column(EntityHelper::getProperty(property)) + " " + SQLConstants::ASC);
        return shared_from_this();
    }

    const std::vector<std::string> &getOrderBys() const {
        return orderBys;
    }
};

#endif //MAPPER_ORDERBY_HPP
