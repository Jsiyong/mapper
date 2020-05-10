//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_CRITERION_HPP
#define MAPPER_CRITERION_HPP

#include <string>
#include <util/TypeUtils.hpp>

/**
 * 通用的一个标准
 */
class Criterion {
private:
    const std::string AND = "AND";
    const std::string OR = "OR";

private:
    std::string condition = "";
    void *value = nullptr;//传入的比较值,之后可以通过实体字段类型获取
    void *secondValue = nullptr;//一般between会有第二个值

    std::string andOr = "";//and和or的连接语句
    bool noValue = false;//是否是无值的
    bool singleValue = false;//是否是单值
    bool betweenValue = false;//是否是between
    bool listValue = false;//是否是列表值
    std::string typeHandler = "";

public:

    template<typename Object>
    Criterion(const std::string &condition, const Object &value, const Object &secondValue,
              const std::string &typeHandler, bool isOr) {
        this->condition = condition;
        this->value = (void *) &value;
        this->secondValue = (void *) &secondValue;
        this->typeHandler = typeHandler;
        this->betweenValue = true;
        this->andOr = isOr ? OR : AND;
    }

    template<typename Object>
    Criterion(const std::string &condition, const Object &value, const std::string &typeHandler, bool isOr) {
        this->condition = condition;
        this->value = (void *) &value;
        this->typeHandler = typeHandler;
        this->andOr = isOr ? OR : AND;
        //判断是不是集合类型
        if (TypeUtils::isCollection(value)) {
            this->listValue = true;
        } else {
            this->singleValue = true;
        }
    }

    explicit Criterion(const std::string &condition) {
        new(this)Criterion(condition, false);
    }

    Criterion(const std::string &condition, bool isOr) {
        this->condition = condition;
        this->noValue = true;
        this->andOr = isOr ? OR : AND;
    }
};

#endif //MAPPER_CRITERION_HPP
