//
// Created by Jsiyong on 2020-05-17.
//

#ifndef MAPPER_ANY_HPP
#define MAPPER_ANY_HPP

#include <typeindex>
#include <string>
#include <set>
#include <vector>
#include <list>

/**
 * 简单包装一下any类型,方便之后取出来使用
 */
class Any {
private:
    void *value = nullptr;//存放的是值的地址
    std::type_index typeIndex = std::type_index(typeid(void));//存放的是值的类型
public:

    //将可以转为std::string类型,都归入std::string类型
    explicit Any(const std::string &value) : value((void *) &value) {
        this->typeIndex = typeid(std::string);
    };

    //将可以转为std::string类型,都归入int类型
    explicit Any(int value) : value((void *) &value) {
        this->typeIndex = typeid(int);
    };

    template<typename T>
    explicit Any(const std::set<T> &value) : value((void *) &value) {
        this->typeIndex = typeid(std::set<T>);
    };

    template<typename T>
    explicit Any(const std::list<T> &value) : value((void *) &value) {
        this->typeIndex = typeid(std::list<T>);
    };

    template<typename T>
    explicit Any(const std::vector<T> &value) : value((void *) &value) {
        this->typeIndex = typeid(std::vector<T>);
    };


    Any() = default;

    /**
     * 判断是否没有
     * @return
     */
    bool isNull() {
        return nullptr == value;
    }

    void *getValue() const {
        return value;
    }

    void setValue(void *value) {
        this->value = value;
    }

    const std::type_index &getTypeIndex() const {
        return typeIndex;
    }

    void setTypeIndex(const std::type_index &typeIndex) {
        this->typeIndex = typeIndex;
    }

};

#endif //MAPPER_ANY_HPP
