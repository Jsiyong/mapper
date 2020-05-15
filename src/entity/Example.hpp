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
    std::vector<EntityTable> tables;//该实体类对应的表列表

private:
    std::shared_ptr<Criteria> createCriteriaInternal() {
        //传递属性列映射的指针进去
        return std::make_shared<Criteria>(&propertyMap);
    }

public:

    std::string getSelectByExample() {
        auto sqlBuilder = std::make_shared<SQLBuilder>();
        for (const auto &p:propertyMap) {
            sqlBuilder->SELECT(p.second.getColumn());
        }
        for (const auto &t:tables) {
            sqlBuilder->FROM(t.getTableName());
        }
        for (auto &criteria : oredCriteria) {
            if (criteria->getAndOr() == SQLConstants::OR) {
                sqlBuilder->OR();
            }
            sqlBuilder->WHERE(ExampleHelper::getConditionFromCriteria(*criteria));
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
        this->tables = resultMap->getEntityTables();
        this->propertyMap = resultMap->getPropertyMap();
    }

    /**
     * 生成一个标准
     * @return
     */
    std::shared_ptr<Criteria> createCriteria() {
        auto criteria = createCriteriaInternal();
        if (oredCriteria.empty()) {
            criteria->setAndOr(SQLConstants::AND);
            oredCriteria.emplace_back(criteria);
        }
        return criteria;
    }

    /**
     * 添加一个标准，用or连接
     * @param criteria
     */
    void orCriteria(std::shared_ptr<Criteria> criteria) {
        criteria->setAndOr(SQLConstants::OR);
        oredCriteria.emplace_back(criteria);
    }

    /**
    * 添加一个标准，用or连接
    * @param criteria
    */
    std::shared_ptr<Criteria> orCriteria() {
        auto criteria = createCriteriaInternal();
        criteria->setAndOr(SQLConstants::OR);
        oredCriteria.emplace_back(criteria);
        return criteria;
    }

    /**
     * 添加一个标准，用and连接
     * @param criteria
     */
    void andCriteria(std::shared_ptr<Criteria> criteria) {
        criteria->setAndOr(SQLConstants::AND);
        oredCriteria.emplace_back(criteria);
    }

    /**
    * 添加一个标准，用and连接
    * @param criteria
    */
    std::shared_ptr<Criteria> andCriteria() {
        auto criteria = createCriteriaInternal();
        criteria->setAndOr(SQLConstants::AND);
        oredCriteria.emplace_back(criteria);
        return criteria;
    }
};

#endif //MAPPER_EXAMPLE_HPP
