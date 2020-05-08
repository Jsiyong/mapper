//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_CONNECTIONPOOL_HPP
#define MAPPER_CONNECTIONPOOL_HPP

#include <util/Singleton.hpp>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Connection.hpp"

/**
 * 数据库连接池
 */
class ConnectionPool : public Singleton<ConnectionPool> {
private:
    std::queue<std::shared_ptr<Connection>> connections;//所有的连接对象
    std::mutex mutex;//互斥锁
    std::condition_variable conditionVariable;//条件变量

    std::once_flag onceFlag;//执行一次标志
    int maxWaitTime = 10;//最大等待时间
    int minConnectionNum = 1;//最小连接数
    int maxConnectionNum = 20;//最大连接数
    int currentConnectionNum = 0;//当前池中的连接数

public:
    ConnectionPool(const ConnectionPool &) = delete;

    ConnectionPool &operator=(const ConnectionPool &) = delete;

    ConnectionPool() = default;

    /**
     * 内部初始化函数
     */
    void initInternal() {
        std::call_once(onceFlag, [this]() {
            //初始化数据库最小连接数
            for (int i = 0; i < this->minConnectionNum; ++i) {
                auto connection = createConnection();
                if (connection) {
                    this->connections.push(connection);
                }
            }
        });
    }

    /**
    * 获取一个数据库连接
    * @return
    */
    std::shared_ptr<Connection> getConnection() {
        std::cout << "connection" << currentConnectionNum << std::endl;
        std::unique_lock<std::mutex> lock(mutex);//加上互斥锁
        std::shared_ptr<Connection> connection = nullptr;
        //移除无效连接
        //若连接已经没用了,ping不通了,直接关闭连接,并移除出队列
        while (!connections.empty() && !connections.front()->ping()) {
            connections.front()->close();//关闭无效连接
            connections.pop();//弹出队
            currentConnectionNum--;
        }

        //进行一番尝试之后,接下来,如果队列不为空,则队首连接是有效连接
        if (!connections.empty()) {
            return popConnection();
        }

        //接下来是队列为空的情况
        //判断是否已经大于最大连接数
        if (currentConnectionNum < maxConnectionNum) {
            //若当前的连接数还没到最大连接数,则创建新的连接
            return createConnection();
        }

        //接下来当前连接数已经达到最大连接数了,无法创建更多的连接,等待一定的时间
        //只有当pred条件为false时调用wait(),才会阻塞当前线程
        if (!conditionVariable.wait_for(lock, std::chrono::seconds(maxWaitTime), [this]() {
            return !connections.empty();
        })) {
            //timeout
            std::cerr << "wait for a connection timeout!!" << std::endl;
            return connection;
        }
        //等待成功了,则获取到那个最新的连接
        return popConnection();
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
        //记上当前连接数
        currentConnectionNum++;
        //连接上,返回
        return connection;
    }


    /**
     * 释放数据库连接
     * @param connection
     */
    void releaseConnection(std::shared_ptr<Connection> connection) {
        if (connection != nullptr) {
            std::unique_lock<std::mutex> lock(mutex);//加上互斥锁
            connections.push(connection);
            conditionVariable.notify_one();//释放互斥锁，通知等待线程
        }
    }

private:

    /**
     * 弹出一个连接,并返回
     * @return
     */
    std::shared_ptr<Connection> popConnection() {
        if (connections.empty()) {
            return nullptr;
        }
        auto connection = connections.front();
        connections.pop();
        return connection;
    }
};

#endif //MAPPER_CONNECTIONPOOL_HPP
