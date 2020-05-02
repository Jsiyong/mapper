//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_BLOCKINGQUEUE_HPP
#define MAPPER_BLOCKINGQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <boost/noncopyable.hpp>

/**
 * 阻塞队列
 * @tparam T 任意类型
 */
template<typename T>
class BlockingQueue : public boost::noncopyable {
private:
    std::queue<T> queue;//队列
    std::mutex mutex;//互斥锁
    std::condition_variable conditionVariable;
public:
    //把Object加到BlockingQueue里
    void put(const T &t) {
        std::unique_lock<std::mutex> lock(mutex);//加上互斥锁
        queue.push(t);
        conditionVariable.notify_one();//释放互斥锁，通知等待线程
    }

    //取走BlockingQueue里排在首位的对象,若BlockingQueue为空,阻断进入等待状态直到BlockingQueue有新的数据被加入
    T take() {
        std::unique_lock<std::mutex> lock(mutex);//互斥锁加上
        if (queue.empty()) {
            //若队列为空，则等待互斥锁，当调用wait方法时会unlock
            conditionVariable.wait(lock);
        }
        T t = queue.front();
        queue.pop();
        return t;
    }

};

#endif //MAPPER_BLOCKINGQUEUE_HPP
