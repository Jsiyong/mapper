#include <utility>

#include <utility>

//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_ENTITYTABLE_HPP
#define MAPPER_ENTITYTABLE_HPP

#include <string>
#include <memory>
#include <util/AliasHelper.hpp>


/**
 * 实体类和表的映射关系
 */
class EntityTable {
private:
    std::string tableName;//表名
    std::string className;//类名
    std::string alias;//数据库表的别名,为系统自动分配,默认生成的别名

public:
    EntityTable() = default;

    template<typename Entity>
    EntityTable(Entity *, std::string className, std::string tableName)
            : tableName(std::move(tableName)),
              className(std::move(className)) {
        alias = AliasHelper::getAliasFromType<Entity>();//别名
    }

public:

    const std::string &getClassName() const {
        return className;
    }

    const std::string &getTableName() const {
        return tableName;
    }

    const std::string &getAlias() const {
        return alias;
    }


};

#endif //MAPPER_ENTITYTABLE_HPP
