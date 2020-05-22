//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_BASEMAPPER_HPP
#define MAPPER_BASEMAPPER_HPP

#include <util/QueryHelper.hpp>
#include "ExampleMapper.hpp"

/**
 * 通用的查询接口
 * @tparam T
 */
template<typename T>
class BaseMapper {
public:
    /////////////////////////// select /////////////////////////////

    /**
     * 根据实体中的属性进行查询，只能有一个返回值，有多个结果是抛出异常，查询条件使用等号
     * @param record
     * @return
     */
    T selectOne(const T &record) {
        auto results = select(record);
        if (results.size() > 1) {
            throw MapperException("select-results have more than one record");
        }
        return results.empty() ? T{} : results.front();
    }

    /**
     * 根据实体中的属性值进行查询，查询条件使用等号
     * @param record
     * @return
     */
    std::vector<T> select(const T &record) {
        Example<T> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(record);
        ExampleMapper<T> exampleMapper;
        return exampleMapper.selectByExample(example);
    }

    /**
     * 查询全部结果
     * @param record
     * @return
     */
    std::vector<T> selectAll() {
        Example<T> example;
        return ExampleMapper<T>().selectByExample(example);
    }

    /**
     * 根据实体中的属性查询总数，查询条件使用等号
     * @param record
     * @return
     */
    int selectCount(const T &record) {
        Example<T> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(record);
        return ExampleMapper<T>().selectCountByExample(example);
    }

    /**
     * 根据主键字段进行查询，方法参数必须包含完整的主键属性，查询条件使用等号
     * @tparam Object
     * @param key
     * @return
     */
    T selectByPrimaryKey(const Object &key) {
        Example<T> example;
        auto keyColumn = example.getKeyEntityColumn();
        auto criteria = example.createCriteria();
        criteria->andEqualTo(keyColumn->getProperty(), key);
        return ExampleMapper<T>().selectOneByExample(example);
    }

    /**
     * 根据主键字段查询总数，方法参数必须包含完整的主键属性，查询条件使用等号
     * @tparam Object
     * @param key
     * @return
     */
    bool existsWithPrimaryKey(const Object &key) {
        Example<T> example;
        auto keyColumn = example.getKeyEntityColumn();
        auto criteria = example.createCriteria();
        criteria->andEqualTo(keyColumn->getProperty(), key);
        return ExampleMapper<T>().selectCountByExample(example) > 0;
    }

    /////////////////////////// update /////////////////////////////
    /**
     * 根据主键更新实体全部字段，null值会被更新
     * @param record
     * @return
     */
    int updateByPrimaryKey(const T &record) {
        Example<T> example;
        auto keyColumn = example.getKeyEntityColumn(record);
        auto criteria = example.createCriteria();
        criteria->andEqualTo(keyColumn->getProperty(), keyColumn->getEntityFieldValue());
        return ExampleMapper<T>().updateByExample(record, example);
    }

    /**
     * 根据主键更新属性不为null的值
     * @param record
     * @return
     */
    int updateByPrimaryKeySelective(const T &record) {
        Example<T> example;
        auto keyColumn = example.getKeyEntityColumn(record);
        auto criteria = example.createCriteria();
        criteria->andEqualTo(keyColumn->getProperty(), keyColumn->getEntityFieldValue());
        return ExampleMapper<T>().updateByExampleSelective(record, example);
    }

    /////////////////////////// insert /////////////////////////////

    /**
     * 保存一个实体，null的属性也会保存，不会使用数据库默认值
     * @param record
     * @return 插入的id
     */
    int insert(const T &record) {
        Example<T> example;
        auto insertContext = example.getInsertContext(record, false);
        return QueryHelper::execute(insertContext.first, insertContext.second).second;
    }

    /**
     * 保存一个实体，null的属性不会保存，会使用数据库默认值
     * @param record
     * @return
     */
    int insertSelective(const T &record) {
        Example<T> example;
        auto insertContext = example.getInsertContext(record, true);
        return QueryHelper::execute(insertContext.first, insertContext.second).second;
    }
    /////////////////////////// delete /////////////////////////////
    /**
    * 根据实体属性作为条件进行删除，查询条件使用等号
    * @param record
    * @return
    */
    int deleteBy(const T &record) {
        Example<T> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(record);
        return ExampleMapper<T>().deleteByExample(example);
    }

    /**
     * 根据主键字段进行删除，方法参数必须包含完整的主键属性
     * @tparam Object
     * @param key
     * @return
     */
    int deleteByPrimaryKey(const Object &key) {
        Example<T> example;
        auto keyColumn = example.getKeyEntityColumn();
        auto criteria = example.createCriteria();
        criteria->andEqualTo(keyColumn->getProperty(), key);
        return ExampleMapper<T>().deleteByExample(example);
    }
};

#endif //MAPPER_BASEMAPPER_HPP
