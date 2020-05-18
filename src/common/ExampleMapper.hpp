//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_EXAMPLEMAPPER_HPP
#define MAPPER_EXAMPLEMAPPER_HPP

#include <vector>
#include <entity/Example.hpp>
#include <entity/RowBounds.hpp>

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
    std::vector<std::shared_ptr<T>> selectByExample(const Example<T> &example) {
        std::vector<std::shared_ptr<T>> results;
        auto sql = example.getSelectStatementByExample();
        auto prepareValues = example.getPrepareValues();

        auto connector = ConnectionPool::getInstance()->getConnection();
        connector->prepare(sql);
        //绑定
        for (int i = 0; i < prepareValues.size(); ++i) {
            connector->bindValue(i, prepareValues[i]);
        }
        connector->execute();
        while (connector->next()) {
            auto tmp = Example<T>();
            auto columnMap = tmp.getColumnAliasMap();
            for (int j = 0; j < connector->getRecords().size(); ++j) {
//                std::cout << connector->getRecords()[j] << std::endl;
                auto c = columnMap[connector->getRecords()[j]];
                c.bindValue2EntityField(connector->value(j));
            }
            results.push_back(tmp.getEntity());
        }
        ConnectionPool::getInstance()->releaseConnection(connector);
        return results;
    }

    /**
     * 根据Example条件进行查询
     * @param example
     * @return
     */
    T selectOneByExample(const Example<T> &example) {

    }

    /**
     * 根据Example条件进行查询总数
     * @param example
     * @return
     */
    int selectCountByExample(const Example<T> &example) {

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
