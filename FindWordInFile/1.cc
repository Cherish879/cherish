#include "./1.h"

int main() {
    ifstream file("123.txt");
    if (!file.is_open()) {
        cerr << "无法打开文件" << endl;
        return 1;
    }

    TextQuery tq(file); // 加载文件并建立查询系统
    file.close(); // 关闭文件，因为已经读取完毕

    // 进行查询
    string queryWord = "will"; // 假设我们要查询的单词
    Query q = Query(queryWord);
    cout << "查询: " << q.rep() << endl;
    auto results = q.eval(tq);
    // 打印查询结果
    print(cout, results) << endl;

    // 尝试复合查询，例如 "example" AND "text"，需要有对应的逻辑实现
    Query q2 = Query("and");
    Query q3 = Query("his");
    Query q4 = Query("like");
    Query andQuery = ~(q & q2 | q3);
    cout << "复合查询: " << andQuery.rep() << endl;
    auto andResults = andQuery.eval(tq);
    // 打印复合查询结果
    print(cout, andResults) << endl;

    return 0;
}
