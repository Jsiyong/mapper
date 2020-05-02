//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_DATABASEOPTION_HPP
#define MAPPER_DATABASEOPTION_HPP

#include <string>

/**
 * 数据库配置类
 */
struct DatabaseOption {
    std::string host = "127.0.0.1";//数据库ip地址
    int port = 3307;//端口号
    std::string username = "root";//用户名
    std::string password = "123456";//数据库密码
    std::string databaseName = "test";//数据库名
    int connectTimeout = 0;//连接的超时时间
    std::string charsetName = "utf8";//编码
    int reconnectTime = 1;//超时重连次数
};

#endif //MAPPER_DATABASEOPTION_HPP
