//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_ENTITYENUM_HPP
#define MAPPER_ENTITYENUM_HPP

/**
 * 数据库列类型
 */
enum class ColumnType {
    Null,//普通类型
    Id//主键
};

/**
 * 主键生成策略
 */
enum class KeySql {
    Null,//无策略
    Id,//最后插入id,即last_insert_id
    UUID//UUID
};

enum class DeleteLogic {
    PhysicalDelete,//物理删除
    LogicDelete//逻辑删除
};

#endif //MAPPER_ENTITYENUM_HPP
