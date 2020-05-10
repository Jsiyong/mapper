//
// Created by Jsiyong on 2020-05-02.
//


#include <sql/Connection.hpp>
#include <sql/TypeMapping.hpp>
#include <sql/PrepareBinder.hpp>
#include <sql/ConnectionPool.hpp>
#include <entity/User.hpp>
#include <sql/builder/SQLBuilder.hpp>

int main() {


    auto sql1 = SQLBuilder()
            .SELECT("P.ID, P.USERNAME, P.PASSWORD, P.FULL_NAME")
            .SELECT("P.LAST_NAME, P.CREATED_ON, P.UPDATED_ON")
            .FROM("PERSON P")
            .FROM("ACCOUNT A")
            .INNER_JOIN("DEPARTMENT D on D.ID = P.DEPARTMENT_ID")
            .INNER_JOIN("COMPANY C on D.COMPANY_ID = C.ID")
            .WHERE("P.ID = A.ID")
            .WHERE("P.FIRST_NAME like ?")
            .OR()
            .WHERE("P.LAST_NAME like ?")
            .GROUP_BY("P.ID")
            .HAVING("P.LAST_NAME like ?")
            .OR()
            .HAVING("P.FIRST_NAME like ?")
            .ORDER_BY("P.ID")
            .ORDER_BY("P.FULL_NAME")
            .LIMIT("?", "?")
            .toString();

    auto sql2 = SQLBuilder()
            .INSERT_INTO("PERSON")
            .VALUES("ID, FIRST_NAME", "?, ?")
            .VALUES("LAST_NAME", "?")
            .toString();

    auto sql3 = SQLBuilder()
            .INSERT_INTO("TABLE_A")
            .INTO_COLUMNS("a,b,c,d")
            .INTO_VALUES("1,2,3,4")
            .ADD_ROW()
            .INTO_VALUES("5,6")
            .INTO_VALUES("7,8")
            .toString();

    auto sql4 = SQLBuilder()
            .DELETE_FROM("PERSON")
            .WHERE("id=?")
            .LIMIT("20")
            .toString();
    auto sql5 = SQLBuilder()
            .UPDATE("PERSON")
            .SET("name=?")
            .WHERE("id=10")
            .toString();

    EntityWrapper<User> user;
    auto usr = std::make_shared<User>();
    auto aaa = user.getReflectionInfo(usr);

    ConnectionPool::getInstance()->initInternal();
    {
        ConnectionPool::getInstance()->getConnection();
        ConnectionPool::getInstance()->getConnection();
        ConnectionPool::getInstance()->getConnection();
        ConnectionPool::getInstance()->getConnection();
        auto connection = ConnectionPool::getInstance()->getConnection();
        connection->connect();
        std::string sql = "select name,id from user where id > ? and name like ?";
        connection->prepare(sql);
        connection->bindValue(0, 1);
        connection->bindValue(1, "%i%");
        connection->execute();
        while (connection->next()) {
            std::cout << "id = " << connection->value<int>(1);
            std::cout << " name = " << connection->value<std::string>(0) << std::endl;
        }
        ConnectionPool::getInstance()->releaseConnection(connection);
    }


    return 0;
}