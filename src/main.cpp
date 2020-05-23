//
// Created by Jsiyong on 2020-05-02.
//


#include <common/Mapper.hpp>
#include "Test.hpp"
#include <chrono>
#include <entity/User.hpp>

using std::chrono::system_clock;

int main() {
    {
        Mapper<School> schoolMapper;
//    std::cout << "==================插入三个学校==================" << std::endl;
//    std::cout << "【学校insertId】"<< schoolMapper.insert(School("Hello-School_update", system_clock::to_time_t(system_clock::now())))<< std::endl;
//    std::cout << "【学校insertId】" << schoolMapper.insert(School("World-School",system_clock::to_time_t(system_clock::now()))) << std::endl;
//    std::cout << "【学校insertId】" << schoolMapper.insert(School("My-School",system_clock::to_time_t(system_clock::now()))) << std::endl;

        std::cout << "==================查询所有学校==================" << std::endl;
        for (auto &school:schoolMapper.selectAll()) {
            std::cout << school << std::endl;
        }
//    std::cout << "==================更新学校==================" << std::endl;
//    std::cout << "【受影响行数】"<< schoolMapper.updateByPrimaryKey(School(7,"Hello-School_update", system_clock::to_time_t(system_clock::now())))<< std::endl;

//    std::cout << "==================删除学校==================" << std::endl;
//    std::cout << "【受影响行数】"<< schoolMapper.deleteByPrimaryKey(7)<< std::endl;
        std::cout << "==================查询name中含有“d”并且id大于8的数据==================" << std::endl;
        Example<School> example;
        auto criteria = example.createCriteria();
        criteria->andLike(&School::name, "%d%");
        criteria->andGreaterThan(&School::id, 8);
        for (auto &school:schoolMapper.selectByExample(example)) {
            std::cout << school << std::endl;
        }
    }
    {
        std::cout << "==================查询学生id大于1并且班级名称是“classA”的学生==================" << std::endl;
        Mapper<Student> studentMapper;
        Example<Student> example;
        auto criteria = example.createCriteria();
        criteria->andGreaterThan(&Student::id, 1);
        criteria->andEqualTo(&Class::className, "classA");
        for (auto &school:studentMapper.selectByExample(example)) {
            std::cout << school << std::endl;
        }
    }
    {
        //连表查询
        Mapper<School> schoolMapper;
        std::cout << "==================查询所有学校==================" << std::endl;
        for (auto &school:schoolMapper.selectAll()) {
            std::cout << school << std::endl;
        }
    }

    return 0;
}