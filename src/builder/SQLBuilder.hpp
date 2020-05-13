//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_SQLBUILDER_HPP
#define MAPPER_SQLBUILDER_HPP

#include <string>
#include "SQLStatement.hpp"

/**
 * sql语句构建器
 */
class SQLBuilder {
private:
    SQLStatement sql;

public:
    SQLBuilder *UPDATE(const std::string &table) {
        sql.statementType = SQLStatement::StatementType::UPDATE;
        sql.tables.emplace_back(table);
        return this;
    }

    SQLBuilder *SET(const std::string &sets) {
        sql.sets.emplace_back(sets);
        return this;
    }

    SQLBuilder *INSERT_INTO(const std::string &tableName) {
        sql.statementType = SQLStatement::StatementType::INSERT;
        sql.tables.emplace_back(tableName);
        return this;
    }

    SQLBuilder *VALUES(const std::string &columns, const std::string &values) {
        INTO_COLUMNS(columns);
        INTO_VALUES(values);
        return this;
    }

    SQLBuilder *INTO_COLUMNS(const std::string &columns) {
        sql.columns.emplace_back(columns);
        return this;
    }

    SQLBuilder *INTO_VALUES(const std::string &values) {
        sql.valuesList.back().emplace_back(values);
        return this;
    }

    SQLBuilder *SELECT(const std::string &columns) {
        sql.statementType = SQLStatement::StatementType::SELECT;
        sql.select.emplace_back(columns);
        return this;
    }

    SQLBuilder *SELECT_DISTINCT(const std::string &columns) {
        sql.distinct = true;
        return SELECT(columns);
    }

    SQLBuilder *DELETE_FROM(const std::string &table) {
        sql.statementType = SQLStatement::StatementType::DELETE;
        sql.tables.emplace_back(table);
        return this;
    }

    SQLBuilder *FROM(const std::string &table) {
        sql.tables.emplace_back(table);
        return this;
    }

    SQLBuilder *JOIN(const std::string &join) {
        sql.join.emplace_back(join);
        return this;
    }

    SQLBuilder *INNER_JOIN(const std::string &join) {
        sql.innerJoin.emplace_back(join);
        return this;
    }

    SQLBuilder *LEFT_OUTER_JOIN(const std::string &join) {
        sql.leftOuterJoin.emplace_back(join);
        return this;
    }

    SQLBuilder *RIGHT_OUTER_JOIN(const std::string &join) {
        sql.rightOuterJoin.emplace_back(join);
        return this;
    }

    SQLBuilder *OUTER_JOIN(const std::string &join) {
        sql.outerJoin.emplace_back(join);
        return this;
    }

    SQLBuilder *WHERE(const std::string &conditions) {
        sql.where.emplace_back(conditions);
        sql.lastList = &sql.where;
        return this;
    }

    SQLBuilder *OR() {
        sql.lastList->emplace_back(sql.OR);
        return this;
    }

    SQLBuilder *AND() {
        sql.lastList->emplace_back(sql.AND);
        return this;
    }

    SQLBuilder *GROUP_BY(const std::string &columns) {
        sql.groupBy.emplace_back(columns);
        return this;
    }

    SQLBuilder *HAVING(const std::string &conditions) {
        sql.having.emplace_back(conditions);
        sql.lastList = &sql.having;
        return this;
    }

    SQLBuilder *ORDER_BY(const std::string &columns) {
        sql.orderBy.emplace_back(columns);
        return this;
    }

    SQLBuilder *LIMIT(const std::string &variable, const std::string &offset) {
        sql.limit = variable;
        sql.offset = offset;
        return this;
    }

    SQLBuilder *LIMIT(const std::string &variable) {
        sql.limit = variable;
        sql.offset = std::to_string(0);
        return this;
    }

    /**
     * 批量插入时,多插入一行
     * @return
     */
    SQLBuilder *ADD_ROW() {
        sql.valuesList.resize(sql.valuesList.size() + 1);
        return this;
    }

    std::string toString() {
        return sql.sql();
    }

};

#endif //MAPPER_SQLBUILDER_HPP
