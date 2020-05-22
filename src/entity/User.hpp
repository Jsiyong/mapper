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

ResultMap(
        EntityMap(Team),
        PropertyMap(teamId, "id", ColumnType::Id),
        PropertyMap(teamName),
        PropertyMap(deptId, "dept_id")
)

struct Dept {
    int deptId = 0;
    std::string deptName = "123";
    std::vector<Team> teams;

    Dept() = default;

    Dept(int deptId, const std::string &deptName) : deptId(deptId), deptName(deptName) {}

    friend std::ostream &operator<<(std::ostream &os, const Dept &dept) {
        os << "deptId: " << dept.deptId << " deptName: " << dept.deptName << " teams: [";
        for (int i = 0; i < dept.teams.size(); i++) {
            os << dept.teams[i];
            if (i < dept.teams.size() - 1)
                os << " ; ";
        }
        os << "]";
        return os;
    }

};

ResultMap(
        EntityMap(Dept),
        PropertyMap(deptId, "id", ColumnType::Id),
        PropertyMap(deptName, "name"),
        PropertyMap(teams, "id", JoinType::OneToMany, &Team::deptId)
)

struct User {
    int id = 0;
    std::string name = "";
    std::time_t createTime = {};
    Team team;

    friend std::ostream &operator<<(std::ostream &os, const User &user) {
        os << "id: " << user.id << " name: " << user.name << " createTime: " << user.createTime << " team: "
           << user.team;
        return os;
    }
};

ResultMap(
        EntityMap(User),
        PropertyMap(id, ColumnType::Id),
        PropertyMap(name),
        PropertyMap(createTime),
        PropertyMap(team, "team_id", JoinType::OneToOne, &Team::teamId)
)

#endif //MAPPER_USER_HPP
