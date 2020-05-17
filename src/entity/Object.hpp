//
// Created by Jsiyong on 2020-05-17.
//

#ifndef MAPPER_OBJECT_HPP
#define MAPPER_OBJECT_HPP

#include <typeindex>
#include <string>
#include <set>
#include <vector>
#include <list>

/**
 * 简单包装一下any类型,方便之后取出来使用
 */
class Object {
protected:
    void *value = nullptr;//存放的是值的地址
    std::type_index typeIndex = std::type_index(typeid(void));//存放的是值的类型
    bool container = false;//是否是一个容器值
    std::vector<Object> values;

protected:
    //专门供给子类调用的构造函数
    template<typename Value>
    Object(const Value &value, std::type_index typeIndex, bool container):
            value(value), typeIndex(typeIndex), container(container) {}

public:
    //将可以转为std::string类型,都归入std::string类型
    Object(const std::string &value) : value((void *) &value), typeIndex(typeid(std::string)) {};

    //将可以转为const char*类型,也都归入std::string类型
    Object(const char *value) : value((void *) &value), typeIndex(typeid(std::string)) {};

    //将可以转为int类型,都归入int类型
    Object(int value) : value((void *) &value), typeIndex(typeid(int)) {};

    Object() = default;

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

    const std::type_index &getTypeIndex() const {
        return typeIndex;
    }

    /**
     * 是否是一个容器
     * @return
     */
    bool isContainer() const {
        return container;
    }

    /**
     * 若是容器,获取容器的每一个值,转为Object类型
     * @return
     */
    const std::vector<Object> &getValues() const {
        return values;
    }
};

#endif //MAPPER_OBJECT_HPP
