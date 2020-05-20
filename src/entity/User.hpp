//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_USER_HPP
#define MAPPER_USER_HPP

#include <string>
#include <memory>
#include <tuple>
#include <ostream>
#include "EntityWrapper.hpp"
#include "EntityColumn.hpp"
#include "EntityTable.hpp"

struct Team {
    int teamId = 0;
    std::string teamName = "123";
    int deptId = 0;

    friend std::ostream &operator<<(std::ostream &os, const Team &team) {
        os << "teamId: " << team.teamId << " teamName: " << team.teamName << " deptId: " << team.deptId;
        return os;
    }

};

template<>
class EntityWrapper<Team> {
public:
    auto getReflectionInfo(Team *entity) {
        return std::make_tuple(
                EntityTable(entity, "Team", "team"),
                std::make_pair(&Team::teamId,
                               EntityColumn(entity, &entity->teamId, "teamId", "id", ColumnType::Null, KeySql::Null,
                                            JoinType::Null)),
                std::make_pair(&Team::teamName,
                               EntityColumn(entity, &entity->teamName, "teamName", "team_name", ColumnType::Null,
                                            KeySql::Null, JoinType::Null)),
                std::make_pair(&Team::deptId,
                               EntityColumn(entity, &entity->deptId, "deptId", "dept_id", ColumnType::Null,
                                            KeySql::Null, JoinType::Null))
        );
    }
};

struct Dept {
    int deptId = 0;
    std::string deptName = "123";
    std::vector<Team> teams;

    friend std::ostream &operator<<(std::ostream &os, const Dept &dept) {
        os << "deptId: " << dept.deptId << " deptName: " << dept.deptName << " teams: [";
        for (int i = 0; i < dept.teams.size(); i++) {
            os << dept.teams[i];
            if (i < dept.teams.size() - 1)
                os << ",";
        }
        os << "]";
        return os;
    }

};

template<>
class EntityWrapper<Dept> {
public:
    auto getReflectionInfo(Dept *entity) {
        return std::make_tuple(
                EntityTable(entity, "Dept", "dept"),
                std::make_pair(&Dept::deptId,
                               EntityColumn(entity, &entity->deptId, "deptId", "id", ColumnType::Null, KeySql::Null,
                                            JoinType::Null)),
                std::make_pair(&Dept::deptName,
                               EntityColumn(entity, &entity->deptName, "deptName", "name", ColumnType::Null,
                                            KeySql::Null, JoinType::Null)),
                std::make_pair(&Dept::teams,
                               EntityColumn(entity, &entity->teams, "team", "id", ColumnType::Null,
                                            KeySql::Null, JoinType::LeftJoin, &Team::deptId))
        );
    }
};


struct User {
    int id = 0;
    std::string name = "";
    Team team;

    friend std::ostream &operator<<(std::ostream &os, const User &user) {
        os << "id: " << user.id << " name: " << user.name << " team: " << user.team;
        return os;
    }
};

template<>
class EntityWrapper<User> {
public:

    auto getReflectionInfo(User *entity) {
        return std::make_tuple(
                EntityTable(entity, "User", "user"),
                std::make_pair(&User::id, EntityColumn(entity, &entity->id, "id", "id", ColumnType::Null, KeySql::Null,
                                                       JoinType::Null)),
                std::make_pair(&User::name,
                               EntityColumn(entity, &entity->name, "name", "name", ColumnType::Null, KeySql::Null,
                                            JoinType::Null)),
                //连表查询,表示用user表中的team_id去连接team表,默认是team表的主键id,也可以指定其他列
                std::make_pair(&User::team,
                               EntityColumn(entity, &entity->team, "team", "team_id", ColumnType::Null, KeySql::Null,
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
