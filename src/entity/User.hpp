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
#include "EntityTableMap.hpp"

struct Team {
    int teamId = 0;
    std::string teamName = "123";

};

template<>
class EntityWrapper<Team> {
public:
    auto getReflectionInfo(std::shared_ptr<Team> entity) {
        return std::make_tuple(
                EntityTable("Team", "team"),
                std::make_pair(&Team::teamId, EntityColumn(&entity->teamId, "teamId", "teamId")),
                std::make_pair(&Team::teamName, EntityColumn(&entity->teamName, "teamName", "teamName"))
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
                EntityTable("User", "user"),
                std::make_pair(&User::id, EntityColumn(&entity->id, "id", "id")),
                std::make_pair(&User::name, EntityColumn(&entity->name, "name", "name")),
                //连表查询,表示用user表中的team_id去连接team表,默认是team表的主键id,也可以指定其他列
                std::make_pair(&User::team, EntityColumn(&entity->team, "team", "teamId"))
        );
    }
};

#endif //MAPPER_USER_HPP
