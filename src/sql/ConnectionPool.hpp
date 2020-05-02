//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_CONNECTIONPOOL_HPP
#define MAPPER_CONNECTIONPOOL_HPP

#include <util/Singleton.hpp>
#include "DataSource.hpp"

/**
 * 数据库连接池
 */
class ConnectionPool : public DataSource, public Singleton<ConnectionPool> {
public:
    /**
    * 获取一个数据库连接
    * @return
    */
    std::shared_ptr<Connection> getConnection() override {

    }
};

#endif //MAPPER_CONNECTIONPOOL_HPP
