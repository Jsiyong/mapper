//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_TYPEUTILS_HPP
#define MAPPER_TYPEUTILS_HPP

#include <vector>
#include <set>
#include <list>

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
    static bool isCollection(const std::vector<T> &t, int &size) {
        size = t.size();
        return true;
    }

    template<typename T>
    static bool isCollection(const std::set<T> &t, int &size) {
        size = t.size();
        return true;
    }

    template<typename T>
    static bool isCollection(const std::list<T> &t, int &size) {
        size = t.size();
        return true;
    }

    template<typename T>
    static bool isCollection(const T &t, int &size) {
        return false;
    }

};

#endif //MAPPER_TYPEUTILS_HPP
