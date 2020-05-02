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
                setLastError(mysql_error(connection));
                return false;
            }
        }
        //编码
        mysql_options(connection, MYSQL_SET_CHARSET_NAME, option.charsetName.c_str());
        mysql_options(connection, MYSQL_OPT_RECONNECT, &option.reconnectTime);
        if (!mysql_real_connect(connection, option.host.c_str(), option.username.c_str(), option.password.c_str(),
                                option.databaseName.c_str(), option.port, nullptr, 0)) {
            setLastError(std::string("failed to connect to database: Error: ") + mysql_error(connection));
            return false;
        }
        return true;
    }


    void setLastError(const std::string &lastError) {
        this->lastError = lastError;
        BOOST_LOG_TRIVIAL(error) << lastError;
    }

    std::string getLastError() const {
        return this->lastError;
    }

    bool begin() override {
        if (mysql_query(connection, "BEGIN")) {
            setLastError(std::string("begin error:") + mysql_error(connection));
            return false;
        }
        return true;
    }

    bool commit() override {
        if (mysql_query(connection, "COMMIT")) {
            setLastError(std::string("commit error:") + mysql_error(connection));
            return false;
        }
        return true;
    }

    bool rollback() override {
        if (mysql_query(connection, "ROLLBACK")) {
            setLastError(std::string("rollback error:") + mysql_error(connection));
            return false;
        }
        return true;
    }

    void query() {
        fprintf(stderr, "Successfully connected to Database.\n");
        auto stmt = mysql_stmt_init(&conn);
        std::string sql = "select * from user where `id` >= ?";
        mysql_stmt_prepare(stmt, sql.c_str(), sql.size());

        auto m_param_count = mysql_stmt_param_count(stmt);
        std::vector<MYSQL_BIND> param_binds;
        int id = 1;
        std::vector<char> name(65535, 0);
        param_binds.resize(m_param_count + 1);

        param_binds[0].buffer_type = MYSQL_TYPE_LONG;
        param_binds[0].buffer = &id;

        param_binds[1].buffer_type = MYSQL_TYPE_STRING;
        param_binds[1].buffer = name.data();
        param_binds[1].buffer_length = 65536;

        int iRet = mysql_stmt_bind_param(stmt, &param_binds[0]);
        mysql_stmt_bind_result(stmt, &param_binds[0]);
        if (mysql_stmt_execute(stmt)) {
            std::cout << "sorry!" << std::endl;
        } else {
            if (mysql_stmt_store_result(stmt)) {
                std::cout << "store result stmt err!" << mysql_error(&conn) << std::endl;
            } else {
                printf("----------------------------table data--------------------------------\n");
                while (0 == mysql_stmt_fetch(stmt)) {
                    std::cout << "id = " << id;
                    std::cout << "  name = " << name.data() << std::endl;
                }
            }
            mysql_stmt_free_result(stmt);
        }

//        auto r = getResultSet();
//        if (!r) {
//            printf("query error\n");
//        }
//        printf("query result:\n");
////        mysql_stmt_store_result(stmt); //将所有结果缓冲到客户端
//        while (Row row = r.next()) {
//            auto id = row.toInt(0);
//            auto name = row.toString(1);
//            printf("id:%d, name: %s\n", id, name.c_str());
//        }
//        int i = 0;
    }

private:
    ResultSet getResultSet() {
        MYSQL_RES *result = mysql_use_result(connection);
        if (!result) {
            return ResultSet{};
        }
        return ResultSet{result};
    }

private:
    MYSQL *connection = nullptr;
    MYSQL_STMT *mysqlStmt = nullptr;
    std::string lastError;//最后的错误信息
    DatabaseOption option;//数据库配置信息

};

#endif //MAPPER_MYSQL_HPP
