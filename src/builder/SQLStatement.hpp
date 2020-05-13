//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_SQLSTATEMENT_HPP
#define MAPPER_SQLSTATEMENT_HPP

#include <vector>
#include <string>
#include <memory>

/**
 * 封装SQL句
 */
class SQLStatement {
    friend class SQLBuilder;

private:
    const std::string AND = ") AND (";
    const std::string OR = ") OR (";

    /**
     * 语句类型:增删改查
     */
    enum class StatementType {
        DELETE, INSERT, SELECT, UPDATE
    } statementType;

    std::vector<std::string> sets;
    std::vector<std::string> select;
    std::vector<std::string> tables;
    std::vector<std::string> join;
    std::vector<std::string> innerJoin;
    std::vector<std::string> outerJoin;
    std::vector<std::string> leftOuterJoin;
    std::vector<std::string> rightOuterJoin;
    std::vector<std::string> where;
    std::vector<std::string> having;
    std::vector<std::string> groupBy;
    std::vector<std::string> orderBy;
    std::vector<std::string> *lastList = nullptr;
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> valuesList;//插入
    bool distinct = false;
    std::string offset;
    std::string limit;
public:
    SQLStatement() {
        //初始化,插入值有一行
        valuesList.resize(1);
    }

private:
    void sqlClause(std::string &builder, const std::string &keyword, const std::vector<std::string> &parts,
                   const std::string &open, const std::string &close, const std::string &conjunction) {
        if (parts.empty()) {
            return;
        }
        if (!builder.empty()) {
            builder.append(" ");
        }
        builder.append(keyword);
        builder.append(" ");
        builder.append(open);
        std::string last = "";
        for (int i = 0, n = parts.size(); i < n; i++) {
            std::string part = parts.at(i);
            if (i > 0 && part != AND && part != OR && last != AND && last != OR) {
                builder.append(conjunction);
            }
            builder.append(part);
            last = part;
        }
        builder.append(close);
    }

    void deleteSQL(std::string &builder) {
        sqlClause(builder, "DELETE FROM", tables, "", "", "");
        sqlClause(builder, "WHERE", where, "(", ")", " AND ");
        appendLimitClause(builder, "", limit);
    }

    void updateSQL(std::string &builder) {
        sqlClause(builder, "UPDATE", tables, "", "", "");
        joins(builder);
        sqlClause(builder, "SET", sets, "", "", ", ");
        sqlClause(builder, "WHERE", where, "(", ")", " AND ");
        appendLimitClause(builder, "", limit);
    }

    void insertSQL(std::string &builder) {
        sqlClause(builder, "INSERT INTO", tables, "", "", "");
        sqlClause(builder, "", columns, "(", ")", ", ");
        for (int i = 0; i < valuesList.size(); i++) {
            sqlClause(builder, i > 0 ? "," : "VALUES", valuesList.at(i), "(", ")", ", ");
        }
    }

    void joins(std::string &builder) {
        sqlClause(builder, "JOIN", join, "", "", " JOIN ");
        sqlClause(builder, "INNER JOIN", innerJoin, "", "", " INNER JOIN ");
        sqlClause(builder, "OUTER JOIN", outerJoin, "", "", " OUTER JOIN ");
        sqlClause(builder, "LEFT OUTER JOIN", leftOuterJoin, "", "", " LEFT OUTER JOIN ");
        sqlClause(builder, "RIGHT OUTER JOIN", rightOuterJoin, "", "", " RIGHT OUTER JOIN ");
    }

    void appendLimitClause(std::string &builder, const std::string &offset, const std::string &limit) {
        if (!limit.empty()) {
            builder.append(" LIMIT ").append(limit);
        }
        if (!offset.empty()) {
            builder.append(" OFFSET ").append(offset);
        }
    }

    void selectSQL(std::string &builder) {
        if (distinct) {
            sqlClause(builder, "SELECT DISTINCT", select, "", "", ", ");
        } else {
            sqlClause(builder, "SELECT", select, "", "", ", ");
        }

        sqlClause(builder, "FROM", tables, "", "", ", ");
        joins(builder);
        sqlClause(builder, "WHERE", where, "(", ")", " AND ");
        sqlClause(builder, "GROUP BY", groupBy, "", "", ", ");
        sqlClause(builder, "HAVING", having, "(", ")", " AND ");
        sqlClause(builder, "ORDER BY", orderBy, "", "", ", ");
        appendLimitClause(builder, offset, limit);
    }

private:
    std::string sql() {
        std::string answer;
        switch (statementType) {
            case StatementType::DELETE:
                deleteSQL(answer);
                break;
            case StatementType::INSERT:
                insertSQL(answer);
                break;
            case StatementType::SELECT:
                selectSQL(answer);
                break;
            case StatementType::UPDATE:
                updateSQL(answer);
                break;
            default:
                answer = "";
        }
        return answer;
    }

};

#endif //MAPPER_SQLSTATEMENT_HPP
