#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <string>
#include <set>
#include <regex>
#include <vector>
#include <fstream>
#include <memory>
#include <map>
#include <algorithm>

using namespace std;

class QueryResult;

//已完成，保存输入文件的类
class TextQuery
{
public:
    using line_no=vector<string>::size_type;//set的返回值类型，line_no是别名
    TextQuery(ifstream&);//读文件，存储每一行的内容进file
    QueryResult query(const string&)const;
private:
    shared_ptr<vector<string>> file;  //存行内容
    map<string,shared_ptr<set<line_no>>> wm;
};

//已完成，保存查询结果、有个print函数
class QueryResult
{
    friend ostream& print(ostream&,const QueryResult&);//打印结果
public:
    using line_no=TextQuery::line_no;
    //参数1：查找的字符串  参数2：新set或者是map中已有元素的set  参数3：记录行内容的vector
    QueryResult(string s,shared_ptr<set<line_no>> p,shared_ptr<vector<string>> f)
        :sought(s),lines(p),file(f){}
    auto begin() const { return lines->begin(); }
    auto end() const { return lines->end(); }
    shared_ptr<vector<string>> get_file() const { return file; }//返回行内容容器
private:
    string sought;//参数1：查找的字符串
    shared_ptr<set<line_no>> lines;//参数2：新set或者是map中已有元素的set
    shared_ptr<vector<string>> file;//参数3：记录行内容的vector
};

//已完成，抽象基类
class Query_base
{
    friend class Query;
protected:
    using line_no=TextQuery::line_no;
    virtual ~Query_base()=default;
private:
    virtual QueryResult eval(const TextQuery&)const=0;
    virtual string rep()const=0;
};

//已完成，查找给定的string
class WordQuery:public Query_base
{
    friend class Query;
    WordQuery(const string &s):query_word(s){}
    QueryResult eval(const TextQuery& t)const{return t.query(query_word);}
    string rep()const{return query_word;}
private:
    string query_word;
};

//已完成，接口类
class Query
{
    friend Query operator~(const Query&);
    friend Query operator|(const Query&,const Query&);
    friend Query operator&(const Query&,const Query&);
public:
    Query(const string& s):q(new WordQuery(s)){}
    QueryResult eval(const TextQuery& t)const{return q->eval(t);}
    string rep()const{return q->rep();}
private:
    Query(shared_ptr<Query_base> query):q(query){}
    shared_ptr<Query_base> q;
};

//已完成，取反
class NotQuery:public Query_base
{
    friend Query operator~(const Query&);
    NotQuery(const Query& q):query(q){}
    string rep()const{return "~("+query.rep()+")";}
    QueryResult eval(const TextQuery&)const;
private:
    Query query;
};

//已完成，抽象基类，保存两个运算对象的查询类型所需要的数据
class BinaryQuery:public Query_base
{
protected:
    BinaryQuery(const Query& l,const Query& r,string s)
        :lhs(l),rhs(r),opSym(s){}
    string rep()const{return "("+lhs.rep()+" "+opSym+" "+rhs.rep()+")";}
    Query lhs,rhs;
    string opSym;
};

//已完成，A&B
class AndQuery:public BinaryQuery
{
    friend Query operator&(const Query&,const Query&);
    AndQuery(const Query& left,const Query& right)
        :BinaryQuery(left,right,"&"){}
    QueryResult eval(const TextQuery&)const;
};

//已完成，A|B
class OrQuery:public BinaryQuery
{
    friend Query operator|(const Query&,const Query&);
    OrQuery(const Query& left,const Query& right)
        :BinaryQuery(left,right,"|"){}
    QueryResult eval(const TextQuery&)const;
};

ostream& operator<<(ostream& os,const Query& query);

inline Query operator&(const Query& lhs,const Query& rhs)
{
    return shared_ptr<Query_base>(new AndQuery(lhs,rhs));
}

inline Query operator~(const Query& operand)
{
    return shared_ptr<Query_base>(new NotQuery(operand));
}

inline Query operator|(const Query& lhs,const Query& rhs)
{
    return shared_ptr<Query_base>(new OrQuery(lhs,rhs));
}

#endif
