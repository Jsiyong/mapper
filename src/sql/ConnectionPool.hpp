//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_CONNECTIONPOOL_HPP
#define MAPPER_CONNECTIONPOOL_HPP

#include <util/Singleton.hpp>
#include <entity/BlockingQueue.hpp>
#include "Connection.hpp"

/**
 * 数据库连接池
 */
class ConnectionPool : public Singleton<ConnectionPool> {
private:
    BlockingQueue<std::shared_ptr<Connection>> connections;//所有的连接对象
    std::once_flag onceFlag;//执行一次标志
    int maxWaitTime = 10;//最大等待时间
    int minConnectionNum = 1;//最小连接数
    int maxConnectionNum = 20;//最大连接数

public:
    ConnectionPool(const ConnectionPool &) = delete;

    ConnectionPool &operator=(const ConnectionPool &) = delete;

    ConnectionPool() = default;

    /**
     * 内部初始化函数
     */
    void initInternal() {
        connections.setMaxSize(maxConnectionNum);
        connections.setMaxWaitTime(maxWaitTime);
        std::call_once(onceFlag, [this]() {
            //初始化数据库最小连接数
            for (int i = 0; i < this->minConnectionNum; ++i) {
                auto connection = createConnection();
                if (connection) {
                    this->connections.put(connection);
                }
            }
        });
    }

    /**
    * 获取一个数据库连接
    * @return
    */
    std::shared_ptr<Connection> getConnection() {
        std::shared_ptr<Connection> connection = nullptr;
        try {
            connection = connections.take();
            //若连接失效,或者当前连接数小于最大连接数
            if (!connection || !connection->ping() || connections.size() < maxConnectionNum) {
                //重新创建一条连接
                connection = createConnection();
            }
        } catch (std::runtime_error &error) {
            //获取连接超时
            std::cerr << "timeout or other error!!" << error.what() << std::endl;
        }
        std::cout << "now get: " << connection << " - " << connections.size() << std::endl;
        return connection;
    }


    /**
     * 创建一个连接
     * @return
     */
    std::shared_ptr<Connection> createConnection() {
        auto connection = std::make_shared<Connection>();
        if (!connection->connect()) {
            return nullptr;
        }
        //连接上,返回
        return connection;
    }


    /**
     * 释放数据库连接
     * @param connection
     */
    void releaseConnection(std::shared_ptr<Connection> connection) {
        if (!connection) {
            return;
        }
        try {
            connections.put(connection);
        } catch (...) {
            std::cerr << "release error!!" << std::endl;
        }
        std::cout << "now release: " << connection << " - " << connections.size() << std::endl;
    }
};

#endif //MAPPER_CONNECTIONPOOL_HPP
