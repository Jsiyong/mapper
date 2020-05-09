#include <utility>

#include <utility>

//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_ENTITYTABLE_HPP
#define MAPPER_ENTITYTABLE_HPP

#include <string>

/**
 * 实体类和表的映射关系
 */
class EntityTable {
private:
    std::string tableName;
public:
    EntityTable() = default;

    EntityTable(std::string tableName, std::string className)
            : tableName(std::move(tableName)),
              className(std::move(className)) {}

private:
    std::string className;
public:


    const std::string &getClassName() const {
        return className;
    }

    const std::string &getTableName() const {
        return tableName;
    }


};

#endif //MAPPER_ENTITYTABLE_HPP
