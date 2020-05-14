//
// Created by Jsiyong on 2020-05-14.
//

#ifndef MAPPER_ENTITYHELPER_HPP
#define MAPPER_ENTITYHELPER_HPP

#include <tuple>
#include <entity/EntityTableMap.hpp>
#include <memory>

/**
 * 帮助实体外包类提取出信息
 */
class EntityHelper {
private:
    template<typename Tuple, size_t N>
    struct ResultMapGetter {
        static void getResultMap(const Tuple &tuple, std::shared_ptr<EntityTableMap> resultMap) {
            //编译期递归
            ResultMapGetter<Tuple, N - 1>::getResultMap(tuple, resultMap);
            //模板匹配
            bind2ResultMap(std::get<N - 1>(tuple), resultMap);
        }

    private:
        template<typename T>
        static void bind2ResultMap(const T &t, std::shared_ptr<EntityTableMap> resultMap) {}

        //获取EntityTable
        static void bind2ResultMap(const EntityTable &entityTable, std::shared_ptr<EntityTableMap> resultMap) {
            resultMap->setEntityTable(entityTable);
        }

        //获取实体字段到表格列的映射
        template<typename T>
        static void bind2ResultMap(const std::pair<T, EntityColumn> &pair, std::shared_ptr<EntityTableMap> resultMap) {
            auto &propertyMap = resultMap->getPropertyMap();
            propertyMap.insert(std::make_pair(pair.second.getProperty(), pair.second));
        }


    };

    template<typename Tuple>
    struct ResultMapGetter<Tuple, 0> {
        static void getResultMap(const Tuple &tuple, std::shared_ptr<EntityTableMap> resultMap) {}
    };

public:

    /**
     * 从tuple中获取ResultMap
     * @tparam Args
     * @param tuple
     * @param resultMap
     */
    template<typename... Args>
    static void getResultMap(const std::tuple<Args...> &tuple, std::shared_ptr<EntityTableMap> resultMap) {
        ResultMapGetter<decltype(tuple), sizeof...(Args)>::getResultMap(tuple, resultMap);
    }
};

#endif //MAPPER_ENTITYHELPER_HPP
