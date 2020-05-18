//
// Created by Jsiyong on 2020-05-17.
//

#ifndef MAPPER_ITERABLE_HPP
#define MAPPER_ITERABLE_HPP


#include "Object.hpp"

//包装了集合类
class Iterable : public Object {
private:

    /**
    * 将容器的值保存到std::vector<Object>中存储
    * @tparam Collection
    * @param collection
    * @return
    */
    template<typename Collection>
    void save2Collection(const Collection &collection) {
        for (const auto &c:collection) {
            //加入values集合中,注意,不能push_back,避免对象发生赋值
            this->buff.values.emplace_back(c);
        }
    }

public:
    template<typename T>
    Iterable(const std::set<T> &value) : Object(typeid(std::set<T>), true) {
        save2Collection(value);
    };

    template<typename T>
    Iterable(const std::list<T> &value) : Object(typeid(std::list<T>), true) {
        save2Collection(value);
    };

    template<typename T>
    Iterable(const std::vector<T> &value) : Object(typeid(std::list<T>), true) {
        save2Collection(value);
    };

    Iterable() = default;
};

#endif //MAPPER_ITERABLE_HPP
