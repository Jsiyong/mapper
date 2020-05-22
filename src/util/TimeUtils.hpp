//
// Created by Jsiyong on 2020-05-22.
//

#ifndef MAPPER_TIMEUTILS_HPP
#define MAPPER_TIMEUTILS_HPP

#include <mysql.h>
#include <ctime>

/**
 * 时间的工具类
 */
class TimeUtils {
public:
    /**
     * C语言时间类型转为数据库时间类型
     * @param time
     * @return
     */
    static MYSQL_TIME convertTime2MysqlTime(std::time_t time) {
        struct tm *timeTmp = localtime(&time);
        return {(unsigned int) 1900 + timeTmp->tm_year,
                (unsigned int) 1 + timeTmp->tm_mon,
                (unsigned int) timeTmp->tm_mday,
                (unsigned int) timeTmp->tm_hour,
                (unsigned int) timeTmp->tm_min,
                (unsigned int) timeTmp->tm_sec
        };
    }

    static std::time_t convertMysqlTime2Time(MYSQL_TIME mysqlTime) {
        struct tm timeTmp;
        timeTmp.tm_year = mysqlTime.year - 1900;
        timeTmp.tm_mon = mysqlTime.month - 1;
        timeTmp.tm_mday = mysqlTime.day;
        timeTmp.tm_hour = mysqlTime.hour;
        timeTmp.tm_min = mysqlTime.minute;
        timeTmp.tm_sec = mysqlTime.second;
        return mktime(&timeTmp);
    }
};

#endif //MAPPER_TIMEUTILS_HPP
