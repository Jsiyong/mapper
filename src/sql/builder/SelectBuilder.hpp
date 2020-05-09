//
// Created by Jsiyong on 2020-05-09.
//

#ifndef MAPPER_SELECTBUILDER_HPP
#define MAPPER_SELECTBUILDER_HPP

#include <memory>
#include "AbstractSqlBuilder.hpp"

/**
 * select语句的生成器
 */
class SelectBuilder : public AbstractSqlBuilder {
private:

    std::vector<std::string> columns;//列
    std::vector<std::string> tables;//表
    std::vector<std::string> joins;//连接的表
    std::vector<std::string> leftJoins;//左连接的表
    std::vector<std::string> wheres;//where语句
    std::vector<std::string> groupBys;//分组
    std::vector<std::string> havings;
//    std::vector<std::shared_ptr<SelectBuilder>> unions;
    std::vector<std::string> orderBys;
    bool _distinct = false;
    int _limit = 0;
    int _offset = 0;
    bool _forUpdate = false;
    bool _noWait = false;

public:

    SelectBuilder &select(const std::string &expr) {
        columns.emplace_back(expr);
        return *this;
    }

    SelectBuilder &where(const std::string &expr) {
        wheres.emplace_back(expr);
        return *this;
    }

    SelectBuilder &orderBy(const std::string &name) {
        orderBys.emplace_back(name);
        return *this;
    }


    SelectBuilder &leftJoin(const std::string &join) {
        leftJoins.emplace_back(join);
        return *this;
    }

    SelectBuilder &join(const std::string &join) {
        joins.emplace_back(join);
        return *this;
    }

    SelectBuilder &having(const std::string &expr) {
        havings.emplace_back(expr);
        return *this;
    }

    SelectBuilder &groupBy(const std::string &expr) {
        groupBys.emplace_back(expr);
        return *this;
    }

    SelectBuilder &from(const std::string &table) {
        tables.emplace_back(table);
        return *this;
    }

    SelectBuilder &forUpdate() {
        _forUpdate = true;
        return *this;
    }

    SelectBuilder &distinct() {
        _distinct = true;
        return *this;
    }

    SelectBuilder &limit(int row) {
        return limit(row, 0);
    }

    SelectBuilder &limit(int row, int offset) {
        _limit = row;
        _offset = offset;
        return *this;
    }

    std::string toString() {
        std::string sql = "select ";
        if (_distinct) {
            sql += "distinct ";
        }
        append(sql, columns, "", ", ");
        append(sql, tables, " from ", ", ");
        append(sql, joins, " join ", " join ");
        append(sql, leftJoins, " left join ", " left join ");
        append(sql, wheres, " where ", " and ");
        append(sql, groupBys, " group by ", ", ");
        append(sql, havings, " having ", " and ");
        append(sql, orderBys, " order by ", ", ");

        if (_forUpdate) {
            sql += " for update";
            if (_noWait) {
                sql += " nowait";
            }
        }
        if (_limit > 0)
            sql += " limit ?";
        if (_offset > 0)
            sql += ",?";
        return sql;
    }
};

#endif //MAPPER_SELECTBUILDER_HPP
