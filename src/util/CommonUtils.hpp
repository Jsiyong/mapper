//
// Created by Jsiyong on 2020-05-16.
//

#ifndef MAPPER_COMMONUTILS_HPP
#define MAPPER_COMMONUTILS_HPP

#include <random>
#include <time.h>

/**
 * 公用的工具类
 */
class CommonUtils {
public:

    /**
     * 获取某个范围内的随机整数
     * @param start
     * @param end
     * @return
     */
    static int getRandomNum(int min, int max) {
        //需要设置一个随机数种子，一般用当前时间作为种子，否则每次运行获取到的随机数序列是相同的
        std::default_random_engine randomEngine(std::random_device{}());
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(randomEngine);
    }


};

#endif //MAPPER_COMMONUTILS_HPP
