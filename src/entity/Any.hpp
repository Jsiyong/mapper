//
// Created by Jsiyong on 2020-05-17.
//

#ifndef MAPPER_ANY_HPP
#define MAPPER_ANY_HPP

#include <typeindex>

/**
 * 简单包装一下any类型,方便之后取出来使用
 */
class Any {
private:
    void *value = nullptr;//存放的是值的地址
    std::type_index typeIndex = std::type_index(typeid(void));//存放的是值的类型
public:
    template<typename Object>
    explicit Any(const Object& value):value((void *) &value), typeIndex(typeid(Object)) {};

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
