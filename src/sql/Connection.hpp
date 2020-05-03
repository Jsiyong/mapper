//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_CONNECTION_HPP
#define MAPPER_CONNECTION_HPP

#include "DatabaseOption.hpp"
#include <boost/any.hpp>

/**
 * 数据库连接接口
 */
class Connection {
public:
    /**
     * 数据库连接的初始化配置
     * @param databaseOption
     */
    virtual void setOption(const DatabaseOption &databaseOption) = 0;

    /**
     * 关闭连接
     */
    virtual void close() = 0;

    /**
     * 连接数据库
     * @return
     */
    virtual bool connect() = 0;

    /**
     * 测试数据库连接是否能联通
     * @return
     */
    virtual bool ping() = 0;

    /**
     * 获取最后受影响的行数
     * @return
     */
    virtual int getLastAffectedRows() = 0;

    /**
     * 开启事务
     * @return
     */
    virtual bool begin() = 0;

    /**
     * 提交事务
     * @return
     */
    virtual bool commit() = 0;

    /**
     * 回滚事务
     * @return
     */
    virtual bool rollback() = 0;

    /**
     * 执行数据库查询
     * @return
     */
    virtual bool execute() = 0;

    /**
     * 给placeholder相对应的位置绑定参数
     * @param pos 位置
     * @param value 值
     */
    virtual void bindValue(int pos, const boost::any &value) = 0;

    /**
     * 预处理
     * @param query
     * @return
     */
    virtual bool prepare(const std::string &query) = 0;

    /**
     * 获取下一条记录
     * @return
     */
    virtual bool next() = 0;

    /**
     * 提取结果集中的结果
     * @param index
     * @return
     */
    virtual boost::any value(int index) = 0;

};

#endif //MAPPER_CONNECTION_HPP
