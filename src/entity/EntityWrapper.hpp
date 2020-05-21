//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_ENTITYWRAPPER_HPP
#define MAPPER_ENTITYWRAPPER_HPP

#include <memory>
#include <type_traits>
#include "EntityTableMap.hpp"

/**
 * 实体类的外包增强类
 */
template<typename Entity>
class EntityWrapper {
private:
    /**
     * 如果是列表类型,取出其中的元素出来继续获取
     * @tparam T
     * @param entities
     * @return
     */
//    template<typename T>
//    auto getReflectionInfoInternal(std::vector<T> *entities) {
//        std::shared_ptr<T> entity = std::make_shared<T>();
//        auto reflectionInfo = EntityWrapper<T>().getReflectionInfo(entity.get());
////        return getReflectionInfo(entity.get());
//        return reflectionInfo;
//    }

    /**
     * 如果是其他类型,返回空
     * @tparam T
     * @param entities
     * @return
     */
    template<typename T>
    void *getReflectionInfoInternal(T entities) {
        return nullptr;
    }

public:
    /**
     * 获取实体类的反射信息
     * @param entity
     */
    void *getReflectionInfo(Entity *entity) {
        return nullptr;
    }
};

#endif //MAPPER_ENTITYWRAPPER_HPP
