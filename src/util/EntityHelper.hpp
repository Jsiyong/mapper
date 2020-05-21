//
// Created by Jsiyong on 2020-05-14.
//

#ifndef MAPPER_ENTITYHELPER_HPP
#define MAPPER_ENTITYHELPER_HPP

#include <tuple>
#include <memory>
#include <map>
#include <entity/EntityTableMap.hpp>
#include <entity/EntityWrapper.hpp>
#include "TypeUtils.hpp"


/**
 * 帮助实体外包类提取出信息
 */
class EntityHelper {
private:

    /**
     * 比较通用的实体类元组获取器
     */
    struct EntityTupleGetter {

        /**
         * 不是符合的类型,直接忽略
         * @tparam T
         * @tparam Entity
         * @param t
         * @param property
         * @param from
         * @param to
         */
        template<typename T, typename Entity>
        static void appendPropertyValues(const T &, const std::string &, const Entity *, Entity *) {}

        /**
         * 往字段中加入相对应的项
         * @tparam R
         * @tparam T
         * @tparam Entity
         * @param pair
         * @param property
         * @param from
         * @param to
         */
        template<typename R, typename T, typename Entity>
        static void
        appendPropertyValues(const std::pair<std::vector<R> T::*, EntityColumn> &pair, const std::string &property,
                             const Entity *from, Entity *to) {
            if (pair.second.getProperty() == property) {
                //循环插入
                (to->*pair.first).insert(
                        (to->*pair.first).end(),
                        (from->*pair.first).begin(),
                        (from->*pair.first).end()
                );
            }
        }

        template<typename T, typename Entity>
        static void clearPropertyValues(const T &, const std::string &, Entity *) {}

        template<typename R, typename T, typename Entity>
        static void
        clearPropertyValues(const std::pair<std::vector<R> T::*, EntityColumn> &pair, const std::string &property,
                            Entity *entity) {
            if (pair.second.getProperty() == property) {
                //清除列表信息
                (entity->*pair.first).clear();
            }
        }

        //获取属性名
        template<typename T>
        static std::string getProperty(T) { return {}; }

        template<typename T>
        static std::string getProperty(std::pair<T, EntityColumn> pair) {
            return pair.second.getProperty();
        }

        //偏移量是否匹配得上属性
        template<typename T, typename U>
        static bool isMatchProperty(T, U) { return false; }

        //整型的情况
        template<typename T>
        static bool isMatchProperty(std::pair<int T::*, EntityColumn> pair, int T::* u) {
            return pair.first == u;
        }

        //字符串的情况
        template<typename T>
        static bool isMatchProperty(std::pair<std::string T::*, EntityColumn> pair, std::string T::* u) {
            return pair.first == u;
        }

        template<typename Entity, typename T>
        static void bind2ResultMap(Entity *, const T &, std::shared_ptr<EntityTableMap>) {}

        //获取EntityTable,并加入表格中
        template<typename Entity>
        static void
        bind2ResultMap(Entity *, const EntityTable &entityTable, std::shared_ptr<EntityTableMap> resultMap) {
            resultMap->getEntityTables().emplace_back(entityTable);
        }

        //获取实体字段到表格列的映射
        template<typename Entity, typename R, typename T>
        static void
        bind2ResultMap(Entity *entity, const std::pair<R T::*, EntityColumn> &pair,
                       std::shared_ptr<EntityTableMap> resultMap) {
            auto &propertyMap = resultMap->getPropertyMap();
            //注意!!! 成员属性中已经加入了别名了"别名.属性名"
            propertyMap.insert(std::make_pair(pair.second.getProperty(), pair.second));
            //递归查找其他关联类型的resultMap
            //&entity->*pair.first ==>对应的自定义类型字段的指针
            auto entityPropertyPtr = getEntityPropertyPtr(entity, pair);
            //关联的反射信息
            //其中,若是指针类型,其类型需要是指针类型的原先类型
            auto relatedReflectionInfo = EntityWrapper<typename TypeUtils::getType<decltype(entityPropertyPtr)>::type>().getReflectionInfo(
                    entityPropertyPtr);
            //递归获取结果集映射关系
            EntityHelper::getResultMap(entityPropertyPtr, relatedReflectionInfo, resultMap);
        }

        /**
         * 获取实体类对应属性的指针
         * @tparam Entity
         * @tparam R
         * @tparam T
         * @param entity
         * @param pair
         * @return
         */
        template<typename Entity, typename R, typename T>
        static auto getEntityPropertyPtr(Entity *entity, const std::pair<R T::*, EntityColumn> &pair) {
            return &(entity->*pair.first);
        }

        /**
         * 列表类型的要单独处理
         * @tparam Entity
         * @tparam R
         * @tparam T
         * @param entity
         * @param pair
         * @return
         */
        template<typename Entity, typename R, typename T>
        static auto getEntityPropertyPtr(Entity *entity, const std::pair<std::vector<R> T::*, EntityColumn> &pair) {
            //首先先在对应的位置上[该位置的成员是一个列表类型]加入一个项,再返回这个项的地址
            (entity->*pair.first).emplace_back(R());
            return &(entity->*pair.first).front();
        }
    };

    template<typename Tuple, size_t N>
    struct ResultGetter {
        /**
         * 获取ResultMap
         * @param tuple
         * @param resultMap
         */
        template<typename Entity>
        static void getResultMap(Entity *entity, const Tuple &tuple, std::shared_ptr<EntityTableMap> resultMap) {
            //编译期递归
            ResultGetter<Tuple, N - 1>::getResultMap(entity, tuple, resultMap);
            //模板匹配
            EntityTupleGetter::bind2ResultMap(entity, std::get<N - 1>(tuple), resultMap);
        }

        /**
         * 获取与t偏移位置相同的属性名
         * @tparam T
         * @param tuple
         * @param t
         * @return
         */
        template<typename T>
        static void getProperty(const Tuple &tuple, T t, std::string &property) {
            ResultGetter<Tuple, N - 1>::getProperty(tuple, t, property);
            auto val = std::get<N - 1>(tuple);
            if (EntityTupleGetter::isMatchProperty(val, t)) {
                property = EntityTupleGetter::getProperty(val);
            }
        }

        template<typename Entity>
        static void
        appendPropertyValues(const Tuple &tuple, const std::string &property, const Entity *from, Entity *to) {
            ResultGetter<Tuple, N - 1>::appendPropertyValues(tuple, property, from, to);
            EntityTupleGetter::appendPropertyValues(std::get<N - 1>(tuple), property, from, to);
        }

        template<typename Entity>
        static void clearPropertyValues(const Tuple &tuple, const std::string &property, Entity *entity) {
            ResultGetter<Tuple, N - 1>::clearPropertyValues(tuple, property, entity);
            EntityTupleGetter::clearPropertyValues(std::get<N - 1>(tuple), property, entity);
        }
    };

    template<typename Tuple>
    struct ResultGetter<Tuple, 0> {
        template<typename Entity>
        static void getResultMap(Entity *, const Tuple &, std::shared_ptr<EntityTableMap>) {}

        template<typename T>
        static void getProperty(const Tuple &, T, std::string &) {}

        template<typename Entity>
        static void appendPropertyValues(const Tuple &, const std::string &, const Entity *, Entity *) {}

        template<typename Entity>
        static void clearPropertyValues(const Tuple &, const std::string &, Entity *) {}
    };


    //获取属性字符串的实现
    template<typename... Args, typename T>
    static std::string getProperty(const std::tuple<Args...> &tuple, T t) {
        std::string res;
        ResultGetter<decltype(tuple), sizeof...(Args)>::getProperty(tuple, t, res);
        return res;
    }

    /**
     * 这个空的是故意的,专门用来忽略无效的对象
     * @tparam T
     * @param t
     * @param resultMap
     */
    template<typename Entity, typename T>
    static void getResultMap(Entity, T, std::shared_ptr<EntityTableMap> resultMap) {}

    /**
     * 从tuple中获取ResultMap
     * @tparam Args
     * @param tuple
     * @param resultMap
     */
    template<typename Entity, typename... Args>
    static void
    getResultMap(Entity *entity, const std::tuple<Args...> &tuple, std::shared_ptr<EntityTableMap> resultMap) {
        ResultGetter<decltype(tuple), sizeof...(Args)>::getResultMap(entity, tuple, resultMap);
    }

    template<typename Entity, typename... Args>
    static void
    appendPropertyValues(const std::tuple<Args...> &tuple, const std::string &property, const Entity *from,
                         Entity *to) {
        ResultGetter<decltype(tuple), sizeof...(Args)>::appendPropertyValues(tuple, property, from, to);
    }

    template<typename Entity, typename... Args>
    static void
    clearPropertyValues(const std::tuple<Args...> &tuple, const std::string &property, Entity *entity) {
        ResultGetter<decltype(tuple), sizeof...(Args)>::clearPropertyValues(tuple, property, entity);
    }

public:

    /**
     * 获取Entity对象的ResultMap
     * @tparam Entity
     * @param resultMap
     * @return
     */
    template<typename Entity>
    static void getResultMap(Entity *entity, std::shared_ptr<EntityTableMap> resultMap) {
        auto reflectionInfo = EntityWrapper<Entity>().getReflectionInfo(entity);
        getResultMap(entity, reflectionInfo, resultMap);
    }

    /**
     * 根据类在函数中的偏移地址获取对应的实体列属性名
     * @tparam T 返回类型
     * @tparam Entity 实体类
     * @param t 偏移地址
     * @param propertyMap Entity类某个对象的属性列
     * @return
     */
    template<typename T, typename Entity>
    static std::string getProperty(T Entity::* t) {
        auto entity = std::make_shared<Entity>();
        auto resultTuple = EntityWrapper<Entity>().getReflectionInfo(entity.get());
        return getProperty(resultTuple, t);
    }


    /**
     * 往to中加入from的property字段内容
     * @tparam Entity
     * @param property
     * @param from
     * @param to
     */
    template<typename Entity>
    static void appendPropertyValues(const std::string &property, const Entity *from, Entity *to) {
        auto resultTuple = EntityWrapper<Entity>().getReflectionInfo(to);
        appendPropertyValues(resultTuple, property, from, to);
    }


    /**
     * 抹去entity中property属性的内容
     * @tparam Entity
     * @param property
     * @param entity
     */
    template<typename Entity>
    static void clearPropertyValues(const std::string &property, Entity *entity) {
        auto resultTuple = EntityWrapper<Entity>().getReflectionInfo(entity);
        clearPropertyValues(resultTuple, property, entity);
    }

};

#endif //MAPPER_ENTITYHELPER_HPP
