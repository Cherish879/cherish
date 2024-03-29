#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <deque>
#include <algorithm>
#include <stack>
#include <set>
#include <queue>
#include <numeric> 
using namespace std;

class LRUCache {
public:
    LRUCache(int capacity) :capacity(capacity){}
    
    int get(int key) {
        if(idx.count(key) == 0) return -1;
        v.splice(v.begin(), v, idx[key]);
        return v.front().second;
    }
    
    void put(int key, int value) {
        if(idx.count(key)) {
            v.splice(v.begin(), v, idx[key]);
            v.front().second = value;
        }
        else {
            v.emplace_front(key, value);            
            idx[key] = v.begin();
        }
        if((int)v.size() > capacity) {
            idx.erase(v.back().first);
            v.pop_back();
        }
    }
private:
    list<pair<int, int>> v;
    unordered_map<int, list<pair<int, int>>::iterator> idx;
    int capacity;
};

int main(void)
{
    LRUCache s(2);
    s.put(1, 1);
    s.put(2, 2);
    s.get(1);
    s.put(3, 3);
    s.get(2);
    return 0;
}
