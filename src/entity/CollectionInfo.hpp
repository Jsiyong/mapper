//
// Created by Jsiyong on 2020-05-15.
//

#ifndef MAPPER_COLLECTIONINFO_HPP
#define MAPPER_COLLECTIONINFO_HPP

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
    void *value = nullptr;//容器内容
public:
    CollectionType getCollectionType() const {
        return collectionType;
    }

    void setCollectionType(CollectionType collectionType) {
        CollectionInfo::collectionType = collectionType;
    }

    int getSize() const {
        return size;
    }

    void setSize(int size) {
        CollectionInfo::size = size;
    }

    void *getValue() const {
        return value;
    }

    void setValue(void *value) {
        CollectionInfo::value = value;
    }

    CollectionInfo(CollectionType collectionType, int size) : collectionType(collectionType), size(size) {}

    CollectionInfo(CollectionType collectionType, int size, void *value) : collectionType(collectionType), size(size),
                                                                           value(value) {}

    CollectionInfo() = default;
};

#endif //MAPPER_COLLECTIONINFO_HPP
