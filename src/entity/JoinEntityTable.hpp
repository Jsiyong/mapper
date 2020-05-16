//
// Created by Jsiyong on 2020-05-16.
//

#ifndef MAPPER_JOINENTITYTABLE_HPP
#define MAPPER_JOINENTITYTABLE_HPP

#include "EntityTable.hpp"

/**
 * 连表的实体类
 */
class JoinEntityTable : public EntityTable {
private:
    std::string joinedColumn;//被连接的列
    std::string joinedProperty;//被连接的属性

    std::string joinColumn;//主表连接的列
    std::string joinProperty;//主表连接的属性
    std::string joinAlias;//主表的别名

public:

    explicit JoinEntityTable(const EntityTable &entityTable) : EntityTable(entityTable) {}

    const std::string &getJoinedColumn() const {
        return joinedColumn;
    }

    void setJoinedColumn(const std::string &joinedColumn) {
        JoinEntityTable::joinedColumn = joinedColumn;
    }

    const std::string &getJoinedProperty() const {
        return joinedProperty;
    }

    void setJoinedProperty(const std::string &joinedProperty) {
        JoinEntityTable::joinedProperty = joinedProperty;
    }

    const std::string &getJoinColumn() const {
        return joinColumn;
    }

    void setJoinColumn(const std::string &joinColumn) {
        JoinEntityTable::joinColumn = joinColumn;
    }

    const std::string &getJoinProperty() const {
        return joinProperty;
    }

    void setJoinProperty(const std::string &joinProperty) {
        JoinEntityTable::joinProperty = joinProperty;
    }

    const std::string &getJoinAlias() const {
        return joinAlias;
    }

    void setJoinAlias(const std::string &joinAlias) {
        JoinEntityTable::joinAlias = joinAlias;
    }

};

#endif //MAPPER_JOINENTITYTABLE_HPP
