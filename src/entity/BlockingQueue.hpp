//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_BLOCKINGQUEUE_HPP
#define MAPPER_BLOCKINGQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <future>

/**
 * 阻塞队列
 * @tparam T 任意类型
 */
template<typename T>
class BlockingQueue {
private:
    std::queue<T> queue;//队列
    std::mutex mutex;//互斥锁
    std::condition_variable conditionVariable;

    int maxWaitTime = 0;//最长等待时间,0表示无限等待,单位为秒
    int maxSize = 0;//阻塞队列最大的数量,0表示无上界

public:
//    BlockingQueue(const BlockingQueue &) = delete;
//
//    BlockingQueue &operator=(const BlockingQueue &) = delete;

    void setMaxWaitTime(int maxWaitTime) {
        this->maxWaitTime = maxWaitTime;
    }

    void setMaxSize(int maxSize) {
        this->maxSize = maxSize;
    }

    //把Object加到BlockingQueue里
    void put(const T &t) {
        std::unique_lock<std::mutex> lock(mutex);//加上互斥锁
        if (queue.size() >= maxSize) {
            throw std::range_error("reached the maximum size,cannot put more object!!");
        }
        queue.push(t);
        conditionVariable.notify_one();//释放互斥锁，通知等待线程
    }

    //取走BlockingQueue里排在首位的对象,若BlockingQueue为空,阻断进入等待状态直到BlockingQueue有新的数据被加入
    T take() {
        std::unique_lock<std::mutex> lock(mutex);//互斥锁加上
        if (queue.empty()) {
            if (conditionVariable.wait_for(lock, std::chrono::seconds(maxWaitTime)) == std::cv_status::timeout) {
                //timeout
                throw std::logic_error("wait for a object timeout!!");
            }
        }
        T t = queue.front();
        queue.pop();
        return t;
    }

    int size() {
        return queue.size();
    }
};

#endif //MAPPER_BLOCKINGQUEUE_HPP
