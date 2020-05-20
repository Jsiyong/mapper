//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_EXAMPLEMAPPER_HPP
#define MAPPER_EXAMPLEMAPPER_HPP

#include <vector>
#include <entity/Example.hpp>
#include <entity/RowBounds.hpp>
#include <util/QueryHelper.hpp>

/**
 * Example查询
 * @tparam T
 */
template<typename T>
class ExampleMapper {
public:

    /////////////////////////// select /////////////////////////////
    /**
     * 根据Example条件进行查询
     * @param example
     * @return
     */
    std::vector<T> selectByExample(const Example<T> &example) {
        std::vector<T> results;
        auto sql = example.getSelectStatementByExample();
        auto prepareValues = example.getPrepareValues();
        return QueryHelper::select(sql, prepareValues, results);
    }

    /**
     * 根据Example条件进行查询
     * @param example
     * @return
     */
    T selectOneByExample(const Example<T> &example) {
        auto results = this->selectByExample(example);
        return results.empty() ? T{} : results.front();
    }

    /**
     * 根据Example条件进行查询总数
     * @param example
     * @return
     */
    int selectCountByExample(const Example<T> &example) {
        std::vector<std::vector<Object>> results;
        auto sql = example.getSelectCountStatementByExample();
        auto prepareValues = example.getPrepareValues();
        QueryHelper::select(sql, prepareValues, results);
        return results.front().front().getValue<int>();
    }

    /////////////////////////// delete /////////////////////////////
    /**
     * 根据Example条件删除数据
     * @param example
     * @return
     */
    int deleteByExample(const Example<T> &example) {

    }

    /////////////////////////// update /////////////////////////////

    /**
     * 根据Example条件更新实体`record`包含的全部属性，null值会被更新
     * @param record
     * @param example
     * @return
     */
    int updateByExample(const T &record, const Example<T> &example) {

    }

    /**
     * 根据Example条件更新实体`record`包含的不是null的属性值
     * @param record
     * @param example
     * @return
     */
    int updateByExampleSelective(const T &record, const Example<T> &example) {

    }

};

#endif //MAPPER_EXAMPLEMAPPER_HPP
