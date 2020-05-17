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
 * 简单包装一下Object类型,方便之后取出来使用
 */
class Object {
protected:
    struct Buff {
        std::string stringValue;//字符串的缓存区
        int intValue = 0;//整型的缓存区
        std::vector<Object> values;
    } buff;//缓存数据的内容

    void *value = nullptr;
    std::type_index typeIndex = std::type_index(typeid(void));//存放的是值的类型
    bool container = false;//是否是一个容器值

protected:
    //专门供给子类调用的构造函数
    Object(std::type_index typeIndex, bool container)
            : typeIndex(typeIndex), container(container) {}

public:
    //将可以转为std::string类型,都归入std::string类型
    Object(const std::string &value) : typeIndex(typeid(std::string)) {
        buff.stringValue = value;
        this->value = (void *) buff.stringValue.c_str();
    };

    //将可以转为const char*类型,也都归入std::string类型
    Object(const char *value) : typeIndex(typeid(std::string)) {
        buff.stringValue = value;
        this->value = (void *) buff.stringValue.c_str();
    };

    //将可以转为int类型,都归入int类型
    Object(int value) : typeIndex(typeid(int)) {
        buff.intValue = value;
        this->value = (void *) &buff.intValue;
    };

    Object() = default;

    /**
     * 判断是否没有
     * @return
     */
    bool isNull() {
        return nullptr == value;
    }

    /**
     * 整型
     * @tparam T
     * @return
     */
    template<typename T>
    const typename std::enable_if<std::is_same<int, T>::value, int>::type &getValue() const {
        return buff.intValue;
    }

    /**
     * 字符串类型
     * @tparam T
     * @return
     */
    template<typename T>
    const typename std::enable_if<std::is_same<std::string, T>::value, std::string>::type &getValue() const {
        return buff.stringValue;
    }

//    /**
//     * 返回的是buf的地址值
//     * @return
//     */
//    void *getValuePtr() const {
//        return value;
//    }

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
        return buff.values;
    }
};

#endif //MAPPER_OBJECT_HPP
