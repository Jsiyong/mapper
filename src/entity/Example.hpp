//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_EXAMPLE_HPP
#define MAPPER_EXAMPLE_HPP

#include <memory>
#include <vector>
#include <builder/SQLBuilder.hpp>
#include <util/ExampleHelper.hpp>
#include <util/EntityHelper.hpp>
#include "Criteria.hpp"
#include "EntityWrapper.hpp"

/**
 * 通用的Example查询对象
 */
template<typename Entity>
class Example {
private:
    std::vector<std::shared_ptr<Criteria>> oredCriteria;//标准列表
    std::map<std::string, EntityColumn> propertyMap; //属性和列对应
    std::shared_ptr<Entity> entityClass = std::make_shared<Entity>();//实体类
    EntityTable table;//该实体类对应的表

private:
    std::shared_ptr<Criteria> createCriteriaInternal() {
        //传递属性列映射的指针进去
        return std::make_shared<Criteria>(&propertyMap);
    }

public:

    std::string getSelectByExample() {
        auto sqlBuilder = std::make_shared<SQLBuilder>();
        for (auto &p:propertyMap) {
            sqlBuilder->SELECT(p.second.getColumn());
        }
        sqlBuilder->FROM(table.getTableName());
        for (auto &i : oredCriteria) {
            auto conditions = ExampleHelper::getConditionFromCriteria(*i);
            if (i->getAndOr() == Constants::OR) {
                sqlBuilder->OR();
            }
            sqlBuilder->WHERE(conditions);
        }
        return sqlBuilder->toString();
    }

public:
    /**
     * 初始化获取数据
     */
    Example() {
        std::shared_ptr<EntityTableMap> resultMap = std::make_shared<EntityTableMap>();
        auto reflectionInfo = EntityWrapper<Entity>().getReflectionInfo(entityClass);
        EntityHelper::getResultMap(reflectionInfo, resultMap);
        this->table = resultMap->getEntityTable();
        this->propertyMap = resultMap->getPropertyMap();
    }

    /**
     * 生成一个标准
     * @return
     */
    std::shared_ptr<Criteria> createCriteria() {
        auto criteria = createCriteriaInternal();
        if (oredCriteria.empty()) {
            criteria->setAndOr(Constants::AND);
            oredCriteria.emplace_back(criteria);
        }
        return criteria;
    }

    /**
     * 添加一个标准，用or连接
     * @param criteria
     */
    void orCriteria(std::shared_ptr<Criteria> criteria) {
        criteria->setAndOr(Constants::OR);
        oredCriteria.emplace_back(criteria);
    }

    /**
    * 添加一个标准，用or连接
    * @param criteria
    */
    std::shared_ptr<Criteria> orCriteria() {
        auto criteria = createCriteriaInternal();
        criteria->setAndOr(Constants::OR);
        oredCriteria.emplace_back(criteria);
        return criteria;
    }

    /**
     * 添加一个标准，用and连接
     * @param criteria
     */
    void andCriteria(std::shared_ptr<Criteria> criteria) {
        criteria->setAndOr(Constants::AND);
        oredCriteria.emplace_back(criteria);
    }

    /**
    * 添加一个标准，用and连接
    * @param criteria
    */
    std::shared_ptr<Criteria> andCriteria() {
        auto criteria = createCriteriaInternal();
        criteria->setAndOr(Constants::AND);
        oredCriteria.emplace_back(criteria);
        return criteria;
    }
};

#endif //MAPPER_EXAMPLE_HPP
