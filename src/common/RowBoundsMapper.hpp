//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_ROWBOUNDSMAPPER_HPP
#define MAPPER_ROWBOUNDSMAPPER_HPP

#include <vector>
#include <entity/Example.hpp>
#include <entity/RowBounds.hpp>
#include "ExampleMapper.hpp"

/**
 * 分页查询的mapper
 */
template<typename T>
class RowBoundsMapper {
public:
    /**
    * 根据实体属性和RowBounds进行分页查询
    * @param record
    * @param rowBounds
    * @return
    */
    std::vector<T> selectByRowBounds(const T &record, const RowBounds &rowBounds) {
        Example<T> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(record);
        example.limit(rowBounds.getOffset(), rowBounds.getLimit());
        return ExampleMapper<T>().selectByExample(example);
    }

    /**
    * 根据example条件和RowBounds进行分页查询
    * @param example
    * @param rowBounds
    * @return
    */
    std::vector<T> selectByExampleAndRowBounds(const Example<T> &example, const RowBounds &rowBounds) {
        auto exampleInner = example;
        auto criteria = exampleInner.createCriteria();
        exampleInner.limit(rowBounds.getOffset(), rowBounds.getLimit());
        return ExampleMapper<T>().selectByExample(exampleInner);
    }
};

#endif //MAPPER_ROWBOUNDSMAPPER_HPP
