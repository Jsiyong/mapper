//
// Created by Jsiyong on 2020-05-20.
//

#ifndef MAPPER_QUERYHELPER_HPP
#define MAPPER_QUERYHELPER_HPP

#include <string>
#include <vector>
#include <entity/Iterable.hpp>
#include <sql/ConnectionPool.hpp>
#include <entity/EntityColumn.hpp>

/**
 * 查询的帮助类
 */
class QueryHelper {
private:
public:

    /**
     * 通过查询sql语句和查询参数列表查询结果集
     * @tparam Entity
     * @param sql 查询的sql语句
     * @param args 查询参数
     * @param results 返回的结果集
     * @return
     */
    template<typename Entity>
    static std::vector<Entity> select(const std::string &sql, const Iterable &args, std::vector<Entity> &results) {
        //首先第一步,获取数据库连接
        auto connector = ConnectionPool::getInstance()->getConnection();
        connector->prepare(sql);
        //绑定
        for (int i = 0; i < args.size(); ++i) {
            connector->bindValue(i, args[i]);
        }
        //执行数据库查询
        connector->execute();

        std::map<int, int> idIndexMap;//key id value results的index

        //循环获取结果集
        while (connector->next()) {
            auto exampleTmp = Example<Entity>();
            auto columnMap = exampleTmp.getColumnAliasMap();
            //找出连接的列team
            auto joinColumns = exampleTmp.getJoinEntityColumns();//连接的列,主要用于属性连接
            auto keyColumn = exampleTmp.getKeyEntityColumn();//主键列,主要用于唯一性判断
            auto containerIter = std::find_if(joinColumns.begin(), joinColumns.end(), [](EntityColumn &entityColumn) {
                return entityColumn.isContainer();
            });

            //TODO id暂时为Int型
            int id = 0;

            for (int j = 0; j < connector->getRecords().size(); ++j) {
                auto col = columnMap[connector->getRecords()[j]];
                col.bindValue2EntityField(connector->value(j));
                //记录当前的id值
                if (nullptr != keyColumn && col.getColumnWithTableAlias() == keyColumn->getColumnWithTableAlias()) {
                    id = connector->value(j).getValue<int>();
                }
            }
            //最普通的,没有容器类型的,只需要直接返回就行
            if (containerIter == joinColumns.end()) {
                results.push_back(*exampleTmp.getEntity());
                continue;
            }
            //有容器类型的,需要合并
            //没有的话,直接加入
            if (0 != id && idIndexMap.count(id) <= 0) {
                results.push_back(*exampleTmp.getEntity());
                //记录插入的id相对应于列表的索引值
                idIndexMap.insert(std::make_pair(id, results.size() - 1));
            } else if (idIndexMap.count(id) > 0) {
                //获取对应的实体,必须是引用
                auto &entity = results[idIndexMap[id]];
                //一个个插入
                for (const auto &joinColumn:joinColumns) {
                    EntityHelper::appendPropertyValues<Entity>(joinColumn.getProperty(),
                                                               exampleTmp.getEntity().get(), &entity);
                }
            }
        }

        //TODO 释放连接的时候需要同时清空连接的资源
//        ConnectionPool::getInstance()->releaseConnection(connector);
        return results;
    }
};

#endif //MAPPER_QUERYHELPER_HPP
