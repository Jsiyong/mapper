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
#include <guard/ConnectionGuard.hpp>

/**
 * 查询的帮助类
 */
class QueryHelper {
private:

    /**
     * 数据库公共的查询的步骤的封装
     * @param connection
     * @param sql
     * @param args
     * @return
     */
    static bool
    prepareExecute(const std::shared_ptr<Connection> &connection, const std::string &sql, const Iterable &args) {
        if (!connection->prepare(sql)) {
            return false;
        }
        //绑定
        for (int i = 0; i < args.size(); ++i) {
            connection->bindValue(i, args[i]);
        }
        //执行数据库查询
        return connection->execute();
    }

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
    static void select(const std::string &sql, const Iterable &args, std::vector<Entity> &results) {
        //首先第一步,获取数据库连接
        ConnectionGuard connectionGuard;
        auto connection = connectionGuard.getConnection();
        if (!prepareExecute(connection, sql, args)) {
            return;
        }

        std::map<int, int> idIndexMap;//key id value results的index
        auto exampleTmp = Example<Entity>();
        auto columnMap = exampleTmp.getColumnAliasMap();
        //找出连接的列team
        auto joinColumns = exampleTmp.getJoinEntityColumns();//连接的列,主要用于属性连接
        auto keyColumn = exampleTmp.getKeyEntityColumn();//主键列,主要用于唯一性判断
        auto containerIter = std::find_if(joinColumns.begin(), joinColumns.end(), [](EntityColumn &entityColumn) {
            return entityColumn.isContainer();
        });

        //循环获取结果集
        while (connection->next()) {
            //TODO id暂时为Int型
            int id = 0;

            std::set<std::string> nullJoinPropertySet;//存放所有空连接的字段的集合

            for (int j = 0; j < connection->getRecords().size(); ++j) {
                auto col = columnMap[connection->getRecords()[j]];
                Object value = connection->value(j);
                if (value.isNull()) {
                    //找出joinCoulmns的连接字段是否包含该列,若包含,则加入
                    auto joinPropertyIter = std::find_if(joinColumns.begin(), joinColumns.end(),
                                                         [&col](EntityColumn &entityColumn) {
                                                             return entityColumn.getJoinProperty() == col.getProperty();
                                                         });
                    if (joinPropertyIter != joinColumns.end()) {
                        //保存空的字段,方便还原无效行
                        nullJoinPropertySet.insert(joinPropertyIter->getProperty());
                    }
                }
                col.bindValue2EntityField(connection->value(j));
                //记录当前的id值
                if (nullptr != keyColumn && col.getColumnWithTableAlias() == keyColumn->getColumnWithTableAlias()) {
                    id = connection->value(j).getValue<int>();
                }
            }
            auto entityResult = *exampleTmp.getEntity();
            //如果连接列 [即ON a.id=b.id 中b.id的值为空] 说明连接不到,需要清空连接列表
            //然后再删除实体类的那一项生成的信息
            for (auto &nullProperty:nullJoinPropertySet) {
                //清除空的项
                EntityHelper::clearPropertyValues<Entity>(nullProperty, &entityResult);
            }
            //最普通的,没有容器类型的,只需要直接返回就行
            if (containerIter == joinColumns.end()) {
                results.push_back(entityResult);
                continue;
            }
            //有容器类型的,需要合并
            //没有的话,直接加入
            if (0 != id && idIndexMap.count(id) <= 0) {
                results.push_back(entityResult);
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
    }

    /**
     * 单纯的查询结果的封装
     * @param sql 查询的sql语句
     * @param args 查询的参数
     * @param results 查询的结果,一行一行的结果
     */
    static void
    select(const std::string &sql, const Iterable &args, std::vector<std::vector<Object>> &results) {
        ConnectionGuard connectionGuard;
        auto connection = connectionGuard.getConnection();
        if (!prepareExecute(connection, sql, args)) {
            return;
        }

        //循环获取结果集
        while (connection->next()) {
            std::vector<Object> record;
            for (int i = 0; i < connection->getRecords().size(); ++i) {
                record.emplace_back(connection->value(i));
            }
            results.emplace_back(record);
        }
    }


    /**
     * 执行更新,删除语句,插入语句,返回<first,second>: first 受影响的行数,second 最后一次插入的id
     * @param sql
     * @param args
     * @return
     */
    static std::pair<int, int> execute(const std::string &sql, const Iterable &args) {
        ConnectionGuard connectionGuard;
        auto connection = connectionGuard.getConnection();
        if (!prepareExecute(connection, sql, args)) {
            return {};
        }
        return std::make_pair(connection->getLastAffectedRows(), connection->getLastInsertId());
    }
};

#endif //MAPPER_QUERYHELPER_HPP
