//
// Created by Jsiyong on 2020-05-11.
//

#ifndef MAPPER_MAPPEREXCEPTION_HPP
#define MAPPER_MAPPEREXCEPTION_HPP

#include <stdexcept>

/**
 * mapper异常类
 */
class MapperException : std::runtime_error {
public:
    explicit MapperException(const std::string &message) : runtime_error(message) {}
};

#endif //MAPPER_MAPPEREXCEPTION_HPP
