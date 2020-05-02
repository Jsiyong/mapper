//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_CONNECTION_HPP
#define MAPPER_CONNECTION_HPP

#include "DatabaseOption.hpp"

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
};

#endif //MAPPER_CONNECTION_HPP
