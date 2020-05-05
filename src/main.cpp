//
// Created by Jsiyong on 2020-05-02.
//


#include <sql/Connection.hpp>
#include <sql/TypeMapping.hpp>
#include <sql/PrepareBinder.hpp>

int main() {

    std::vector<int> a = {1, 2, 3, 4};
    a.clear();

    Connection connection;
    connection.setOption(DatabaseOption());
    connection.connect();
//    bool isConnect = connection->ping();
//    mySql->query();
    std::string sql = "select * from user where `id` > ?";
    connection.prepare(sql);
    connection.bindValue(0, 1);
    connection.execute();
    while (connection.next()) {
        std::cout << "id = " << connection.getIntValue(0);
        std::cout << " name = " << connection.getStringValue(1) << std::endl;
    }

    return 0;
}