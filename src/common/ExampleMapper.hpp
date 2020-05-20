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

        std::map<int, int> idIndexMap;//key id value results的index
        //找出连接的列team
        auto joinColumns = example.getJoinEntityColumns();//连接的列,主要用于属性连接
        auto keyColumn = example.getKeyEntityColumn();//主键列,主要用于唯一性判断
        auto containerIter = std::find_if(joinColumns.begin(), joinColumns.end(), [](EntityColumn &entityColumn) {
            return entityColumn.isContainer();
        });


        while (connector->next()) {
            auto tmp = Example<T>();
            auto columnMap = tmp.getColumnAliasMap();

            //TODO id暂时为Int型
            int id = 0;

            for (int j = 0; j < connector->getRecords().size(); ++j) {
//                std::cout << connector->getRecords()[j] << std::endl;
                auto col = columnMap[connector->getRecords()[j]];
                col.bindValue2EntityField(connector->value(j));

                //记录当前的id值
                if (nullptr != keyColumn && col.getColumnWithTableAlias() == keyColumn->getColumnWithTableAlias()) {
                    id = connector->value(j).getValue<int>();
                }
            }
            //最普通的,没有容器类型的,只需要直接返回就行
            if (containerIter == joinColumns.end()) {
                results.push_back(tmp.getEntity());
                continue;
            }
            //有容器类型的,需要合并
            //没有的话,直接加入
            if (0 != id && idIndexMap.count(id) <= 0) {
                results.push_back(tmp.getEntity());
                //记录插入的id相对应于列表的索引值
                idIndexMap.insert(std::make_pair(id, results.size() - 1));
            } else if (idIndexMap.count(id) > 0) {
                //获取对应的实体
                auto e = results[idIndexMap[id]];
                //一个个插入
                for (const auto &joinColumn:joinColumns) {
                    EntityHelper::appendPropertyValues<T>(joinColumn.getProperty(), tmp.getEntity().get(), e.get());
                }
            }
        }
        //TODO 释放连接的时候需要同时清空连接的资源
//        ConnectionPool::getInstance()->releaseConnection(connector);
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
