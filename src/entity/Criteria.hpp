//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_CRITERIA_HPP
#define MAPPER_CRITERIA_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <MapperException.hpp>
#include "Criterion.hpp"
#include "EntityColumn.hpp"

/**
 * 通用Example的标准的集合
 */
class Criteria {
private:
    std::vector<Criterion> criteria;//标准的集合
    //属性和列对应
    std::map<std::string, EntityColumn> propertyMap;
private:
    std::string column(const std::string &property) {
        if (propertyMap.find(property) == propertyMap.end()) {
            return propertyMap.get(property).getColumn();
        } else {
            //属性不存在,跑异常
            throw MapperException("[property]" + property + "is not exist!");
        }
    }

private:
    /**
     * 添加一个标准
     * @param condition 条件
     * @param value 属性的值
     * @param property 属性
     */
    template<typename Object>
    void addCriterion(const std::string &condition, const Object &value, const std::string &property) {
        if (property.empty()) {
            std::cerr << "[" << property << "] cannot be null" << std::endl;
            return;
        }
        criteria.emplace_back(new Criterion(condition, value));
    }

public:

    Criteria &andIsNull(const std::string &property) {
//        addCriterion(column(property) + " is null");
//        return (Criteria) this;
    }

    template<typename Object>
    Criteria &andEqualTo(const std::string &property, const Object &value) {

        return *this;
    }

};

#endif //MAPPER_CRITERIA_HPP
