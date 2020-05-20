//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_SQLEXCEPTION_HPP
#define MAPPER_SQLEXCEPTION_HPP

#include <stdexcept>

/**
 * 数据库异常类
 */
class SQLException : public std::runtime_error {
    explicit SQLException(const std::string &message) : runtime_error(message) {}
};

#endif //MAPPER_SQLEXCEPTION_HPP
