//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_BASEMAPPER_HPP
#define MAPPER_BASEMAPPER_HPP

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

    }

    /**
     * 根据实体中的属性值进行查询，查询条件使用等号
     * @param record
     * @return
     */
    std::vector <T> select(const T &record) {

    }

    /**
     * 查询全部结果
     * @param record
     * @return
     */
    std::vector <T> selectAll() {

    }

    /**
     * 根据实体中的属性查询总数，查询条件使用等号
     * @param record
     * @return
     */
    int selectCount(const T &record) {

    }

    /**
     * 根据主键字段进行查询，方法参数必须包含完整的主键属性，查询条件使用等号
     * @tparam Object
     * @param key
     * @return
     */
    template<typename Object>
    T selectByPrimaryKey(const Object &key) {

    }

    /**
     * 根据主键字段查询总数，方法参数必须包含完整的主键属性，查询条件使用等号
     * @tparam Object
     * @param key
     * @return
     */
    template<typename Object>
    bool existsWithPrimaryKey(const Object &key) {

    }

    /////////////////////////// update /////////////////////////////
    /**
     * 根据主键更新实体全部字段，null值会被更新
     * @param record
     * @return
     */
    int updateByPrimaryKey(const T &record) {

    }

    /**
     * 根据主键更新属性不为null的值
     * @param record
     * @return
     */
    int updateByPrimaryKeySelective(const T &record) {

    }

    /////////////////////////// insert /////////////////////////////

    /**
     * 保存一个实体，null的属性也会保存，不会使用数据库默认值
     * @param record
     * @return
     */
    int insert(const T &record) {

    }

    /**
     * 保存一个实体，null的属性不会保存，会使用数据库默认值
     * @param record
     * @return
     */
    int insertSelective(const T &record) {

    }
    /////////////////////////// delete /////////////////////////////
    /**
    * 根据实体属性作为条件进行删除，查询条件使用等号
    * @param record
    * @return
    */
    int deleteBy(const T &record) {

    }

    /**
     * 根据主键字段进行删除，方法参数必须包含完整的主键属性
     * @tparam Object
     * @param key
     * @return
     */
    template<typename Object>
    int deleteByPrimaryKey(const Object &key) {

    }
};

#endif //MAPPER_BASEMAPPER_HPP
