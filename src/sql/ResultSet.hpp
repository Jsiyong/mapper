//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_RESULTSET_HPP
#define MAPPER_RESULTSET_HPP

#include <mysql.h>
#include <Row.hpp>

/**
 * 数据库查询的结果集
 */
class ResultSet {
private:
    MYSQL_RES *mysqlRes = nullptr;
    unsigned int fieldsNum = 0;//获取结果中列的个数
public:
    ResultSet() = default;

    explicit ResultSet(MYSQL_RES *mysqlRes) : mysqlRes(mysqlRes), fieldsNum(mysql_num_fields(mysqlRes)) {}

    ResultSet(ResultSet &&r) {
        mysqlRes = r.mysqlRes;
        r.mysqlRes = nullptr;
        fieldsNum = r.fieldsNum;
    }

    ~ResultSet() {
        if (mysqlRes)
            mysql_free_result(mysqlRes);
    }

    explicit operator bool() {
        return !!mysqlRes;
    }

    ResultSet &operator=(ResultSet &&r) {
        mysql_free_result(mysqlRes);
        mysqlRes = r.mysqlRes;
        fieldsNum = r.fieldsNum;
        r.mysqlRes = nullptr;
        return *this;
    }

    ResultSet &operator=(const ResultSet &) = delete;

    ResultSet(const ResultSet &) = delete;

    Row fetch_row() {
        MYSQL_ROW row = mysql_fetch_row(mysqlRes);
        unsigned long *lengths = mysql_fetch_lengths(mysqlRes);
        return Row{row, lengths};
    }

    inline Row next() {
        return fetch_row();
    }
};

#endif //MAPPER_RESULTSET_HPP
