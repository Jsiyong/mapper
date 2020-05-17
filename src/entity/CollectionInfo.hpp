//
// Created by Jsiyong on 2020-05-15.
//

#ifndef MAPPER_COLLECTIONINFO_HPP
#define MAPPER_COLLECTIONINFO_HPP

#include "Any.hpp"

/**
 * 容器信息
 */
class CollectionInfo {
public:
    enum class CollectionType {
        Null, List, Set, Vector
    };
private:
    CollectionType collectionType = CollectionType::Null;//容器类型
    int size = 0;//容器大小
    std::vector<Any> values;//将容器中的所有值都保存下来,方便SQL绑定

public:
    CollectionType getCollectionType() const {
        return collectionType;
    }

    int getSize() const {
        return size;
    }

    const std::vector<Any> &getValues() const {
        return values;
    }

    CollectionInfo(CollectionType collectionType, const std::vector<Any> &values)
            : collectionType(collectionType), values(values) {
        this->size = values.size();
    }

    CollectionInfo() = default;
};

#endif //MAPPER_COLLECTIONINFO_HPP
