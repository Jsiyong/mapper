//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_USER_HPP
#define MAPPER_USER_HPP

#include <string>
#include <memory>
#include "EntityWrapper.hpp"
#include "PropertyColumn.hpp"
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
    std::shared_ptr<EntityTableMap> getReflectionInfo(std::shared_ptr<User> entity) {
        return std::make_shared<EntityTableMap>(
                EntityTable("User", "user"),
                std::vector<PropertyColumn>({
                                                    PropertyColumn(&entity->id, "id", "id"),
                                                    PropertyColumn(&entity->name, "name", "name")
                                            })
        );

    }
};

#endif //MAPPER_USER_HPP
