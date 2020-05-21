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
#include <cstring>

/**
 * 简单包装一下Object类型,方便之后取出来使用
 */
class Object {
protected:
    struct Buff {
        std::vector<char> stringValue;//字符串的缓存区
        int intValue = 0;//整型的缓存区
        std::vector<Object> values;
    } buff;//缓存数据的内容

    std::type_index typeIndex = std::type_index(typeid(void));//存放的是值的类型
    bool container = false;//是否是一个容器值
    bool null = false;//是不是空的

protected:
    //专门供给子类调用的构造函数
    Object(std::type_index typeIndex, bool container)
            : typeIndex(typeIndex), container(container) {}

public:
    //创建一个相对应类型的Object,构造函数的形式
    Object(const std::type_index &typeIndex) : typeIndex(typeIndex) {}

    //将可以转为std::string类型,都归入std::string类型
    Object(const std::string &value) : typeIndex(typeid(std::string)) {
        buff.stringValue.assign(value.begin(), value.end());
        buff.stringValue.emplace_back('\0');//必须加入结束符,避免之后不必要的错误
    };

    //将可以转为const char*类型,也都归入std::string类型
    Object(const char *value) : typeIndex(typeid(std::string)) {
        buff.stringValue.resize(strlen(value));
        std::memcpy(buff.stringValue.data(), value, strlen(value));
        buff.stringValue.emplace_back('\0');//必须加入结束符,避免之后不必要的错误
    };

    //将可以转为int类型,都归入int类型
    Object(int value) : typeIndex(typeid(int)) { buff.intValue = value; };

    Object() = default;

    /**
     * 判断是否没有,是不是空的
     * @return
     */
    bool isNull() {
        return typeIndex == typeid(void) || null;
    }

    /**
     * 清空该对象
     */
    void clear() {
        this->buff.values.clear();
        this->buff.intValue = 0;
        this->buff.stringValue = {'\0'};
        this->null = true;
        this->typeIndex = typeid(void);
        this->container = false;
    }

    /**
     * 整型
     * @tparam T
     * @return
     */
    template<typename T>
    typename std::enable_if<std::is_same<int, T>::value, int>::type getValue() const {
        return buff.intValue;
    }

    /**
     * 字符串类型
     * @tparam T
     * @return
     */
    template<typename T>
    typename std::enable_if<std::is_same<std::string, T>::value, std::string>::type getValue() const {
        return buff.stringValue.data();
    }

    /**
     * 返回的是buf的地址值
     * @return
     */
    void *getValuePtr() const {
        if (typeIndex == typeid(int)) {
            return (void *) &buff.intValue;
        }
        if (typeIndex == typeid(std::string)) {
            return (void *) buff.stringValue.data();
        }
        return nullptr;
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
    const std::vector<Object> &toVector() const {
        if (isContainer()) {
            return buff.values;
        }
        return {};
    }

    /**
     * 重新设置缓存区的大小,在这里特指
     * @param size
     */
    void resize(int size) {
        this->buff.stringValue.resize(size);
    }
};

#endif //MAPPER_OBJECT_HPP
