//
// Created by Jsiyong on 2020-05-02.
//

#pragma once


#include <string>
#include <mysql.h>
#include <memory.h>

class Stmt;
class Result;



class Stmt
{
private:
    MYSQL_STMT* stmt;
    size_t count;
    MYSQL_BIND* params;
    long unsigned int str_length;

public:
    Stmt() : stmt(0), count(0), params(0) {}
    Stmt(MYSQL_STMT* stmt)
            : stmt(stmt), count(mysql_stmt_param_count(stmt)), params(new MYSQL_BIND[count])
    {
        memset(params, 0, count * sizeof(MYSQL_BIND));
    }

    ~Stmt()
    {
        if (stmt) mysql_stmt_close(stmt);
        delete [] params;
    }

    Stmt(const Stmt&) = delete;
    Stmt& operator=(const Stmt&) = delete;

    Stmt(Stmt&& x) : stmt(x.stmt), count(x.count), params(x.params), str_length(x.str_length)
    {
        x.stmt = 0;
        x.params = nullptr;
    }

    Stmt& operator=(Stmt&& x)
    {
        stmt   = x.stmt;
        count  = x.count;
        params = x.params;
        str_length = x.str_length;
        x.stmt   = 0;
        x.params = nullptr;
        return *this;
    }

    operator bool()
    {
        return !!stmt;
    }



    inline	int step()//the same to execute
    {
        return execute();
    }

    inline	int execute() //run sql
    {
        mysql_stmt_bind_param(stmt, params);
        return mysql_stmt_execute(stmt);
    }

    void bind_param(int i, enum_field_types buffer_type, void* buffer, int buffer_length, my_bool* is_null, long unsigned int* length)
    {
        if (i >= count)
        {
            printf("Stmt::bind_param error! index:%d, count:%d \n", i, count);
            return;
        }
        MYSQL_BIND& b   = params[i];
        b.buffer_type   = buffer_type;
        b.buffer        = (char*)buffer;
        b.buffer_length = buffer_length;
        b.is_null       = is_null;
        b.length        = length;
    }

    inline	int bindint64(int i, int64_t x)
    {
        bind_param(i, MYSQL_TYPE_LONGLONG, (char*)&x, 0, 0, 0);
    }
    inline	void bindint(int i, const int& x)
    {
        bind_param(i, x);
    }

    //char *x should long live tot execute()
    inline	void bindtext(int i, const char* x, int len)
    {
        bind_param(i, x, len);
    }

    //string x cann't be a temp string
    inline	void bindstring(int i, const std::string& x)
    {
        const char* c = x.c_str();
        bind_param(i, MYSQL_TYPE_STRING, (char*)c, x.size(), 0, &(params[i].buffer_length));
    }

private:
    inline void bind_param(int i, const int& x)
    {
        bind_param(i, MYSQL_TYPE_LONG, (char*)&x, 0, 0, 0);
    }

    inline void bind_param(int i, const std::string& x)
    {
        const char* c =  x.c_str();
        bind_param(i, MYSQL_TYPE_STRING, (char*)c, x.size(), 0, &(params[i].buffer_length));
    }

    inline	void bind_param(int i, const char* x, int len)
    {
        bind_param(i, MYSQL_TYPE_STRING, (char*)x, len, 0, &(params[i].buffer_length));
    }

};

class Row
{
private:
    MYSQL_ROW row;
    unsigned long *lengths;
public:
    Row() : row(nullptr), lengths(nullptr) {}
    Row(MYSQL_ROW row, unsigned long* lengths) : row(row), lengths(lengths) {}

    Row(Row&& x) : row(x.row), lengths(x.lengths)
    {
        x.row     = nullptr;
        x.lengths = nullptr;
    }

    Row& operator=(Row&& x)
    {
        using std::swap;
        swap(*this, x);
        return *this;
    }

    operator bool()
    {
        return !!row;
    }

    std::string operator[](size_t n)
    {
        if (n >= *lengths)
            return "";
        return std::string(row[n], lengths[n]);
    }

    std::string columntext(size_t n)
    {
        return std::string(row[n], lengths[n]);
    }

    int columnint(size_t n)
    {
        auto str = std::string(row[n], lengths[n]);
        return std::stoi(str);
    }
    int columnint64(size_t n)
    {
        auto str = std::string(row[n], lengths[n]);
        return std::stoll(str);
    }
};

class Result
{
private:
    MYSQL_RES* res;
    int num_fields;
public:
    Result() : res(0), num_fields(0) {}
    Result(MYSQL_RES* res) : res(res), num_fields(mysql_num_fields(res)) {}
    Result(Result&& r)
    {
        res = r.res;
        r.res = 0;

        num_fields = r.num_fields;
    }

    ~Result()
    {
        if (res) mysql_free_result(res);
    }

    operator bool()
    {
        return !!res;
    }

    Result& operator=(Result&& r)
    {
        mysql_free_result(res);
        res = r.res;
        num_fields = r.num_fields;
        r.res = 0;
        return *this;
    }

    Result& operator=(const Result&) = delete;
    Result(const Result&) = delete;

    Row fetch_row()
    {
        MYSQL_ROW row = mysql_fetch_row(res);
        unsigned long* lengths = mysql_fetch_lengths(res);
        return Row{row, lengths};
    }

    inline  Row next()
    {
        return fetch_row();
    }
};



class Mysqlmgr
{
private:
    Mysqlmgr()
    {
        handle = mysql_init(0);

        //https://dev.mysql.com/doc/refman/5.7/en/c-api-auto-reconnect.html
        //set auto reconnect flag. When mysql restarted, we will auto reconnect
        my_bool reconnect = 1;
        mysql_options(handle, MYSQL_OPT_RECONNECT, &reconnect);
    }
    MYSQL* handle;

    Mysqlmgr(const Mysqlmgr&) = delete;//no copy
    Mysqlmgr& operator=(const Mysqlmgr&) = delete;//no assignment
public:
    inline static Mysqlmgr& getinstance()
    {
        static Mysqlmgr instance;
        return instance;
    };


    ~Mysqlmgr()
    {
        if (handle)
        {
            mysql_close(handle);
        }
    }

    bool connect(const char* host, const char* user, const char* password,
                 const char* db, unsigned int port, const char* unix_socket, unsigned long client_flag)
    {
        MYSQL* h = mysql_real_connect(handle, host, user, password,
                                      db, port, unix_socket, client_flag);
        if (h)
            return true;
        else
            return false;
    }

    Stmt getstmt(const std::string &s)
    {
        MYSQL_STMT* stmt = mysql_stmt_init(handle);
        int x = mysql_stmt_prepare(stmt, s.c_str(), s.size());
        if (x != 0)
        {
            return Stmt{};
        }
        return Stmt{ stmt };
    }



    inline Result query(const std::string &s)
    {
        int x = mysql_real_query(handle, s.c_str(), s.size());
        if (x != 0)
        {
            return Result{};
        }
        return use_result();
    }


    int execSQL(const std::string &sql)
    {
        if (!mysql_real_query(handle, sql.c_str(), (unsigned long)sql.length()))
        {
            return (int)mysql_affected_rows(handle);
        }
        else
        {
            return -1;
        }
    }

    inline int more_results()
    {
        return mysql_more_results(handle);
    }

    inline int next_result()
    {
        int x = mysql_next_result(handle);
        return x;
    }

    inline Result use_result()
    {
        MYSQL_RES* result = mysql_use_result(handle);
        if (!result)
        {
            Result{};
        }
        return Result{ result };
    }

    const char* geterror()
    {
        return  mysql_error(handle);
    }

    int ping()
    {
        if (!mysql_ping(handle))
        {
            return 0;
        }
        return -1;
    }

    int create_db(const std::string& strName)
    {
        std::string temp;
        temp = "CREATE DATABASE ";
        temp += strName;
        if (!mysql_real_query(handle, strName.c_str(), (unsigned long)temp.length() ))
            return 0;
        else
            return -1;
    }

    int drop_db(const std::string& strName)
    {
        std::string temp;
        temp = "DROP DATABASE ";
        temp += strName;
        if (!mysql_real_query(handle, temp.c_str(),	(unsigned long)temp.length()))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }


};