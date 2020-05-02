//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_ROW_HPP
#define MAPPER_ROW_HPP

#include <mysql.h>
#include <algorithm>
#include <string>

/**
 * 查询的结果行
 */
class Row {
private:
    MYSQL_ROW row;
    unsigned long *lengths;
public:
    Row() : row(nullptr), lengths(nullptr) {}

    Row(MYSQL_ROW row, unsigned long *lengths) : row(row), lengths(lengths) {}

    Row(Row &&x) : row(x.row), lengths(x.lengths) {
        x.row = nullptr;
        x.lengths = nullptr;
    }

    Row &operator=(Row &&x) {
        std::swap(*this, x);
        return *this;
    }

    operator bool() {
        return !!row;
    }

    std::string operator[](size_t n) {
        if (n >= *lengths)
            return "";
        return std::string(row[n], lengths[n]);
    }

    std::string toString(size_t n) {
        return std::string(row[n], lengths[n]);
    }

    int toInt(size_t n) {
        auto str = std::string(row[n], lengths[n]);
        return std::stoi(str);
    }

    int toLong(size_t n) {
        auto str = std::string(row[n], lengths[n]);
        return std::stoll(str);
    }
};

#endif //MAPPER_ROW_HPP
