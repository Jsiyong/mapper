//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_ENTITYTABLEMAP_HPP
#define MAPPER_ENTITYTABLEMAP_HPP

#include <vector>
#include "EntityTable.hpp"
#include "EntityColumn.hpp"

/**
 * 实体类和表格映射信息
 */
class EntityTableMap {
private:
    EntityTable entityTable;//实体类与表格信息
    std::vector<EntityColumn> propertyColumns;//属性与字段信息
public:

    EntityTableMap(const EntityTable &entityTable, const std::vector<EntityColumn> &propertyColumns) {
        this->entityTable = entityTable;
        this->propertyColumns = propertyColumns;
    }
};

#endif //MAPPER_ENTITYTABLEMAP_HPP
