//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_SINGLETON_HPP
#define MAPPER_SINGLETON_HPP

/**
 * 单例模式的封装类
 * @tparam T
 */
template<typename T>
class Singleton {
public:
    static T *getInstance() {
        static T t;
        return &t;
    }
};

#endif //MAPPER_SINGLETON_HPP
