//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_USER_HPP
#define MAPPER_USER_HPP

#include <string>
#include <memory>
#include <tuple>
#include "EntityWrapper.hpp"
#include "EntityColumn.hpp"
#include "EntityTable.hpp"

struct Team {
    int teamId = 0;
    std::string teamName = "123";

};

template<>
class EntityWrapper<Team> {
public:
    auto getReflectionInfo(std::shared_ptr<Team> entity) {
        return std::make_tuple(
                EntityTable(entity, "Team", "team"),
                std::make_pair(&Team::teamId,
                               EntityColumn(entity, &entity->teamId, "teamId", "Team_Id", ColumnType::Null, KeySql::Null,
                                            JoinType::Null)),
                std::make_pair(&Team::teamName,
                               EntityColumn(entity, &entity->teamName, "teamName", "teamName", ColumnType::Null,
                                            KeySql::Null, JoinType::Null))
        );
    }
};

struct User {
    int id = 0;
    std::string name = "";
    Team team;
};

template<>
class EntityWrapper<User> {
public:

    auto getReflectionInfo(std::shared_ptr<User> entity) {
        return std::make_tuple(
                EntityTable(entity, "User", "user"),
                std::make_pair(&User::id, EntityColumn(entity, &entity->id, "id", "id", ColumnType::Null, KeySql::Null,
                                                       JoinType::Null)),
                std::make_pair(&User::name,
                               EntityColumn(entity, &entity->name, "name", "name", ColumnType::Null, KeySql::Null,
                                            JoinType::Null)),
                //连表查询,表示用user表中的team_id去连接team表,默认是team表的主键id,也可以指定其他列
                std::make_pair(&User::team,
                               EntityColumn(entity, &entity->team, "team", "team_id1", ColumnType::Null, KeySql::Null,
                                            JoinType::LeftJoin, &Team::teamId))
        );
    }
};

//#define EntityResultMap(field)\
//std::make_pair(&_Clazz::field, EntityColumn(&entity->field, #field, #field))
//
//#define RRR(Clazz, ...)\
//template<>\
//class EntityWrapper<Clazz> {\
//public:\
//    auto getReflectionInfo(std::shared_ptr<Clazz> entity) {\
//        using _Clazz=Clazz;\
//        return std::make_tuple(\
//                EntityTable(entity, #Clazz, "user"),##__VA_ARGS__\
//        );\
//    }\
//};
//
//RRR(User,
//    EntityResultMap(id),
//    EntityResultMap(name),
//    EntityResultMap(team)
//)

#endif //MAPPER_USER_HPP
