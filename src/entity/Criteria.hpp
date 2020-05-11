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
    std::map<std::string, EntityColumn> *propertyMap = nullptr;
private:

    /**
     * 类的属性值转列名
     * @param property
     * @return
     */
    std::string column(const std::string &property) const {
        if (propertyMap->count(property) > 0) {
            //返回实体类列名
            return propertyMap->at(property).getColumn();
        } else {
            //属性不存在,跑异常
            throw MapperException("[property]" + property + "is not exist!");
        }
    }

    /**
     * 判断属性值是否存在等
     * @param property
     * @return
     */
    const std::string &property(const std::string &property) const {
        if (propertyMap->count(property) > 0) {
            return property;
        } else {
            throw MapperException("[property]" + property + "is not exist!");
        }
    }

public:

    ////////////////////// and ///////////////////////////////
    Criteria *andIsNull(const std::string &property) {
        criteria.emplace_back(Criterion(column(property) + " IS NULL"));
        return this;
    }

    Criteria *andIsNotNull(const std::string &property) {
        criteria.emplace_back(Criterion(column(property) + " IS NOT NULL"));
        return this;
    }

    template<typename Object>
    Criteria *andEqualTo(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " =", value));
        return this;
    }

    template<typename Object>
    Criteria *andNotEqualTo(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " <>", value));
        return this;
    }

    template<typename Object>
    Criteria *andGreaterThan(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " >", value));
        return this;
    }

    template<typename Object>
    Criteria *andGreaterThanOrEqualTo(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " >=", value));
        return this;
    }

    template<typename Object>
    Criteria *andLessThan(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " <", value));
        return this;
    }

    template<typename Object>
    Criteria *andLessThanOrEqualTo(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " <=", value));
        return this;
    }

    template<typename Iterable>
    Criteria *andIn(const std::string &property, const Iterable &values) {
        criteria.emplace_back(Criterion(column(property) + " IN", values));
        return this;
    }

    template<typename Iterable>
    Criteria *andNotIn(const std::string &property, const Iterable &values) {
        criteria.emplace_back(Criterion(column(property) + " NOT IN", values));
        return this;
    }

    template<typename Object>
    Criteria *andBetween(const std::string &property, const Object &value1, const Object &value2) {
        criteria.emplace_back(Criterion(column(property) + " BETWEEN", value1, value2));
        return this;
    }

    template<typename Object>
    Criteria *andNotBetween(const std::string &property, const Object &value1, const Object &value2) {
        criteria.emplace_back(Criterion(column(property) + " NOT BETWEEN", value1, value2));
        return this;
    }

    Criteria *andLike(const std::string &property, std::string &value) {
        criteria.emplace_back(Criterion(column(property) + " LIKE", value));
        return this;
    }

    Criteria *andNotLike(const std::string &property, std::string &value) {
        criteria.emplace_back(Criterion(column(property) + " NOT LIKE", value));
        return this;
    }

    /**
     * 手写条件
     *
     * @param condition 例如 "length(countryname)<5"
     * @return
     */
    Criteria *andCondition(const std::string &condition) {
        criteria.emplace_back(Criterion(condition));
        return this;
    }

    /**
     * 手写左边条件，右边用value值
     *
     * @param condition 例如 "length(countryname)="
     * @param value     例如 5
     * @return
     */
    template<typename Object>
    Criteria *andCondition(const std::string &condition, const Object &value) {
        criteria.emplace_back(Criterion(condition, value));
        return this;
    }

    ////////////////////// or ///////////////////////////////
    Criteria *orIsNull(const std::string &property) {
        criteria.emplace_back(Criterion(column(property) + " IS NULL", true));
        return this;
    }

    Criteria *orIsNotNull(const std::string &property) {
        criteria.emplace_back(Criterion(column(property) + " IS NOT NULL", true));
        return this;
    }

    template<typename Object>
    Criteria *orEqualTo(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " =", value, true));
        return this;
    }

    template<typename Object>
    Criteria *orNotEqualTo(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " <>", value, true));
        return this;
    }

    template<typename Object>
    Criteria *orGreaterThan(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " >", value, true));
        return this;
    }

    template<typename Object>
    Criteria *orGreaterThanOrEqualTo(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " >=", value, true));
        return this;
    }

    template<typename Object>
    Criteria *orLessThan(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " <", value, true));
        return this;
    }

    template<typename Object>
    Criteria *orLessThanOrEqualTo(const std::string &property, const Object &value) {
        criteria.emplace_back(Criterion(column(property) + " <=", value, true));
        return this;
    }

    template<typename Iterable>
    Criteria *orIn(const std::string &property, const Iterable &values) {
        criteria.emplace_back(Criterion(column(property) + " IN", values, true));
        return this;
    }

    template<typename Iterable>
    Criteria *orNotIn(const std::string &property, const Iterable &values) {
        criteria.emplace_back(Criterion(column(property) + " NOT IN", values, true));
        return this;
    }

    template<typename Object>
    Criteria *orBetween(const std::string &property, const Object &value1, const Object &value2) {
        criteria.emplace_back(Criterion(column(property) + " BETWEEN", value1, value2, true));
        return this;
    }

    template<typename Object>
    Criteria *orNotBetween(const std::string &property, const Object &value1, const Object &value2) {
        criteria.emplace_back(Criterion(column(property) + " NOT BETWEEN", value1, value2, true));
        return this;
    }

    Criteria *orLike(const std::string &property, std::string &value) {
        criteria.emplace_back(Criterion(column(property) + " LIKE", value, true));
        return this;
    }

    Criteria *orNotLike(const std::string &property, std::string &value) {
        criteria.emplace_back(Criterion(column(property) + " NOT LIKE", value, true));
        return this;
    }

    /**
     * 手写条件
     *
     * @param condition 例如 "length(countryname)<5"
     * @return
     */
    Criteria *orCondition(const std::string &condition) {
        criteria.emplace_back(Criterion(condition, true));
        return this;
    }

    /**
     * 手写左边条件，右边用value值
     *
     * @param condition 例如 "length(countryname)="
     * @param value     例如 5
     * @return
     */
    template<typename Object>
    Criteria *orCondition(const std::string &condition, const Object &value) {
        criteria.emplace_back(Criterion(condition, value, true));
        return this;
    }


};

#endif //MAPPER_CRITERIA_HPP
