//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_ENTITYCOLUMN_HPP
#define MAPPER_ENTITYCOLUMN_HPP

#include <string>
#include <typeindex>
#include <util/EntityHelper.hpp>
#include <util/AliasHelper.hpp>
#include "EntityEnum.hpp"

/**
 * 属性与数据库列的对应关系
 */
class EntityColumn {
private:
    void *pProperty = nullptr;//实体类字段地址
    std::type_index typeIndex = std::type_index(typeid(void));//实体类字段类型信息
    std::string column = "";//数据库列名
    std::string property = "";//实体类属性名,包含了别名
    std::string alias;//数据库列的别名,列的别名为表的别名+"_"+列名,也是默认生成的
    ColumnType columnType = ColumnType::Null;//数据库列的类型
    KeySql keySql = KeySql::Null;//主键生成策略
    JoinType joinType = JoinType::Null;//连接其他表的类型,暂时只有LeftJoin
    std::string joinProperty;//连接的外表的属性列
    std::string tableAlias;//所属table的别名,方便通过这个别名,区分字段所属的不同的table,因为还有连表查询
    std::string joinTableAlias;//连接的表的别名,方便通过该别名,找到该字段连接的表的信息

private:
    /**
     * 获取外表连接的属性
     * @tparam T
     * @return
     */
    template<typename T>
    void getJoinPropertyAndTableAlias(const T &, std::string &, std::string &) {}

    template<typename JEntity, typename JRet>
    void
    getJoinPropertyAndTableAlias(JRet JEntity::* joinColunm, std::string &joinProperty, std::string &joinTableAlias) {
        //通过属性偏移地址获取连接的属性名
        joinProperty = EntityHelper::getProperty(joinColunm);
        joinTableAlias = AliasHelper::getAliasFromType<JEntity>();
    }

public:
    EntityColumn() = default;

    template<typename Entity, typename T>
    EntityColumn(std::shared_ptr<Entity>, T *pProperty, const std::string &property, const std::string &column,
                 ColumnType columnType, KeySql keySql, JoinType joinType):typeIndex(typeid(T)) {
        this->tableAlias = AliasHelper::getAliasFromType<Entity>();//先拿出实体类的别名,属性要加上类的别名,避免连表混乱
        this->pProperty = pProperty;
        this->property = tableAlias + "." + property;
        this->column = column;
        this->columnType = columnType;
        this->keySql = keySql;
        this->joinType = joinType;
        this->alias = tableAlias + "_" + this->column;//同时获取别名
    }

    template<typename Entity, typename T, typename J>
    EntityColumn(std::shared_ptr<Entity> entity, T *pProperty, const std::string &property, const std::string &column,
                 ColumnType columnType, KeySql keySql, JoinType joinType, const J &joinColunm) :typeIndex(typeid(T)) {
        new(this)EntityColumn(entity, pProperty, property, column, columnType, keySql, joinType);
        //获取连接属性名和表别
        this->getJoinPropertyAndTableAlias(joinColunm, this->joinProperty, this->joinTableAlias);
    }

    const std::string &getColumn() const {
        return column;
    }

    const std::string &getProperty() const {
        return property;
    }

    const std::string &getAlias() const {
        return alias;
    }

    const std::string &getTableAlias() const {
        return tableAlias;
    }

    /**
     * 获取连同表别名在内的数据库列:格式 表别名.数据库列名
     * @return
     */
    std::string getColumnWithTableAlias() const {
        return tableAlias + "." + column;
    }

    JoinType getJoinType() const {
        return joinType;
    }

    const std::string &getJoinProperty() const {
        return joinProperty;
    }

    const std::string &getJoinTableAlias() const {
        return joinTableAlias;
    }

    /**
     * 获取属性的地址值
     * @return
     */
    void *getPropertyPtr() const {
        return pProperty;
    }

    const std::type_index &getTypeIndex() const {
        return typeIndex;
    }
};

#endif //MAPPER_ENTITYCOLUMN_HPP
