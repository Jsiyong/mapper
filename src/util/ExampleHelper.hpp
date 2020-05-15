//
// Created by Jsiyong on 2020-05-14.
//

#ifndef MAPPER_EXAMPLEHELPER_HPP
#define MAPPER_EXAMPLEHELPER_HPP

#include <string>
#include <entity/Criterion.hpp>
#include <entity/Criteria.hpp>
#include <memory>
#include <vector>

/**
 * Example的工具类
 */
class ExampleHelper {
public:

    /**
     * 从一个小准则中获取SQL条件
     * @param criterion
     * @return
     */
    static std::string getConditionFromCriterion(const Criterion &criterion) {
        std::string answer = criterion.getCondition();
        if (criterion.isNoValue()) {
            answer += "";
        }
        if (criterion.isSingleValue()) {
            answer += " ?";
        }
        if (criterion.isBetweenValue()) {
            answer += " ? " + SQLConstants::AND + " ?";
        }
        if (criterion.isListValue()) {
            answer += " (";
            for (int i = 0; i < criterion.getCollectionInfo().getSize(); ++i) {
                answer += 0 == i ? " ?" : " ,?";
            }
            answer += " )";
        }
        return answer;
    }

    /**
     * 从多个小准则中获取sql语句
     * @param criteria
     * @return
     */
    static std::string getConditionFromCriteria(const Criteria &criteria) {
        std::string answer;
        auto &cria = criteria.getCriteria();
        for (int i = 0; i < cria.size(); ++i) {
            auto condition = getConditionFromCriterion(cria[i]);
            //列表第一个要忽略and和or的判断
            answer += 0 == i ? condition : " " + cria[i].getAndOr() + " " + condition;
        }
        return answer;
    }
};

#endif //MAPPER_EXAMPLEHELPER_HPP
