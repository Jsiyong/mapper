//
// Created by Jsiyong on 2020-05-02.
//


#include <sql/Connection.hpp>
#include <sql/TypeMapping.hpp>
#include <sql/PrepareBinder.hpp>

int main() {

    Connection connection;
    connection.setOption(DatabaseOption());
    connection.connect();
    std::string sql = "select name,id from user where id > ? and name like ?";
    connection.prepare(sql);
    connection.bindValue(0, 1);
    connection.bindValue(1, "%i%");
    connection.execute();
    while (connection.next()) {
        std::cout << "id = " << connection.value<int>(1);
        std::cout << " name = " << connection.value<std::string>(0) << std::endl;
    }

    return 0;
}