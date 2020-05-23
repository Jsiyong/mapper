//
// Created by Jsiyong on 2020-05-23.
//

#ifndef MAPPER_TEST_HPP
#define MAPPER_TEST_HPP

#include <ctime>
#include <string>
#include <EntityWrapper.hpp>
#include <ostream>

struct Class {
    int classId = 0;
    std::string className;
    int schoolId = 0;

    friend std::ostream &operator<<(std::ostream &os, const Class &aClass) {
        os << "classId: " << aClass.classId << " className: " << aClass.className << " schoolId: " << aClass.schoolId;
        return os;
    }
};

ResultMap(
        EntityMap(Class),
        PropertyMap(classId, ColumnType::Id),
        PropertyMap(className),
        PropertyMap(schoolId)
)

struct School {
    int id = 0;
    std::string name;
    std::time_t createTime = {};
    std::vector<Class> clazzs;

    friend std::ostream &operator<<(std::ostream &os, const School &school) {
        os << "id: " << school.id << " name: " << school.name << " createTime: " << school.createTime;
        os << " clazzs: [";
        for (int i = 0; i < school.clazzs.size(); i++) {
            os << " { ";
            os << school.clazzs[i];
            os << "} ";
        }
        os << "]";
        return os;
    }

};

ResultMap(
        EntityMap(School, "t_school"),
        PropertyMap(id, ColumnType::Id),
        PropertyMap(name),
        PropertyMap(createTime),
        PropertyMap(clazzs, "id", JoinType::OneToMany, &Class::schoolId)
)

struct Student {
    int id = 0;
    std::string name;
    Class clazz;
    std::time_t createTime;

    friend std::ostream &operator<<(std::ostream &os, const Student &student) {
        os << "id: " << student.id << " name: " << student.name << " clazz: " << student.clazz << " createTime: "
           << student.createTime;
        return os;
    }
};

ResultMap(
        EntityMap(Student),
        PropertyMap(id, ColumnType::Id),
        PropertyMap(name),
        PropertyMap(clazz, "class_id", JoinType::OneToOne, &Class::classId),
        PropertyMap(createTime)
)


#endif //MAPPER_TEST_HPP
