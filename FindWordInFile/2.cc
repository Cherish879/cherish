#include "./1.h"

//读文件，存储每一行的内容进file,并将所有字符出现行号加入wm
TextQuery::TextQuery(ifstream& is):file(new vector<string>)
{
    string text;
    while(getline(is,text))
    {
        file->push_back(text);
        int n=file->size()-1;
        istringstream line(text);
        string word;
        while(line>>word)
        {
            auto& lines=wm[word];
            if(!lines)
                lines.reset(new set<line_no>);
            lines->insert(n);
        }
    }
}

//查找，参数为要查找的字符串    QueryResult在此调用
QueryResult TextQuery::query(const string&sought)const
{
    static shared_ptr<set<line_no>> nodata(new set<line_no>);
    auto loc=wm.find(sought);
    if(loc==wm.end())
        return QueryResult(sought,nodata,file);
    else
        return QueryResult(sought,loc->second,file);
}

string make_plural(size_t ctr,const string& word,const string&ending)
{
    return (ctr>1)?word+ending:word;
}

//打印结果
ostream& print(ostream& os,const QueryResult& qr)
{
    os<<qr.sought<<" occurs "<<qr.lines->size()<<" "
      <<make_plural(qr.lines->size(),"time","s")<<endl;
    for(auto i:*qr.lines)
        os<<"\t(line"<<i+1<<")"
        <<*(qr.file->begin()+i)<<endl;
    return os;
}

ostream& operator<<(ostream& os,const Query& query)
{
    return os<<query.rep();
}

QueryResult OrQuery::eval(const TextQuery& text)const
{
    auto right=rhs.eval(text),left=lhs.eval(text);
    auto ret_lines=make_shared<set<line_no>>(left.begin(),left.end());
    ret_lines->insert(right.begin(),right.end());
    return QueryResult(rep(),ret_lines,left.get_file());
}

QueryResult AndQuery::eval(const TextQuery& text)const
{
    auto left=lhs.eval(text),right=rhs.eval(text);
    auto ret_lines=make_shared<set<line_no>>();
    set_intersection(left.begin(),left.end(),
                    right.begin(),right.end(),
                    inserter(*ret_lines,ret_lines->begin()));
    return QueryResult(rep(),ret_lines,left.get_file());
}

QueryResult NotQuery::eval(const TextQuery& text)const
{
    auto result=query.eval(text);
    auto ret_lines=make_shared<set<line_no>>();
    auto beg=result.begin(),end=result.end();
    auto sz=result.get_file()->size();
    for(size_t n=0;n!=sz;++n)
    {
        if(beg==end||*beg!=n)
            ret_lines->insert(n);
        else if(beg!=end)
            ++beg;
    }
    return QueryResult(rep(),ret_lines,result.get_file());
}
