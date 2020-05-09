//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_ENTITYWRAPPER_HPP
#define MAPPER_ENTITYWRAPPER_HPP

#include <memory>
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
    std::shared_ptr<EntityTableMap> getReflectionInfo(std::shared_ptr<Entity> entity) {
        return nullptr;
    }
};

#endif //MAPPER_ENTITYWRAPPER_HPP
