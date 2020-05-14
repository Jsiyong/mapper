//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_CRITERION_HPP
#define MAPPER_CRITERION_HPP

#include <string>
#include <util/TypeUtils.hpp>
#include "SQLConstants.hpp"

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
    CollectionInfo collectionInfo;//如果是列表,列表容器的信息
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

    const CollectionInfo &getCollectionInfo() const {
        return collectionInfo;
    }

public:

    template<typename Object>
    Criterion(const std::string &condition, const Object &value, const Object &secondValue, bool isOr = false) {
        this->condition = condition;
        this->value = (void *) &value;
        this->secondValue = (void *) &secondValue;
        this->betweenValue = true;
        this->andOr = isOr ? SQLConstants::OR : SQLConstants::AND;
    }

    template<typename Object>
    Criterion(const std::string &condition, const Object &value, bool isOr = false) {
        this->condition = condition;
        this->value = (void *) &value;
        this->andOr = isOr ? SQLConstants::OR : SQLConstants::AND;
        //判断是不是集合类型
        auto listInfo = TypeUtils::getCollectionInfo(value);
        if (listInfo.getCollectionType() != CollectionInfo::CollectionType::Null) {
            this->listValue = true;
            this->collectionInfo = listInfo;
            this->collectionInfo.setValue(this->value);
        } else {
            this->singleValue = true;
        }
    }

    explicit Criterion(const std::string &condition, bool isOr = false) {
        this->condition = condition;
        this->noValue = true;
        this->andOr = isOr ? SQLConstants::OR : SQLConstants::AND;
    }
};

#endif //MAPPER_CRITERION_HPP
