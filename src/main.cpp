//
// Created by Jsiyong on 2020-05-02.
//


#include <sql/Connection.hpp>
#include <sql/TypeMapping.hpp>
#include <sql/PrepareBinder.hpp>
#include <sql/ConnectionPool.hpp>
#include <entity/User.hpp>
#include <sql/builder/SelectBuilder.hpp>

int main() {
    SelectBuilder selectBuilder;
    auto sql = selectBuilder.select("*")
            .from("user")
            .leftJoin("person on a=b")
            .where("id1=?")
            .where("id2=?")
            .where("name=?")
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