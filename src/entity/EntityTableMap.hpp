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
    //属性与字段信息
    EntityTableMap(const EntityTable &entityTable, const std::vector<EntityColumn> &propertyColumns) {
        this->entityTable = entityTable;
//        this->propertyMap = propertyColumns;
    }

    const EntityTable &getEntityTable() const {
        return entityTable;
    }

    const std::map<std::string, EntityColumn> &getPropertyMap() const {
        return propertyMap;
    }

};

#endif //MAPPER_ENTITYTABLEMAP_HPP
