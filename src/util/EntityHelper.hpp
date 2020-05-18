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


/**
 * 帮助实体外包类提取出信息
 */
class EntityHelper {
private:
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
            bind2ResultMap(entity, std::get<N - 1>(tuple), resultMap);
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
            if (isMatchProperty(val, t)) {
                property = getProperty(val);
            }
        }

    private:

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

    private:
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
            //关联的反射信息
            auto relatedReflectionInfo = EntityWrapper<R>().getReflectionInfo(&(entity->*pair.first));
            //递归获取结果集映射关系
            EntityHelper::getResultMap(&(entity->*pair.first), relatedReflectionInfo, resultMap);
        }
    };

    template<typename Tuple>
    struct ResultGetter<Tuple, 0> {
        template<typename Entity>
        static void getResultMap(Entity *, const Tuple &, std::shared_ptr<EntityTableMap>) {}

        template<typename T>
        static void getProperty(const Tuple &, T, std::string &) {}
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

public:

    /**
     * 获取Entity对象的ResultMap,并返回一个对象
     * @tparam Entity
     * @param resultMap
     * @return
     */
    template<typename Entity>
    static std::shared_ptr<Entity> getResultMap(std::shared_ptr<EntityTableMap> resultMap) {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();
        auto reflectionInfo = EntityWrapper<Entity>().getReflectionInfo(entity.get());
        getResultMap(entity.get(), reflectionInfo, resultMap);
        return entity;
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
};

#endif //MAPPER_ENTITYHELPER_HPP
