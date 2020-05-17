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

private:

    /**
     * 将容器的值转为std::vector<Any>类型存储
     * @tparam Collection
     * @param collection
     * @return
     */
    template<typename Collection>
    static std::vector<Any> getValuesFromCollection(const Collection &collection) {
        std::vector<Any> results;
//        results.insert(results.end(), collection.begin(), collection.end());
        for (const auto &c:collection) {
            results.emplace_back(c);
        }
        return results;
    }

public:

    /**
     * 判断是不是集合类型
     * @tparam T
     * @param t
     * @return
     */
    template<typename T>
    static CollectionInfo getCollectionInfo(const std::vector<T> &value) {
        return CollectionInfo(CollectionInfo::CollectionType::Vector, getValuesFromCollection(value));
    }

    template<typename T>
    static CollectionInfo getCollectionInfo(const std::set<T> &value) {
        return CollectionInfo(CollectionInfo::CollectionType::Set, getValuesFromCollection(value));
    }

    template<typename T>
    static CollectionInfo getCollectionInfo(const std::list<T> &value) {
        return CollectionInfo(CollectionInfo::CollectionType::List, getValuesFromCollection(value));
    }

    template<typename T>
    static CollectionInfo getCollectionInfo(const T &t) {
        return {};
    }

};

#endif //MAPPER_TYPEUTILS_HPP
