//
// Created by Jsiyong on 2020-05-13.
//

#ifndef MAPPER_EXCEPTION_HPP
#define MAPPER_EXCEPTION_HPP

#include <stdexcept>

class Exception : public std::runtime_error {
public:
    explicit Exception(const std::string &msg) : std::runtime_error(msg) {

    }
};

#endif //MAPPER_EXCEPTION_HPP
