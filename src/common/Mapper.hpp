//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_MAPPER_HPP
#define MAPPER_MAPPER_HPP

#include <entity/Example.hpp>
#include "BaseMapper.hpp"
#include "ExampleMapper.hpp"

/**
 * 通用Mapper接口,其他接口继承该接口即可
 * @tparam T
 */
template<typename T>
class Mapper : public BaseMapper<T>, public ExampleMapper<T> {

};

#endif //MAPPER_MAPPER_HPP
