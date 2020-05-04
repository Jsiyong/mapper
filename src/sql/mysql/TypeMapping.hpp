//
// Created by Jsiyong on 2020-05-04.
//

#ifndef MAPPER_TYPEMAPPING_HPP
#define MAPPER_TYPEMAPPING_HPP

#include <boost/any.hpp>
#include <mysql.h>
#include <util/Singleton.hpp>
#include <map>
#include <string>

class TypeMapping : public Singleton<TypeMapping> {
private:
    const static int STRING_MAX_LENGTH = 65535;

//    std::map<enum_field_types, boost::any> fieldTypeMap = {
//            {MYSQL_TYPE_TINY,       boost::any(char{})},
//            {MYSQL_TYPE_SHORT,      boost::any(short{})},
//            {MYSQL_TYPE_LONG,       boost::any(int{})},
//            {MYSQL_TYPE_FLOAT,      boost::any(float{})},
//            {MYSQL_TYPE_DOUBLE,     boost::any(double{})},
//            {MYSQL_TYPE_LONGLONG,   boost::any((long long) {})},
//            {MYSQL_TYPE_VAR_STRING, boost::any(std::string{})},
//            {MYSQL_TYPE_STRING,     boost::any(std::string{})}
//    };
    std::map<std::string, enum_field_types> fieldTypeMap = {
            {typeid(char).name(),        MYSQL_TYPE_TINY},
            {typeid(int).name(),         MYSQL_TYPE_LONG},
            {typeid(long long).name(),   MYSQL_TYPE_LONGLONG},
            {typeid(std::string).name(), MYSQL_TYPE_STRING}
    };
//    std::map<std::type_info, MYSQL_BIND> paramBindMap = {
//            {typeid(int), getBindParam(MYSQL_TYPE_LONG, (char *) &in, 0, 0, 0)}
//    };

//    std::map<std::type_info, MYSQL_BIND> resultBindMap = {
//
//    };
public:

    void bindAnyValue(MYSQL_BIND &mysqlBind, const boost::any &value) {
        auto typeMapping = getInstance();
        if (typeMapping->fieldTypeMap.find(value.type().name()) != typeMapping->fieldTypeMap.end()) {
            auto fieldTypeEnum = typeMapping->fieldTypeMap[value.type().name()];
            mysqlBind.buffer_type = fieldTypeEnum;
            switch (fieldTypeEnum) {
                case MYSQL_TYPE_TINY: {
                    auto val = boost::any_cast<char>(value);
                    mysqlBind.buffer = &val;
                    break;
                }
                case MYSQL_TYPE_LONG: {
                    auto val = boost::any_cast<int>(value);
                    mysqlBind.buffer = &val;
                    break;
                }
                case MYSQL_TYPE_LONGLONG: {
                    auto val = boost::any_cast<long long>(value);
                    mysqlBind.buffer = &val;
                    break;
                }
                case MYSQL_TYPE_STRING: {
                    auto val = boost::any_cast<std::string>(value);
                    auto length = val.size();
                    mysqlBind.buffer = (void *) val.c_str();
                    mysqlBind.buffer_length = length;
                    mysqlBind.length = &mysqlBind.buffer_length;
                    break;
                }
                default:
                    break;
            }
            return;
        }
        BOOST_LOG_TRIVIAL(error) << "unable to bind value,type:" << value.type().name();
    }

    MYSQL_BIND
    getBindParam(enum_field_types bufferType, void *buffer, int bufferLength, bool isNull, unsigned long length) {
        MYSQL_BIND mysqlBind;
        mysqlBind.buffer_type = bufferType;
        mysqlBind.buffer = (char *) buffer;
        mysqlBind.buffer_length = bufferLength;
        mysqlBind.is_null = &isNull;
        mysqlBind.length = &length;
        return mysqlBind;
    }

//    inline int bindint64(int i, int64_t x) {
//        bind_param(i, MYSQL_TYPE_LONGLONG, (char *) &x, 0, 0, 0);
//    }
//
//    inline void bindint(int i, const int &x) {
//        bind_param(i, x);
//    }
//
//    //char *x should long live tot execute()
//    inline void bindtext(int i, const char *x, int len) {
//        bind_param(i, x, len);
//    }
//
//    //string x cann't be a temp string
//    inline void bindstring(int i, const std::string &x) {
//        const char *c = x.c_str();
//        bind_param(i, MYSQL_TYPE_STRING, (char *) c, x.size(), 0, &(params[i].buffer_length));
//    }
//
//private:
//    inline void bind_param(int i, const int &x) {
//        bind_param(i, MYSQL_TYPE_LONG, (char *) &x, 0, 0, 0);
//    }
//
//    inline void bind_param(int i, const std::string &x) {
//        const char *c = x.c_str();
//        bind_param(i, MYSQL_TYPE_STRING, (char *) c, x.size(), 0, &(params[i].buffer_length));
//    }
//
//    inline void bind_param(int i, const char *x, int len) {
//        bind_param(i, MYSQL_TYPE_STRING, (char *) x, len, 0, &(params[i].buffer_length));
//    }


//    static boost::any getEntityTypeFromFieldType(enum_field_types fieldType) {
//        auto typeMapping = getInstance();
//        if (typeMapping->fieldTypeMap.find(fieldType) != typeMapping->fieldTypeMap.end()) {
//            return typeMapping->fieldTypeMap[fieldType];
//        }
//        BOOST_LOG_TRIVIAL(error) << "unknown fieldType:" << fieldType;
//        return {};
//    }

};

#endif //MAPPER_TYPEMAPPING_HPP
