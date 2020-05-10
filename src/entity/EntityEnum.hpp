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

/**
 * 删除逻辑
 */
enum class DeleteLogic {
    PhysicalDelete,//物理删除
    LogicDelete//逻辑删除
};

/**
 * 字段转换方式
 */
enum class Style {
    Normal,//原值
    CamelhumpAndLowercase//驼峰转下划线小写形式
};

#endif //MAPPER_ENTITYENUM_HPP
