//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_ENTITYWRAPPER_HPP
#define MAPPER_ENTITYWRAPPER_HPP

#include <memory>
#include <type_traits>
#include "EntityColumn.hpp"
#include "EntityTable.hpp"
#include "EntityTableMap.hpp"

/**
 * 实体类的外包增强类
 */
template<typename Entity>
class EntityWrapper {
public:
    /**
     * 获取实体类的反射信息
     * @param entity
     */
    void *getReflectionInfo(Entity *entity) {
        return nullptr;
    }
};

#define PropertyMap(propert, ...)\
std::make_pair(&EntityClass::propert, EntityColumn(entity, &entity->propert, #propert, ##__VA_ARGS__))

#define EntityMap(Entity, ...)\
std::make_pair(Entity{},EntityTable((Entity*)nullptr, #Entity,##__VA_ARGS__))

#define ResultMap(EntityTable, ...)\
template<>\
class EntityWrapper<decltype(EntityTable.first)> {\
    using EntityClass = decltype(EntityTable.first);\
public:\
    auto getReflectionInfo(decltype(EntityTable.first) *entity)\
    ->decltype(std::make_tuple(EntityTable.second,##__VA_ARGS__)){\
        return std::make_tuple(EntityTable.second,##__VA_ARGS__);\
    }\
};

#endif //MAPPER_ENTITYWRAPPER_HPP
