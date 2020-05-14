//
// Created by Jsiyong on 2020-05-13.
//

#ifndef MAPPER_SQLCONSTANTS_HPP
#define MAPPER_SQLCONSTANTS_HPP

#include <string>
/**
 * 包含了通用Mapper中的一些常量值
 */
namespace SQLConstants {
    const static std::string AND = "AND";
    const static std::string OR = "OR";

    const static std::string IS_NULL = "IS NULL";
    const static std::string IS_NOT_NULL = "IS NOT NULL";

    const static std::string EQUALl_TO = "=";
    const static std::string NOT_EQUALl_TO = "<>";
    const static std::string LESS_THAN = "<";
    const static std::string LESS_THAN_OR_EQUAL_TO = "<=";
    const static std::string GREATER_THAN = ">";
    const static std::string GREATER_THAN_OR_EQUAL_TO = ">=";

    const static std::string LIKE = "LIKE";
    const static std::string NOT_LIKE = "NOT LIKE";

    const static std::string IN = "IN";
    const static std::string NOT_IN = "NOT IN";

    const static std::string BETWEEN = "BETWEEN";
    const static std::string NOT_BETWEEN = "NOT BETWEEN";

    const static std::string REGEXP = "REGEXP";
    const static std::string NOT_REGEXP = "NOT REGEXP";
}
#endif //MAPPER_SQLCONSTANTS_HPP
