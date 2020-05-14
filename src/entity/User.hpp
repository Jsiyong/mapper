//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_USER_HPP
#define MAPPER_USER_HPP

#include <string>
#include <memory>
#include <tuple>
#include "EntityWrapper.hpp"
#include "EntityColumn.hpp"
#include "EntityTableMap.hpp"

struct User {
    int id = 0;
    std::string name = "";
};

template<>
class EntityWrapper<User> {
public:

    /**
     * 获取实体类的反射信息
     * @param entity
     */
    auto getReflectionInfo(std::shared_ptr<User> entity) {
        return std::make_tuple(
                EntityTable("User", "user"),
                std::make_pair(&User::id, EntityColumn(&entity->id, "id", "id")),
                std::make_pair(&User::name, EntityColumn(&entity->name, "name", "name"))
        );
    }
};

#endif //MAPPER_USER_HPP
