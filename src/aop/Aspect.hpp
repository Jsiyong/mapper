//
// Created by Jsiyong on 2020-05-13.
//

#ifndef MAPPER_ASPECT_HPP
#define MAPPER_ASPECT_HPP

#include <type_traits>
#include <utility>
#include "Exception.hpp"

template<typename U>
class Aspect {
    U *u = nullptr;
public:
    explicit Aspect(U *u) : u(u) {}

    /**
     * 内部环绕,主要用于自动清理资源等
     */
    class InnerAround {
    private:
        Aspect *aspect = nullptr;
    public:
        explicit InnerAround(Aspect *aspect) : aspect(aspect) {
            aspect->before();
        }

        ~InnerAround() {
            if (aspect)
                aspect->afterReturning();
        }
    };

protected:
    /**
     * 紧接着在做完业务之后执行
     */
    virtual void after() {}

    /**
     * 在业务开始的时候执行
     */
    virtual void before() {}

    /**
     * 在业务抛异常的时候执行
     */
    virtual void afterThrowing() {}

    /**
     * 在业务返回的之前[包括抛异常]执行
     */
    virtual void afterReturning() {}

public:

    /**
     * 若返回值为void
     * @tparam Func
     * @tparam Args
     * @param t
     * @param args
     * @return
     */
    template<typename Func, typename ...Args>
    auto invoke(Func &&t, Args &&...args)
    -> typename std::enable_if<
            std::is_void<decltype((u->*t)(std::forward<Args>(args)...))>::value,
            void
    >::type {
        InnerAround around(this);//利用局部变量特性，离开作用域自动析构
        try {
            (u->*t)(std::forward<Args>(args)...);
            after();
        } catch (Exception &exception) {
            afterThrowing();
            //异常外抛
            throw exception;
        }
    }

    /**
     * 若返回值不为void
     * @tparam Func
     * @tparam Args
     * @param t
     * @param args
     * @return
     */
    template<typename Func, typename ...Args>
    auto invoke(Func &&t, Args &&...args)
    -> typename std::enable_if<
            !std::is_void<decltype((u->*t)(std::forward<Args>(args)...))>::value,
            decltype((u->*t)(std::forward<Args>(args)...))
    >::type {
        InnerAround around(this);//利用局部变量特性，离开作用域自动析构
        try {
            auto ret = (u->*t)(std::forward<Args>(args)...);
            after();
            return ret;
        } catch (Exception &exception) {
            afterThrowing();
            //异常外抛
            throw exception;
        }
    }
};

#endif //MAPPER_ASPECT_HPP
