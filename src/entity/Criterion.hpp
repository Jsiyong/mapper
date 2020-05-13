//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_CRITERION_HPP
#define MAPPER_CRITERION_HPP

#include <string>
#include <util/TypeUtils.hpp>
#include "Constants.hpp"

/**
 * 通用的一个标准
 */
class Criterion {
private:
    std::string condition;
    void *value = nullptr;//传入的比较值,之后可以通过实体字段类型获取
    void *secondValue = nullptr;//一般between会有第二个值

    std::string andOr;//and和or的连接语句
    bool noValue = false;//是否是无值的
    bool singleValue = false;//是否是单值
    bool betweenValue = false;//是否是between
    bool listValue = false;//是否是列表值
    int listSize = 0;//如果是列表,列表大小
    std::string typeHandler;

public:
    const std::string &getCondition() const {
        return condition;
    }

    void *getValue() const {
        return value;
    }

    void *getSecondValue() const {
        return secondValue;
    }

    const std::string &getAndOr() const {
        return andOr;
    }

    bool isNoValue() const {
        return noValue;
    }

    bool isSingleValue() const {
        return singleValue;
    }

    bool isBetweenValue() const {
        return betweenValue;
    }

    bool isListValue() const {
        return listValue;
    }

    const std::string &getTypeHandler() const {
        return typeHandler;
    }

    int getListSize() const {
        return listSize;
    }

public:

    template<typename Object>
    Criterion(const std::string &condition, const Object &value, const Object &secondValue,
              const std::string &typeHandler, bool isOr) {
        this->condition = condition;
        this->value = (void *) &value;
        this->secondValue = (void *) &secondValue;
        this->typeHandler = typeHandler;
        this->betweenValue = true;
        this->andOr = isOr ? Constants::OR : Constants::AND;
    }

    template<typename Object>
    Criterion(const std::string &condition, const Object &value, const std::string &typeHandler, bool isOr = false) {
        this->condition = condition;
        this->value = (void *) &value;
        this->typeHandler = typeHandler;
        this->andOr = isOr ? Constants::OR : Constants::AND;
        //判断是不是集合类型
        if (TypeUtils::isCollection(value, this->listSize)) {
            this->listValue = true;
// TODO           this->listSize = value.size();
        } else {
            this->singleValue = true;
        }
    }

    template<typename Object>
    Criterion(const std::string &condition, const Object &value, bool isOr = false) {
        new(this)Criterion(condition, value, std::move(std::string()), isOr);
    }

    explicit Criterion(const std::string &condition, bool isOr = false) {
        this->condition = condition;
        this->noValue = true;
        this->andOr = isOr ? Constants::OR : Constants::AND;
    }
};

#endif //MAPPER_CRITERION_HPP
