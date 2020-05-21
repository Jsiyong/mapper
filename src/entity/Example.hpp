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
#include "JoinEntityTable.hpp"
#include "OrderBy.hpp"

/**
 * 通用的Example查询对象
 */
template<typename Entity>
class Example {
private:
    std::vector<std::shared_ptr<Criteria>> oredCriteria;//标准列表
    std::map<std::string, EntityColumn> propertyMap; //属性和列对应,是所有的,包括连接出来的
    std::shared_ptr<Entity> entityClass = nullptr;//实体类
    EntityTable table;//该实体类对应的表
    std::map<std::string, JoinEntityTable> joinTableMap;//该实体类关联的表集合,key为表的别名
    std::map<std::string, EntityColumn> entityPropertyMap; //属性和列对应,是当前实体类的,不包括连接出来的

    std::shared_ptr<EntityColumn> keyEntityColumn = nullptr;//主键列
    std::vector<EntityColumn> joinEntityColumns;

    std::shared_ptr<OrderBy> orderBy = nullptr;//排序的

private:
    std::shared_ptr<Criteria> createCriteriaInternal() {
        //传递属性列映射的指针进去
        return std::make_shared<Criteria>(&propertyMap, &table);
    }

    /**
     * 合并OredCriteria
     * @param sqlBuilder
     */
    void buildOredCriteria(const std::shared_ptr<SQLBuilder> &sqlBuilder) const {
        for (auto &criteria : oredCriteria) {
            if (criteria->getAndOr() == SQLConstants::OR) {
                sqlBuilder->OR();
            }
            sqlBuilder->WHERE(ExampleHelper::getConditionFromCriteria(*criteria));
        }
    }

    /**
     * 生成orderBy
     * @param sqlBuilder
     */
    void buildOrderBy(const std::shared_ptr<SQLBuilder> &sqlBuilder) const {
        //处理排序
        if (orderBy) {
            for (auto &o: orderBy->getOrderBys()) {
                sqlBuilder->ORDER_BY(o);
            }
        }
    }

    //内部获取select from where后的语句
    void buildFromWhereStatementByExample(const std::shared_ptr<SQLBuilder> &sqlBuilder) const {
        //表也要用别名
        sqlBuilder->FROM(table.getTableName() + " " + SQLConstants::AS + " " + table.getAlias());
        //处理外连接
        for (const auto &jt:joinTableMap) {
            sqlBuilder->LEFT_OUTER_JOIN(
                    jt.second.getTableName() + " " + SQLConstants::AS + " " + jt.second.getAlias()
                    + " " + SQLConstants::ON + " " + jt.second.getJoinColumn() + " " + SQLConstants::EQUALl_TO + " " +
                    jt.second.getJoinedColumn()
            );
        }
        //处理条件
        buildOredCriteria(sqlBuilder);
    }

public:

    /**
     * 获取更新的语句
     * @return
     */
    std::string getUpdateStatementByExample() const {
        std::shared_ptr<SQLBuilder> sqlBuilder = std::make_shared<SQLBuilder>();//SQL语句构建器
        //首先拼接update语句,注意要加上别名
        sqlBuilder->UPDATE(table.getTableName() + " " + SQLConstants::AS + " " + table.getAlias());
        for (auto &entityProperty:entityPropertyMap) {
            //不是一对多的关系才可以设置
            if (entityProperty.second.getJoinType() != JoinType::OneToMany) {
                sqlBuilder->SET(entityProperty.second.getColumnWithTableAlias() + " " + SQLConstants::EQUALl_TO + " " +
                                SQLConstants::PLACEHOLDER);
            }
        }
        buildOredCriteria(sqlBuilder);
        return sqlBuilder->toString();
    }

    /**
    * 通过Example获取查询多少数量的语句
    * @return
    */
    std::string getSelectCountStatementByExample() const {
        std::shared_ptr<SQLBuilder> sqlBuilder = std::make_shared<SQLBuilder>();//SQL语句构建器
        sqlBuilder->SELECT(SQLConstants::COUNT);
        buildFromWhereStatementByExample(sqlBuilder);
        return sqlBuilder->toString();
    }

    /**
     * 通过Example获取查询的语句
     * @return
     */
    std::string getSelectStatementByExample() const {
        std::shared_ptr<SQLBuilder> sqlBuilder = std::make_shared<SQLBuilder>();//SQL语句构建器
        for (const auto &p:propertyMap) {
            //用别名
            if (p.second.getJoinType() != JoinType::OneToMany) {
                sqlBuilder->SELECT(
                        p.second.getColumnWithTableAlias() + " " + SQLConstants::AS + " " + p.second.getAlias());
            }
        }
        buildFromWhereStatementByExample(sqlBuilder);
        buildOrderBy(sqlBuilder);//加上排序信息
        return sqlBuilder->toString();
    }

    /**
     * 获取该Example传入的所有预处理的参数值
     * @return
     */
    std::vector<Object> getPrepareValues() const {
        return ExampleHelper::getValuesFromOredCriteria(this->oredCriteria);
    }

    std::map<std::string, EntityColumn> getColumnAliasMap() const {
        std::map<std::string, EntityColumn> res;
        for (auto &pm :this->propertyMap) {
            res.insert(std::make_pair(pm.second.getAlias(), pm.second));
        }
        return res;
    }

    const std::shared_ptr<EntityColumn> &getKeyEntityColumn() const {
        return keyEntityColumn;
    }

    const std::vector<EntityColumn> &getJoinEntityColumns() const {
        return joinEntityColumns;
    }

public:
    const std::shared_ptr<Entity> &getEntity() const {
        return entityClass;
    }

    /**
     * 初始化获取数据
     */
    Example() {
        std::shared_ptr<EntityTableMap> resultMap = std::make_shared<EntityTableMap>();
        this->entityClass = EntityHelper::getResultMap<Entity>(resultMap);
        //拆分表信息
        for (auto &t: resultMap->getEntityTables()) {
            //若当前类的别名和t的别名一致,说明是当前类,否则是关联的类
            if (AliasHelper::getAliasFromType<Entity>() == t.getAlias()) {
                this->table = t;
            } else {
                this->joinTableMap.insert(std::make_pair(t.getAlias(), t));
            }
        }
        //拆分实体列的信息
        for (auto &r :resultMap->getPropertyMap()) {
            this->propertyMap.insert(r);
            //找出主键列
            if (r.second.getColumnType() == ColumnType::Id) {
                this->keyEntityColumn = std::make_shared<EntityColumn>();
                *this->keyEntityColumn = r.second;
            }
            //找出连接的列
            if (r.second.getJoinType() != JoinType::Null) {
                this->joinEntityColumns.emplace_back(r.second);
            }
            /////////////////////////////////////////////////////////
            //找出连接属性是Join的,设置其连接信息
            if (r.second.getJoinType() != JoinType::Null) {
                //设置连表的连接属性
                auto &joinTable = this->joinTableMap.at(r.second.getJoinTableAlias());
                //设置被连接的表的属性,加上别名
                joinTable.setJoinColumn(r.second.getColumnWithTableAlias());
                joinTable.setJoinProperty(r.second.getProperty());
                joinTable.setJoinAlias(r.second.getTableAlias());
                joinTable.setJoinedProperty(r.second.getJoinProperty());
            }
            //被连接表的信息处理
            if (AliasHelper::getAliasFromType<Entity>() == r.second.getTableAlias()) {
                this->entityPropertyMap.insert(r);
            }
        }
        //设置被连接表
        for (auto &joinMap:joinTableMap) {
            auto joinColumn = this->propertyMap.at(joinMap.second.getJoinedProperty()).getColumnWithTableAlias();
            joinMap.second.setJoinedColumn(joinColumn);
        }
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

    //降序
    template<typename R, typename T>
    std::shared_ptr<OrderBy> orderByDesc(R T::* t) {
        if (orderBy == nullptr) {
            orderBy = std::make_shared<OrderBy>(&propertyMap);
        }
        return orderBy->desc(t);
    }

    //升序
    template<typename R, typename T>
    std::shared_ptr<OrderBy> orderByAsc(R T::* t) {
        if (orderBy == nullptr) {
            orderBy = std::make_shared<OrderBy>(&propertyMap);
        }
        return orderBy->asc(t);
    }
};

#endif //MAPPER_EXAMPLE_HPP
