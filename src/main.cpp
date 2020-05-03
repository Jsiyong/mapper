//
// Created by Jsiyong on 2020-05-02.
//


#include <sql/MySql.hpp>

int main() {

    Connection *connection = new MySql;
    connection->setOption(DatabaseOption());
    connection->connect();
//    bool isConnect = connection->ping();
//    mySql->query();
    std::string sql = "select * from user where `id` >= ?";
    connection->prepare(sql);
    connection->bindValue(0, 1);
    connection->execute();
    while (connection->next()) {
        std::cout << "id = " << boost::any_cast<int>(connection->value(0));
        std::cout << " name = " << boost::any_cast<std::string>(connection->value(1)) << std::endl;
    }

    return 0;
}