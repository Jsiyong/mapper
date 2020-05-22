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
    std::shared_ptr<Entity> entityClass = std::make_shared<Entity>();//实体类
    EntityTable table;//该实体类对应的表
    std::map<std::string, JoinEntityTable> joinTableMap;//该实体类关联的表集合,key为表的别名
    std::map<std::string, EntityColumn> entityPropertyMap; //属性和列对应,是当前实体类的,不包括连接出来的

    std::shared_ptr<EntityColumn> keyEntityColumn = nullptr;//主键列
    std::vector<EntityColumn> joinEntityColumns;

    std::shared_ptr<OrderBy> orderBy = nullptr;//排序的
    //第一个参数指定第一个返回记录行的偏移量,第二个参数指定返回记录行的最大数目,若第二个参数为0,表示不合法
    std::pair<int, int> limitPair = {0, INT32_MIN};//limit first,second

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

    /**
     * 内部构建limit语句
     * @param sqlBuilder
     */
    void buildLimit(const std::shared_ptr<SQLBuilder> &sqlBuilder) const {
        sqlBuilder->LIMIT(SQLConstants::PLACEHOLDER, SQLConstants::PLACEHOLDER);
    }

public:

    /**
     * 获取插入的上下文,包括插入语句和插入的值,插入的话可支持批量插入
     * @param record
     * @return
     */
    std::pair<std::string, std::vector<Object>> getInsertContext(const Entity &record, bool selective) {
        std::shared_ptr<SQLBuilder> sqlBuilder = std::make_shared<SQLBuilder>();//SQL语句构建器
        sqlBuilder->INSERT_INTO(table.getTableName());//插入语句不能有别名
        //获取该对象的信息
        std::shared_ptr<EntityTableMap> resultMap = std::make_shared<EntityTableMap>();
        std::shared_ptr<Criteria> insertCriteria = std::make_shared<Criteria>(
                const_cast<decltype(propertyMap) *>(&propertyMap),
                const_cast<decltype(table) *>(&table));
        //需要转为Object,不然会变成空类型
        EntityHelper::getResultMap(const_cast<Entity *>(&record), resultMap);
        for (auto &entityProperty:resultMap->getPropertyMap()) {
            //不是一对多的关系才可以设置
            if (entityProperty.second.getJoinType() != JoinType::OneToMany
                && entityProperty.second.getTableAlias() == this->table.getAlias()//不是自己的表也不算
                && entityProperty.second.getColumnType() != ColumnType::Id) {//ID列也不能更改
                if (selective && entityProperty.second.isNull()) {
                    continue;
                }
                sqlBuilder->VALUES(entityProperty.second.getColumn(), SQLConstants::PLACEHOLDER);
                insertCriteria->andEqualTo(entityProperty.second.getProperty(),
                                           entityProperty.second.getEntityFieldValue());
            }
        }
        return std::make_pair(sqlBuilder->toString(), ExampleHelper::getValuesFromOredCriteria({insertCriteria}));
    }

    /**
     * 获取删除语句的上下文
     * @return
     */
    std::pair<std::string, std::vector<Object>> getDeleteContextByExample() const {
        std::shared_ptr<SQLBuilder> sqlBuilder = std::make_shared<SQLBuilder>();//SQL语句构建器
        sqlBuilder->DELETE_FROM(table.getTableName() + " " + SQLConstants::AS + " " + table.getAlias());
        buildOredCriteria(sqlBuilder);
        return std::make_pair(sqlBuilder->toString(), ExampleHelper::getValuesFromOredCriteria(this->oredCriteria));
    }

    /**
     * 获取更新的上下文,包括更新语句和更新的值
     * @selective 若是true,只更新不是null的属性值
     * @return
     */
    std::pair<std::string, std::vector<Object>> getUpdateContextByExample(const Entity &param, bool selective) const {
        std::shared_ptr<SQLBuilder> sqlBuilder = std::make_shared<SQLBuilder>();//SQL语句构建器
        //首先拼接update语句,注意要加上别名
        sqlBuilder->UPDATE(table.getTableName() + " " + SQLConstants::AS + " " + table.getAlias());
        //专门是update语句的Criteria
        //获取该对象的信息
        std::shared_ptr<EntityTableMap> resultMap = std::make_shared<EntityTableMap>();
        //需要转为Object,不然会变成空类型
        EntityHelper::getResultMap(const_cast<Entity *>(&param), resultMap);
        std::shared_ptr<Criteria> updateCriteria = std::make_shared<Criteria>(
                const_cast<decltype(propertyMap) *>(&propertyMap),
                const_cast<decltype(table) *>(&table));

        for (auto &entityProperty:resultMap->getPropertyMap()) {
            //不是一对多的关系才可以设置
            if (entityProperty.second.getJoinType() != JoinType::OneToMany
                && entityProperty.second.getTableAlias() == this->table.getAlias()//不是自己的表也不算
                && entityProperty.second.getColumnType() != ColumnType::Id) {//ID列也不能更改
                if (selective && entityProperty.second.isNull()) {
                    continue;
                }
                sqlBuilder->SET(entityProperty.second.getColumnWithTableAlias() + " " + SQLConstants::EQUALl_TO + " " +
                                SQLConstants::PLACEHOLDER);
                updateCriteria->andEqualTo(entityProperty.second.getProperty(),
                                           entityProperty.second.getEntityFieldValue());
            }
        }
        auto updateCriteriaValues = ExampleHelper::getValuesFromOredCriteria({updateCriteria});

        buildOredCriteria(sqlBuilder);
        //where添加的标准的值
        auto whereCriteriaValues = ExampleHelper::getValuesFromOredCriteria(this->oredCriteria);
        //合并条件
        updateCriteriaValues.insert(updateCriteriaValues.end(), whereCriteriaValues.begin(), whereCriteriaValues.end());
        return std::make_pair(sqlBuilder->toString(), updateCriteriaValues);
    }

    /**
    * 通过Example获取查询多少数量的上下文
    * @return
    */
    std::pair<std::string, std::vector<Object>> getSelectCountContextByExample() const {
        std::shared_ptr<SQLBuilder> sqlBuilder = std::make_shared<SQLBuilder>();//SQL语句构建器
        sqlBuilder->SELECT(SQLConstants::COUNT);
        buildFromWhereStatementByExample(sqlBuilder);
        return std::make_pair(sqlBuilder->toString(), ExampleHelper::getValuesFromOredCriteria(this->oredCriteria));
    }

    /**
     * 通过Example获取查询的上下文
     * @return
     */
    std::pair<std::string, std::vector<Object>> getSelectContextByExample() const {
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

        auto oredCriteriaValues = ExampleHelper::getValuesFromOredCriteria(this->oredCriteria);
        //添加limit

        //若limit参数有效
        if (limitPair.second != INT32_MIN) {
            buildLimit(sqlBuilder);
            oredCriteriaValues.emplace_back(limitPair.first);
            oredCriteriaValues.emplace_back(limitPair.second);
        }
        return std::make_pair(sqlBuilder->toString(), oredCriteriaValues);
    }

    std::map<std::string, EntityColumn> getColumnAliasMap() const {
        std::map<std::string, EntityColumn> res;
        for (auto &pm :this->propertyMap) {
            res.insert(std::make_pair(pm.second.getAlias(), pm.second));
        }
        return res;
    }

    /**
     * 若entity为空,则返回本类中的entityColumn,否则返回该对象自己的
     * @param entity
     * @return
     */
    std::shared_ptr<EntityColumn> getKeyEntityColumn(const Entity *entity = nullptr) const {
        if (nullptr == entity) {
            return keyEntityColumn;
        }
        std::shared_ptr<EntityTableMap> resultMap = std::make_shared<EntityTableMap>();
        std::shared_ptr<EntityColumn> primaryEntityColumn = std::make_shared<EntityColumn>();
        //需要转为Object,不然会变成空类型
        EntityHelper::getResultMap(const_cast<Entity *>(entity), resultMap);
        for (auto &entityProperty:resultMap->getPropertyMap()) {
            if (entityProperty.second.getTableAlias() == this->table.getAlias()//不是自己的表不算
                && entityProperty.second.getColumnType() == ColumnType::Id) {//ID列
                *primaryEntityColumn = entityProperty.second;
                return primaryEntityColumn;
            }
        }
        return nullptr;
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
        EntityHelper::getResultMap(this->entityClass.get(), resultMap);
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
            if (r.second.getColumnType() == ColumnType::Id && r.second.getTableAlias() == this->table.getAlias()) {
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

    /**
     * 分页
     * @param offset 偏移量
     * @param size 查询数量
     */
    void limit(int offset, int size) {
        if (0 < offset && 0 >= size)
            return;
        this->limitPair = std::make_pair(offset, size);
    }

    void limit(int size) {
        limit(0, size);
    }
};

#endif //MAPPER_EXAMPLE_HPP
