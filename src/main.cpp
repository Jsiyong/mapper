//
// Created by Jsiyong on 2020-05-02.
//


#include <sql/mysql/MySQL.hpp>
#include <sql/mysql/TypeMapping.hpp>

int main() {

    Connection *connection = new MySQL;
    connection->setOption(DatabaseOption());
    connection->connect();
//    bool isConnect = connection->ping();
//    mySql->query();
    std::string sql = "select * from user where `id` >= ?";
    connection->prepare(sql);
    int i = 1;
    connection->bindValue(0, i);
    connection->execute();
    while (connection->next()) {
        std::cout << "id = " << boost::any_cast<int>(connection->value(0));
        std::cout << " name = " << boost::any_cast<std::string>(connection->value(1)) << std::endl;
    }

    return 0;
}