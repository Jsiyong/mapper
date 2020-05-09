//
// Created by Jsiyong on 2020-05-09.
//

#ifndef MAPPER_ABSTRACTSQLBUILDER_HPP
#define MAPPER_ABSTRACTSQLBUILDER_HPP

#include <string>
#include <vector>

/**
 * 基类sql构造器
 */
class AbstractSqlBuilder {
protected:
    /**
     * 给定分隔符,构造sql
     * @param sql
     * @param list 欲连接的列表
     * @param init sql的开头
     * @param sep 分割符
     */
    void
    append(std::string &sql, const std::vector<std::string> &list, const std::string &init, const std::string &sep) {
        bool first = true;//标记开头
        for (const auto &s:list) {
            sql += first ? init : sep;
            sql += s;
            first = false;
        }
    }
};

#endif //MAPPER_ABSTRACTSQLBUILDER_HPP
