//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_MYSQL_HPP
#define MAPPER_MYSQL_HPP

#include "Connection.hpp"
#include <ResultSet.hpp>
#include <Row.hpp>
#include <boost/log/trivial.hpp>
#include <mysql.h>
#include <memory>
#include <vector>
#include <iostream>
#include <list>

/**
 * mysql数据库连接
 */
class MySql : public Connection {
public:
    void setOption(const DatabaseOption &option) override {
        this->option = option;
    }

    void close() override {
        if (connection != nullptr) {
            mysql_close(connection);
            connection = nullptr;
        }
        if (!mysqlStmt)
            mysql_stmt_free_result(mysqlStmt);
    }

    bool ping() override {
        return mysql_ping(connection) == 0;
    }

    int getLastAffectedRows() override {
        return (int) mysql_affected_rows(connection);
    }

    bool connect() override {
        //初始化mysql
        connection = mysql_init(nullptr);
        if (nullptr == connection) {
            setLastError("mysql init failed");
            return false;
        }
        //设置超时时间
        if (option.connectTimeout > 0) {
            if (0 != mysql_options(connection, MYSQL_OPT_CONNECT_TIMEOUT, &option.connectTimeout)) {
                setLastError("set option error");
                return false;
            }
        }
        //编码
        mysql_options(connection, MYSQL_SET_CHARSET_NAME, option.charsetName.c_str());
        mysql_options(connection, MYSQL_OPT_RECONNECT, &option.reconnectTime);
        if (!mysql_real_connect(connection, option.host.c_str(), option.username.c_str(), option.password.c_str(),
                                option.databaseName.c_str(), option.port, nullptr, 0)) {
            setLastError("failed to connect to database");
            return false;
        }
        return true;
    }


    void setLastError(const std::string &lastError) {
        this->lastError = "[" + lastError + "] " + mysql_error(connection);
        BOOST_LOG_TRIVIAL(error) << this->lastError;
    }

    std::string getLastError() const {
        return this->lastError;
    }

    bool begin() override {
        if (mysql_query(connection, "BEGIN")) {
            setLastError("begin error");
            return false;
        }
        return true;
    }

    bool commit() override {
        if (mysql_query(connection, "COMMIT")) {
            setLastError("commit error");
            return false;
        }
        return true;
    }

    bool rollback() override {
        if (mysql_query(connection, "ROLLBACK")) {
            setLastError("rollback error");
            return false;
        }
        return true;
    }

    bool execute() override {

        //绑定查询参数
        if (mysql_stmt_bind_param(mysqlStmt, &paramBinds[0])) {
            setLastError("mysql_stmt_bind_param error");
            return false;
        }

        //获取结果集元数据
        mysqlRes = mysql_stmt_result_metadata(mysqlStmt);

        std::vector<std::vector<char>> result;
        std::vector<MYSQL_BIND> resultBinds;
        auto columnNum = mysql_num_fields(mysqlRes);
        resultBinds.resize(columnNum);
//        resultRow.resize(columnNum);//查询结果行
        strBuf.resize(64);
//        std::vector<bool> isNull(columnNum, false);
//        std::vector<bool> error(columnNum, false);
//        std::vector<int> length(columnNum, 0);
        //根据表元数据绑定结果集
        for (int i = 0; i < mysqlRes->field_count; i++) {
            if (mysqlRes->fields[i].type == MYSQL_TYPE_LONG) {
                resultBinds[i].buffer_type = MYSQL_TYPE_LONG;
                resultBinds[i].buffer = &intBuf;
//                resultBinds[i].is_null = (bool *) isNull[i];
//                resultBinds[i].error = &error[i];
            }
            if (mysqlRes->fields[i].type == MYSQL_TYPE_VAR_STRING) {
                resultBinds[i].buffer_type = MYSQL_TYPE_STRING;
                resultBinds[i].buffer = strBuf.data();
                resultBinds[i].buffer_length = 64;
            }
            std::cout << "name:" << mysqlRes->fields[i].name << " type: " << mysqlRes->fields[i].type << std::endl;
        }
        //绑定查询结果
        if (mysql_stmt_bind_result(mysqlStmt, &resultBinds[0])) {
            setLastError("mysql_stmt_bind_result");
            return false;
        }

        if (mysql_stmt_execute(mysqlStmt)) {
            setLastError("mysql_stmt_execute error");
            return false;
        }
        return true;
    }


    void bindValue(int pos, const boost::any &value) override {
        if (value.type() == typeid(int)) {
            int i = boost::any_cast<int>(value);
            paramBinds[pos].buffer_type = MYSQL_TYPE_LONG;
            paramBinds[pos].buffer = &i;
        }
    }

    bool prepare(const std::string &query) override {
        //初始化预处理环境
        mysqlStmt = mysql_stmt_init(connection);
        if (!mysqlStmt) {
            setLastError("mysql_stmt_init error");
            return false;
        }
        if (mysql_stmt_prepare(mysqlStmt, query.c_str(), query.size())) {
            setLastError("mysql_stmt_prepare error");
            return false;
        }
        //获取预处理绑定的参数个数
        auto paramCount = mysql_stmt_param_count(mysqlStmt);
        paramBinds.resize(paramCount);//初始化绑定参数列表
        return true;
    }

    bool next() {
        //0表示正确,其他情况返回错误
        if (mysql_stmt_fetch(mysqlStmt) == 0) {
            resultRow.clear();
            //保存一次结果行
            for (int i = 0; i < mysqlRes->field_count; i++) {
                if (mysqlRes->fields[i].type == MYSQL_TYPE_LONG) {
                    resultRow.emplace_back(intBuf);
                }
                if (mysqlRes->fields[i].type == MYSQL_TYPE_VAR_STRING) {
                    resultRow.emplace_back(std::string(strBuf.data()));
                }
            }
            return true;
        }
        return false;
    }

    boost::any value(int index) override {
        return resultRow[index];
    }

private:
    MYSQL *connection = nullptr;
    MYSQL_STMT *mysqlStmt = nullptr;
    MYSQL_RES *mysqlRes = nullptr;//结果

    std::string lastError;//最后的错误信息
    DatabaseOption option;//数据库配置信息
    std::vector<MYSQL_BIND> paramBinds;//预处理的参数绑定
    std::vector<boost::any> resultRow;//一次查询的结果行

    int intBuf = 0;
    std::vector<char> strBuf;
};

#endif //MAPPER_MYSQL_HPP
