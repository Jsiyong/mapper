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

int main() {


    std::vector<char> ta = {'a', 'b', 'c'};
    std::vector<char> tb = {'a', 'b', 'c'};
    const char *abc = "nihao ";
    std::string cdf = "nihaoya ";
    int iiiii = strlen(abc);
    ta.assign(cdf.begin(), cdf.end());
//    tb.assign(abc, 0);
    tb.resize(strlen(abc));
    std::memcpy(tb.data(), abc, strlen(abc));
//    std::copy(abc, abc + strlen(abc) + 10, tb.data());

    auto u1 = AliasHelper::getAliasFromType<User>();
    auto u2 = AliasHelper::getAliasFromType<User>();
    auto u3 = AliasHelper::getAliasFromType<Team>();
    auto u4 = AliasHelper::getAliasFromType<User>();
    auto u5 = AliasHelper::getAliasFromType<Team>();
    {
//    auto name_ = EntityHelper::getProperty(&User::name);
//    auto id_ = EntityHelper::getProperty(&User::id);


//    auto b = &User::id;
//    auto a = &User::team;
        Example<User> example;
        auto item1 = example.createCriteria();
//    item1->andGreaterThan(&User::id, 1);
//    item1->andLike(&User::name, "%i%");
        item1->andIn(&User::id, std::vector<int>({2, 1, 3}));
//    item1->andEqualTo(&User::id, 2);
//    item1->orIsNull(&Team::teamId);
        item1->andEqualTo(&Team::teamName, "teamA");
//    item1->andGreaterThanOrEqualTo(&Team::teamId, 2);
//    auto item2 = example.createCriteria();
//    item2->andBetween(&User::name, "12", "456");
//    item2->orGreaterThan(&User::name, "23llllll");
//    example.orCriteria(item2);
        example.orderByAsc(&User::name);
        example.orderByDesc(&Team::teamId);
//    auto res = example.getSelectStatementByExample();
//    auto values = example.getPrepareValues();
        Mapper<User> userMapper;
        auto users = userMapper.selectByExample(example);
        std::cout << users.size() << std::endl;
        for (auto &u:users) {
            std::cout << *u << std::endl;
        }
    }
//
////    bool v1 = TypeUtils::isCollection(std::vector<int>());
////    bool v2 = TypeUtils::isCollection(std::set<int>());
////    bool v3 = TypeUtils::isCollection(std::string());
////    bool v4 = TypeUtils::isCollection(std::list<int>());
////    bool v5 = TypeUtils::isCollection(1);
//
//    std::string a1 = "123";
//    void *_a1 = (void *) &a1;
//    std::string a22 = *(std::string *) _a1;
//
////    std::shared_ptr<User> u=std::make_shared<User>();
////    auto iii2 = &User::id;
////    auto a1111 = u.get()->*iii2;
////    int a = 10;
////    {
////        std::shared_ptr<int> e1 = std::weak_ptr<int>(&a1111);
////    }
//
//
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