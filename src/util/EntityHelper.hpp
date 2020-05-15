//
// Created by Jsiyong on 2020-05-14.
//

#ifndef MAPPER_ENTITYHELPER_HPP
#define MAPPER_ENTITYHELPER_HPP

#include <tuple>
#include <entity/EntityTableMap.hpp>
#include <memory>
#include <map>
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
        static void getResultMap(const Tuple &tuple, std::shared_ptr<EntityTableMap> resultMap) {
            //编译期递归
            ResultGetter<Tuple, N - 1>::getResultMap(tuple, resultMap);
            //模板匹配
            bind2ResultMap(std::get<N - 1>(tuple), resultMap);
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
            return pair.second.getColumn();
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
        template<typename T>
        static void bind2ResultMap(const T &t, std::shared_ptr<EntityTableMap> resultMap) {}

        //获取EntityTable,并加入表格中
        static void bind2ResultMap(const EntityTable &entityTable, std::shared_ptr<EntityTableMap> resultMap) {
            resultMap->getEntityTables().emplace_back(entityTable);
        }

        //获取实体字段到表格列的映射
        template<typename R, typename T>
        static void
        bind2ResultMap(const std::pair<R T::*, EntityColumn> &pair, std::shared_ptr<EntityTableMap> resultMap) {
            auto &propertyMap = resultMap->getPropertyMap();
            propertyMap.insert(std::make_pair(pair.second.getProperty(), pair.second));
            //递归查找其他关联类型的resultMap
            auto relatedEntity = std::make_shared<R>();
            //关联的反射信息
            auto relatedReflectionInfo = EntityWrapper<R>().getReflectionInfo(relatedEntity);
            //递归获取结果集映射关系
            EntityHelper::getResultMap(relatedReflectionInfo, resultMap);
        }
    };

    template<typename Tuple>
    struct ResultGetter<Tuple, 0> {
        static void getResultMap(const Tuple &, std::shared_ptr<EntityTableMap>) {}

        template<typename T>
        static void getProperty(const Tuple &, T, std::string &) {}
    };


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
    template<typename T>
    static void getResultMap(const T &t, std::shared_ptr<EntityTableMap> resultMap) {}

public:

    /**
     * 从tuple中获取ResultMap
     * @tparam Args
     * @param tuple
     * @param resultMap
     */
    template<typename... Args>
    static void getResultMap(const std::tuple<Args...> &tuple, std::shared_ptr<EntityTableMap> resultMap) {
        ResultGetter<decltype(tuple), sizeof...(Args)>::getResultMap(tuple, resultMap);
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
        auto resultTuple = EntityWrapper<Entity>().getReflectionInfo(entity);
        return getProperty(resultTuple, t);
    }
};

#endif //MAPPER_ENTITYHELPER_HPP
