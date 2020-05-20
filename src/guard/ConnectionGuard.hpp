//
// Created by Jsiyong on 2020-05-20.
//

#ifndef MAPPER_CONNECTIONGUARD_HPP
#define MAPPER_CONNECTIONGUARD_HPP

#include <sql/Connection.hpp>
#include <sql/ConnectionPool.hpp>

/**
 * 数据库连接的安全类
 */
class ConnectionGuard {
private:
    std::shared_ptr<Connection> connection = nullptr;
public:
    ConnectionGuard() {
        connection = ConnectionPool::getInstance()->getConnection();
    }

    /**
     * 获取一个数据库连接
     * @return
     */
    std::shared_ptr<Connection> getConnection() {
        return this->connection;
    }

    /**
     * 析构的时候自动释放数据库连接
     */
    ~ConnectionGuard() {
        ConnectionPool::getInstance()->releaseConnection(connection);
    }
};

#endif //MAPPER_CONNECTIONGUARD_HPP
