# 基于C++11的ORM库
基于c++11的mysql api的封装，高仿通用mapper的接口使用方式

## 特点
1. 模仿通用mapper的接口方式，方便数据库的查询
1. 项目基于C++11,不需要用到更高版本的C++特性
1. 跨平台
1. 无须第三方库的支持

## 项目开发环境说明
1. Clion 2019 + WSL
1. Ubuntu 19.10
1. CMake 3.13.4
1. gcc 9.2.1

## 快速开始
- **表格的准备**

```bash
CREATE TABLE `t_school` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8
```

 - **实体类的准备**
 
需要编写实体类与数据库表字段的映射关系。如果不指定数据库对应的名称，默认采用实体类驼峰命名法到下划线命名法的转换。

```cpp
struct School {
    int id = 0;
    std::string name;
    std::time_t createTime = {};

    School() = default;

    School(const std::string &name, time_t createTime) : name(name), createTime(createTime) {}

    School(int id, const std::string &name, time_t createTime) : id(id), name(name), createTime(createTime) {}
    
    friend std::ostream &operator<<(std::ostream &os, const School &school) {
        os << "id: " << school.id << " name: " << school.name << " createTime: " << school.createTime;
        return os;
    }

};

ResultMap(
        EntityMap(School, "t_school"),
        PropertyMap(id, ColumnType::Id),
        PropertyMap(name),
        PropertyMap(createTime)
)

```

 - **单表查询**

然后就可以进行数据的一系列查询操作了

> 插入操作

```cpp
    Mapper<School> schoolMapper;
    std::cout << "==================插入三个学校==================" << std::endl;
    std::cout << "【学校insertId】"<< schoolMapper.insert(School("Hello-School", system_clock::to_time_t(system_clock::now())))<< std::endl;
    std::cout << "【学校insertId】" << schoolMapper.insert(School("World-School",system_clock::to_time_t(system_clock::now()))) << std::endl;
    std::cout << "【学校insertId】" << schoolMapper.insert(School("My-School",system_clock::to_time_t(system_clock::now()))) << std::endl;
```
输出结果为

```bash
==================插入三个学校==================
【学校insertId】7
【学校insertId】8
【学校insertId】9
```

> 查询操作

```cpp
    std::cout << "==================查询所有学校==================" << std::endl;
    for (auto &school:schoolMapper.selectAll()) {
        std::cout << school << std::endl;
    }
```
输出结果为

```bash
==================查询所有学校==================
id: 7 name: Hello-School createTime: 1590210918
id: 8 name: World-School createTime: 1590210918
id: 9 name: My-School createTime: 1590210918
```

> 更新操作

```cpp
    std::cout << "==================更新学校==================" << std::endl;
    std::cout << "【受影响行数】"<< schoolMapper.updateByPrimaryKey(School(7,"Hello-School_update", system_clock::to_time_t(system_clock::now())))<< std::endl;
```
输出结果为

```bash
==================更新学校==================
【受影响行数】1
```

> 删除操作

```bash
    std::cout << "==================删除学校==================" << std::endl;
    std::cout << "【受影响行数】"<< schoolMapper.deleteByPrimaryKey(7)<< std::endl;
```
输出结果为

```bash
==================删除学校==================
【受影响行数】1
```

> 复杂的查询，Example的使用

现在表中的数据如下
```bash
id	name					create_time

8	World-School			2020-05-23 13:15:18
9	My-School				2020-05-23 13:15:18
10	Hello-School_update		2020-05-23 13:26:17
```
查询name中含有“d”并且id大于8的数据

```cpp
    std::cout << "==================查询name中含有“d”并且id大于8的数据==================" << std::endl;
    //构造Example,构造查询条件
    Example<School> example;
    auto criteria= example.createCriteria();
    criteria->andLike(&School::name,"%d%");
    criteria->andGreaterThan(&School::id,8);
    //执行查询
    for (auto &school:schoolMapper.selectByExample(example)) {
        std::cout << school << std::endl;
    }
```
输出结果为

```bash
==================查询name中含有“d”并且id大于8的数据==================
id: 10 name: Hello-School_update createTime: 1590211577
```

其中，Example< T >和 Criteria 的更多用法参照Java的通用mapper

 - **连表查询**

该ORM除了支持单表查询，还支持连表查询。
>一对一连表查询
 
 表信息如下：
 

```bash
-- 班级表
CREATE TABLE `class` (
  `class_id` int NOT NULL AUTO_INCREMENT,
  `class_name` varchar(255) DEFAULT NULL,
  `school_id` int DEFAULT NULL,
  PRIMARY KEY (`class_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8

-- 学生表
CREATE TABLE `student` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT NULL,
  `class_id` int DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
```
表数据如下

```bash
-- class表
class_id 	class_name 		school_id
1			classA				8
2			classB				8
3			classC				9

-- student表
id	name	class_id	create_time
1	zhangsan	1		2020-05-23 13:56:51
2	lisi		1		2020-05-23 13:57:05
3	wangwu		2		2020-05-23 13:57:24
4	zhaoliu		3		2020-05-23 13:57:39
```
实体类与表字段的映射信息

```cpp
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
```
查询学生id大于1并且班级名称是“classA”的学生

```cpp
        std::cout << "==================查询学生id大于1并且班级名称是“classA”的学生==================" << std::endl;
        Mapper <Student> studentMapper;
        Example <Student> example;
        auto criteria = example.createCriteria();
        criteria->andGreaterThan(&Student::id, 1);
        criteria->andEqualTo(&Class::className, "classA");
        for (auto &school:studentMapper.selectByExample(example)) {
            std::cout << school << std::endl;
        }
```
输出结果为

```bash
==================查询学生id大于1并且班级名称是“classA”的学生==================
id: 2 name: lisi clazz: classId: 1 className: classA schoolId: 8 createTime: 1590213425
```

>一对多连表查询

该ORM关系同时支持一对多的连表查询
将上面School类的信息改一改，改为如下结构

```cpp
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
        //特别注意,“id”为School表的连接字段id,&Class::schoolId为class表的连接字段
        PropertyMap(clazzs, "id", JoinType::OneToMany, &Class::schoolId)
)
```
依旧还是之前的查询接口

```cpp
        //连表查询
        Mapper<School> schoolMapper;
        std::cout << "==================查询所有学校==================" << std::endl;
        for (auto &school:schoolMapper.selectAll()) {
            std::cout << school << std::endl;
        }
```

查询结果如下
```bash
==================查询所有学校==================
id: 8 name: World-School createTime: 1590210918 clazzs: [ { classId: 2 className: classB schoolId: 8}  { classId: 1 className: classA schoolId: 8} ]
id: 9 name: My-School createTime: 1590210918 clazzs: [ { classId: 3 className: classC schoolId: 9} ]
id: 10 name: Hello-School_update createTime: 1590211577 clazzs: []
```
