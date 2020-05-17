//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_TYPEUTILS_HPP
#define MAPPER_TYPEUTILS_HPP

#include <vector>
#include <set>
#include <list>
#include <entity/CollectionInfo.hpp>

/**
 * 类型工具类
 */
class TypeUtils {
public:

    /**
     * 判断是不是集合类型
     * @tparam T
     * @param t
     * @return
     */
    template<typename T>
    static CollectionInfo getCollectionSize(const std::vector<T> &value) {
        return CollectionInfo(CollectionInfo::CollectionType::Vector, value.size(), (void *) &value);
    }

    template<typename T>
    static CollectionInfo getCollectionInfo(const std::set<T> &value) {
        return CollectionInfo(CollectionInfo::CollectionType::Set, value.size(), (void *) &value);
    }

    template<typename T>
    static CollectionInfo getCollectionInfo(const std::list<T> &value) {
        return CollectionInfo(CollectionInfo::CollectionType::List, value.size(), (void *) &value);
    }

    template<typename T>
    static CollectionInfo getCollectionInfo(const T &t) {
        return {};
    }

};

#endif //MAPPER_TYPEUTILS_HPP
