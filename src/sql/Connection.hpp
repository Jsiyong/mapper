//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_CONNECTION_HPP
#define MAPPER_CONNECTION_HPP


#include "DatabaseOption.hpp"
#include "PrepareBinder.hpp"
#include "ResultBinder.hpp"
#include <mysql.h>
#include <memory>
#include <vector>
#include <iostream>
#include <list>
#include <chrono>

/**
 * mysql数据库连接
 */
class Connection {
private:
    /**
     * 清除资源内容
     */
    void clear() {
        if (!mysqlRes)
            mysql_free_result(mysqlRes);
        if (!mysqlStmt)
            mysql_stmt_free_result(mysqlStmt);
        records.clear();
        prepareBinder.reset();
        resultBinder.reset();
    }

public:
    /**
    * 数据库连接的初始化配置
    * @param databaseOption
    */
    void setOption(const DatabaseOption &option) {
        this->option = option;
    }

    /**
     * 关闭连接
     */
    void close() {
        if (connection != nullptr) {
            mysql_close(connection);
            connection = nullptr;
        }
    }

    /**
     * 测试数据库连接是否能联通
     * @return
     */
    bool ping() {
        return mysql_ping(connection) == 0;
    }

    /**
     * 获取最后受影响的行数
     * @return
     */
    int getLastAffectedRows() {
        return (int) mysql_affected_rows(connection);
    }

    /**
     * 返回最后插入的数据库id
     * @return
     */
    int getLastInsertId() {
        return (int) mysql_insert_id(connection);
    }

    /**
     * 连接数据库
     * @return
     */
    bool connect() {
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
        std::cerr << this->lastError << std::endl;
    }

    std::string getLastError() const {
        return this->lastError;
    }

    /**
     * 开启事务
     * @return
     */
    bool begin() {
        if (mysql_query(connection, "BEGIN")) {
            setLastError("begin error");
            return false;
        }
        return true;
    }

    /**
     * 提交事务
     * @return
     */
    bool commit() {
        if (mysql_query(connection, "COMMIT")) {
            setLastError("commit error");
            return false;
        }
        return true;
    }

    /**
     * 回滚事务
     * @return
     */
    bool rollback() {
        if (mysql_query(connection, "ROLLBACK")) {
            setLastError("rollback error");
            return false;
        }
        return true;
    }

    /**
     * 执行数据库查询
     * @return
     */
    bool execute() {

        //绑定查询参数
        if (mysql_stmt_bind_param(mysqlStmt, &prepareBinder->getParamBinds()[0])) {
            setLastError("mysql_stmt_bind_param error");
            return false;
        }

        //获取结果集元数据
        mysqlRes = mysql_stmt_result_metadata(mysqlStmt);
        //若有查询结果,则绑定查询结果集,否则直接执行查询语句
        //插入/更新/删除语句没有查询结果集
        if (mysqlRes) {
            auto columnNum = mysql_num_fields(mysqlRes);
            resultBinder = std::make_shared<ResultBinder>(columnNum);
            //根据表元数据绑定结果集
            for (int i = 0; i < mysqlRes->field_count; i++) {
                resultBinder->bindValue(mysqlRes->fields[i].type, i);
                this->records.emplace_back(mysqlRes->fields[i].name);
//            std::cout << "name:" << mysqlRes->fields[i].name << " type: " << mysqlRes->fields[i].type << std::endl;
            }
            //绑定查询结果
            if (mysql_stmt_bind_result(mysqlStmt, &resultBinder->getBindResult()[0])) {
                setLastError("mysql_stmt_bind_result");
                return false;
            }
        }

        if (mysql_stmt_execute(mysqlStmt)) {
            setLastError("mysql_stmt_execute error");
            return false;
        }
        return true;
    }


    /**
     * 给placeholder相对应的位置绑定参数
     * @param pos 位置
     * @param value 值
     */
//    template<typename T>
    void bindValue(int pos, const Object &value) {
        prepareBinder->bindValue(pos, value);
    }

    /**
     * 预处理
     * @param query
     * @return
     */
    bool prepare(const std::string &query) {
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
        prepareBinder = std::make_shared<PrepareBinder>(paramCount);
        return true;
    }

    /**
     * 获取下一条记录
     * @return
     */
    bool next() {
        //0表示正确,其他情况返回错误
        if (mysql_stmt_fetch(mysqlStmt) != 0) {
            //若没有下一个了,就清除资源
            clear();
            return false;
        }
        return true;
    }

    /**
    * 提取结果集中的结果
    * @param index
    * @return
    */
    Object value(int index) {
        return resultBinder->value(index);
    }

    const std::vector<std::string> &getRecords() const {
        return records;
    }

private:
    MYSQL *connection = nullptr;
    MYSQL_STMT *mysqlStmt = nullptr;
    MYSQL_RES *mysqlRes = nullptr;//结果

    std::string lastError;//最后的错误信息
    DatabaseOption option;//数据库配置信息
    std::vector<std::string> records;//查询的列的名称

    std::shared_ptr<PrepareBinder> prepareBinder = nullptr;
    std::shared_ptr<ResultBinder> resultBinder = nullptr;


};

#endif //MAPPER_CONNECTION_HPP
