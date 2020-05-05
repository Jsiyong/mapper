//
// Created by Jsiyong on 2020-05-02.
//

#ifndef MAPPER_CONNECTION_HPP
#define MAPPER_CONNECTION_HPP


#include "TypeMapping.hpp"
#include "DatabaseOption.hpp"
#include "PrepareBinder.hpp"
#include "ResultBinder.hpp"
#include <ResultSet.hpp>
#include <Row.hpp>
#include <mysql.h>
#include <memory>
#include <vector>
#include <iostream>
#include <list>

/**
 * mysql数据库连接
 */
class Connection {
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
        if (!mysqlStmt)
            mysql_stmt_free_result(mysqlStmt);
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
        if (mysql_stmt_bind_param(mysqlStmt, &paramBinds[0])) {
            setLastError("mysql_stmt_bind_param error");
            return false;
        }

        //获取结果集元数据
        mysqlRes = mysql_stmt_result_metadata(mysqlStmt);

//        std::vector<std::vector<char>> result;
//        std::vector<MYSQL_BIND> resultBinds;
        auto columnNum = mysql_num_fields(mysqlRes);
//        resultBinds.resize(columnNum);
//        resultRow.resize(columnNum);//查询结果行
//        strBuf.resize(64);
        resultBinder = std::make_shared<ResultBinder>(columnNum);
//        std::vector<bool> isNull(columnNum, false);
//        std::vector<bool> error(columnNum, false);
//        std::vector<int> length(columnNum, 0);
        //根据表元数据绑定结果集
        for (int i = 0; i < mysqlRes->field_count; i++) {
            resultBinder->bindValue(mysqlRes->fields[i].type, i);
        }
        //......
//            std::cout << "name:" << mysqlRes->fields[i].name << " type: " << mysqlRes->fields[i].type << std::endl;
//        }
        //绑定查询结果
        if (mysql_stmt_bind_result(mysqlStmt, &resultBinder->getBindResult()[0])) {
            setLastError("mysql_stmt_bind_result");
            return false;
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
    template<typename T>
    void bindValue(int pos, const T &value) {
        if (pos >= paramBinds.size()) {
            setLastError("bind pos out of range");
            return;
        }
        PrepareBinder::bindValue(paramBinds[pos], value);
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
        paramBinds.resize(paramCount);//初始化绑定参数列表
        return true;
    }

    /**
     * 获取下一条记录
     * @return
     */
    bool next() {
        //0表示正确,其他情况返回错误
        return mysql_stmt_fetch(mysqlStmt) == 0;
    }

    /**
     * 提取结果集中的整型结果
     * @param index
     * @return
     */
    int getIntValue(int index) {
        return resultBinder->getIntValue(index);
    }

    /**
    * 提取结果集中的字符串结果
    * @param index
    * @return
    */
    std::string getStringValue(int index) {
        return resultBinder->getStringValue(index);
    }

private:
    MYSQL *connection = nullptr;
    MYSQL_STMT *mysqlStmt = nullptr;
    MYSQL_RES *mysqlRes = nullptr;//结果

    std::string lastError;//最后的错误信息
    DatabaseOption option;//数据库配置信息
    std::vector<MYSQL_BIND> paramBinds;//预处理的参数绑定

    std::shared_ptr<ResultBinder> resultBinder = nullptr;

};

#endif //MAPPER_CONNECTION_HPP
