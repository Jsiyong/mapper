//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_ENTITYTABLEMAP_HPP
#define MAPPER_ENTITYTABLEMAP_HPP

#include <vector>
#include <set>
#include "EntityTable.hpp"
#include "EntityColumn.hpp"

/**
 * 实体类和表格映射信息
 */
class EntityTableMap {
private:
    EntityTable entityTable;//实体类与表格信息
    std::map<std::string, EntityColumn> propertyMap;//属性和列对应
    std::set<std::string> keyProperties; //实体类主键属性
    std::set<std::string> keyColumns;//实体类主键列

public:

    const EntityTable &getEntityTable() const {
        return entityTable;
    }

    void setEntityTable(const EntityTable &entityTable) {
        EntityTableMap::entityTable = entityTable;
    }

    std::map<std::string, EntityColumn> &getPropertyMap() {
        return propertyMap;
    }

    void setPropertyMap(const std::map<std::string, EntityColumn> &propertyMap) {
        EntityTableMap::propertyMap = propertyMap;
    }

    const std::set<std::string> &getKeyProperties() const {
        return keyProperties;
    }

    void setKeyProperties(const std::set<std::string> &keyProperties) {
        EntityTableMap::keyProperties = keyProperties;
    }

    const std::set<std::string> &getKeyColumns() const {
        return keyColumns;
    }

    void setKeyColumns(const std::set<std::string> &keyColumns) {
        EntityTableMap::keyColumns = keyColumns;
    }

};

#endif //MAPPER_ENTITYTABLEMAP_HPP
