//
// Created by Jsiyong on 2020-05-02.
//


#include <sql/Connection.hpp>
#include <sql/PrepareBinder.hpp>
#include <sql/ConnectionPool.hpp>
#include <entity/User.hpp>
#include <builder/SQLBuilder.hpp>
#include <util/StringUtils.hpp>
#include <util/TypeUtils.hpp>
#include <entity/Example.hpp>
#include <util/AliasHelper.hpp>
#include <common/Mapper.hpp>
#include <cstring>

class ABC {
public:
    void A() {
        ABC abc;
        return;
    }
};

int main() {
    ABC abc;
    abc.A();
    Dept dept;
    auto de = &Dept::teams;
    auto ii = &(&dept->*de);
    EntityWrapper<std::vector<Team>>().getReflectionInfo(ii);

    {

        Example<Dept> example;

        auto item1 = example.createCriteria();
        item1->andEqualTo(&Dept::deptId, 1);
        Mapper<Dept> userMapper;
        auto depts = userMapper.selectByExample(example);
        for (auto &d:depts) {
            std::cout << *d << std::endl;
        }
    }

    {
        Example<User> example;
        auto item1 = example.createCriteria();
        item1->andIn(&User::id, std::vector<int>({2, 1, 3}));
        item1->andEqualTo(&Team::teamName, "teamA");
        example.orderByAsc(&User::name);
        example.orderByDesc(&Team::teamId);
        Mapper<User> userMapper;
        auto users = userMapper.selectByExample(example);
        for (auto &u:users) {
            std::cout << *u << std::endl;
        }
    }
    getchar();


//    auto name = StringUtils::camelhump2Underline("userName");
//
//
//    auto sql1 = std::make_shared<SQLBuilder>()
//            ->SELECT("P.ID, P.USERNAME, P.PASSWORD, P.FULL_NAME")
//            ->SELECT("P.LAST_NAME, P.CREATED_ON, P.UPDATED_ON")
//            ->FROM("PERSON P")
//            ->FROM("ACCOUNT A")
//            ->INNER_JOIN("DEPARTMENT D on D.ID = P.DEPARTMENT_ID")
//            ->INNER_JOIN("COMPANY C on D.COMPANY_ID = C.ID")
//            ->WHERE("P.ID = A.ID")
//            ->WHERE("P.FIRST_NAME like ?")
//            ->OR()
//            ->WHERE("P.LAST_NAME like ?")
//            ->GROUP_BY("P.ID")
//            ->HAVING("P.LAST_NAME like ?")
//            ->OR()
//            ->HAVING("P.FIRST_NAME like ?")
//            ->ORDER_BY("P.ID")
//            ->ORDER_BY("P.FULL_NAME")
//            ->LIMIT("?", "?")
//            ->toString();
//
//    auto sql2 = std::make_shared<SQLBuilder>()
//            ->INSERT_INTO("PERSON")
//            ->VALUES("ID, FIRST_NAME", "?, ?")
//            ->VALUES("LAST_NAME", "?")
//            ->toString();
//
//    auto sql3 = std::make_shared<SQLBuilder>()
//            ->INSERT_INTO("TABLE_A")
//            ->INTO_COLUMNS("a,b,c,d")
//            ->INTO_VALUES("1,2,3,4")
//            ->ADD_ROW()
//            ->INTO_VALUES("5,6")
//            ->INTO_VALUES("7,8")
//            ->toString();
//
//    auto sql4 = std::make_shared<SQLBuilder>()
//            ->DELETE_FROM("PERSON")
//            ->WHERE("id=?")
//            ->LIMIT("20")
//            ->toString();
//    auto sql5 = std::make_shared<SQLBuilder>()
//            ->UPDATE("PERSON")
//            ->SET("name=?")
//            ->WHERE("id=10")
//            ->toString();
//
//    EntityWrapper<User> user;
//    auto usr = std::make_shared<User>();
//    auto aaa = user.getReflectionInfo(usr.get());
//
//    ConnectionPool::getInstance()->initInternal();
//    {
//        ConnectionPool::getInstance()->getConnection();
//        ConnectionPool::getInstance()->getConnection();
//        ConnectionPool::getInstance()->getConnection();
//        ConnectionPool::getInstance()->getConnection();
//        auto connection = ConnectionPool::getInstance()->getConnection();
//        connection->connect();
//        std::string sql = "select name,id from user where id > ? and name like ?";
//        connection->prepare(sql);
//        connection->bindValue(0, 1);
//        connection->bindValue(1, "%i%");
//        connection->execute();
//        while (connection->next()) {
//            std::cout << "id = " << connection->value(1).getValue<int>();
//            std::cout << " name = " << connection->value(0).getValue<std::string>() << std::endl;
//        }
//        ConnectionPool::getInstance()->releaseConnection(connection);
//    }


    return 0;
}