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
#include <time.h>
#include <chrono>


int main() {

    {
        for (int i = 0; i < 10; i++) {
            std::cout << "===================== dept ============================================" << std::endl;
            Example<Dept> example;
            auto item1 = example.createCriteria();
            item1->andGreaterThanOrEqualTo(&Dept::deptId, 1);
            Mapper<Dept> userMapper;
            auto depts = userMapper.selectByExample(example);
            for (auto &d:depts) {
                std::cout << d << std::endl;
            }
        }

//        Example<Dept> example2;
//        auto item2 = example2.createCriteria();
//        item2->andEqualTo(&Dept::deptId, 1);
//        std::cout << "selectOneByExample " << userMapper.selectOneByExample(example) << std::endl;
//        std::cout << "selectOneByExample " << userMapper.selectCountByExample(example) << std::endl;
//        std::cout << "updateByExample " << userMapper.updateByExample(Dept(1, "deptA1"), example2) << std::endl;
//        std::cout << "updateByExampleSelective " << userMapper.updateByExampleSelective(Dept(1, ""), example2)
//                  << std::endl;
//        std::cout << "deleteByExample " << userMapper.deleteByExample(example2) << std::endl;
//
//
//        std::cout << "insert " << userMapper.insert(Dept(0, "deptInsertTest")) << std::endl;
//        std::cout << "deleteBy " << userMapper.deleteBy(Dept(7, "deptInsertTest")) << std::endl;
//        std::cout << "deleteByPrimaryKey " << userMapper.deleteByPrimaryKey(8) << std::endl;
//
//        std::cout << "selectAll " << userMapper.selectAll().size() << std::endl;
    }

    {
        std::cout << "===================== user ============================================" << std::endl;
        Example<User> example;
//        auto item1 = example.createCriteria();
////        item1->andIn(&User::id, std::set<int>{2, 1, 3});
//        example.orderByAsc(&User::name);
//        example.orderByDesc(&Team::teamId);
        Mapper<User> userMapper;
        auto users = userMapper.selectAll();
        for (auto &u:users) {
            std::cout << u << std::endl;
        }
        User user;

        auto ii = &Team::teamId;

        auto b = user.team.*ii;
        user.name = "zhaoqing";
        user.createTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        user.team.teamId = 10;
        std::cout << "insert user " << userMapper.insert(user) << std::endl;

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