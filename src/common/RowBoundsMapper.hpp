//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_ROWBOUNDSMAPPER_HPP
#define MAPPER_ROWBOUNDSMAPPER_HPP

#include <vector>

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

    }

    /**
    * 根据example条件和RowBounds进行分页查询
    * @param example
    * @param rowBounds
    * @return
    */
    std::vector<T> selectByExampleAndRowBounds(const Example &example, const RowBounds &rowBounds) {

    }

};

#endif //MAPPER_ROWBOUNDSMAPPER_HPP
