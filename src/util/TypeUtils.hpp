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
    static CollectionInfo getCollectionInfo(const std::vector<T> &t) {
        return CollectionInfo(CollectionInfo::CollectionType::Vector, t.size());
    }

    template<typename T>
    static CollectionInfo getCollectionInfo(const std::set<T> &t) {
        return CollectionInfo(CollectionInfo::CollectionType::Set, t.size());
    }

    template<typename T>
    static CollectionInfo getCollectionInfo(const std::list<T> &t) {
        return CollectionInfo(CollectionInfo::CollectionType::List, t.size());
    }

    template<typename T>
    static CollectionInfo getCollectionInfo(const T &t) {
        return {};
    }

};

#endif //MAPPER_TYPEUTILS_HPP
