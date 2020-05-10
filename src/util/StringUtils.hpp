//
// Created by Jiangsiyong on 5/9/2020.
//

#ifndef MAPPER_STRINGUTILS_HPP
#define MAPPER_STRINGUTILS_HPP

#include <string>

/**
 * 字符串工具类
 */
class StringUtils {
public:
    /**
     * 判断是不是大写字符
     * @param c
     * @return
     */
    static bool isUppercaseAlpha(char c) {
        return (c >= 'A') && (c <= 'Z');
    }

    /**
     * 转换为小写字符
     * @param c
     * @return
     */
    static char toLowerAscii(char c) {
        if (isUppercaseAlpha(c)) {
            c += (char) 0x20;
        }
        return c;
    }

    /**
     * 将驼峰风格替换为下划线风格
     */
    static std::string camelhump2Underline(const std::string &str) {
        std::string result;
        char c;
        for (char i : str) {
            if (isUppercaseAlpha(i)) {
                result += '_';
                result += toLowerAscii(i);
            } else {
                result += i;
            }
        }
        return result.at(0) == '_' ? result.substr(1) : result;
    }
};

#endif //MAPPER_STRINGUTILS_HPP
