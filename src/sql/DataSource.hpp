//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_DATASOURCE_HPP
#define MAPPER_DATASOURCE_HPP

#include <memory>
#include "Connection.hpp"

/**
 * 连接池基类
 */
class DataSource {
public:
    /**
     * 获取一个数据库连接
     * @return
     */
    virtual std::shared_ptr<Connection> getConnection() = 0;

    /**
     * 释放数据库连接
     * @param connection
     */
    virtual void releaseConnection(std::shared_ptr<Connection> connection) = 0;

};

#endif //MAPPER_DATASOURCE_HPP
