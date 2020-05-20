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


public:
    template<class T>
    struct getType {
        typedef T type;
    };

    //指针类型的偏特化,指针类型是其原类型
    template<class T>
    struct getType<T *> {
        typedef T type;
    };

    /**
     * 判断是不是集合类型
     * @tparam T
     * @param t
     * @return
     */
    template<typename T>
    static bool isContainer(const std::vector<T> &value) {
        return true;
    }

    template<typename T>
    static bool isContainer(const std::set<T> &value) {
        return true;
    }

    template<typename T>
    static bool isContainer(const std::list<T> &value) {
        return true;
    }

    template<typename T>
    static bool isContainer(const T &t) {
        return false;
    }

};

#endif //MAPPER_TYPEUTILS_HPP
