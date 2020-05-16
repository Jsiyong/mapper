//
// Created by Jsiyong on 2020-05-16.
//

#ifndef MAPPER_ALIASHELPER_HPP
#define MAPPER_ALIASHELPER_HPP

#include <map>
#include <typeindex>
#include <set>
#include "Singleton.hpp"
#include "CommonUtils.hpp"

/**
 * 专门解决数据库表别名的帮助类,包括数据库列和数据库表
 */
class AliasHelper : public Singleton<AliasHelper> {
private:
    std::map<std::type_index, std::string> typeAliasMap;//类型别名的map,数据库所有实体类的类型的别名都在这里
    std::set<std::string> aliasSet;//别名的集合,方便查找是否存在重名

private:
    /**
     * 生成类型的别名,并加入map中存储,同时返回新生成的别名
     * @tparam T 实体类类型
     * @return
     */
    template<typename T>
    std::string genTypeAlias() {
        //别名规则,随机a-z的字符,+随机的0-9字符合在一起,比如说: a0,b1,c7等
        std::string alias;
        //只要为空,就开始循环获取新的别名
        while (alias.empty()) {
            char c = (char) CommonUtils::getRandomNum('a', 'z');//a-z范围内的随机字符
            int num = CommonUtils::getRandomNum(0, 9);//0-9范围内的随机数
            alias += c;
            alias += std::to_string(num);
            //若插入不成功,说明已经有重复的了,直接再来一遍
            if (!aliasSet.insert(alias).second) {
                alias.clear();//清空
            }
        }
        //插入别名map中
        typeAliasMap.insert(std::make_pair(std::type_index(typeid(T)), alias));
        return alias;
    }

public:

    /**
     * 根据类型获取别名
     * @tparam T 数据库实体类类型
     * @return
     */
    template<typename T>
    static std::string getAliasFromType() {
        auto helper = getInstance();
        if (helper->typeAliasMap.count(typeid(T)) <= 0) {
            return helper->genTypeAlias<T>();
        }
        return helper->typeAliasMap.at(typeid(T));
    }

};

#endif //MAPPER_ALIASHELPER_HPP
