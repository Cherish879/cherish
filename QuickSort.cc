#include <iostream>
#include <vector>
using namespace std;

template<typename T,typename Compare=std::less<T>> 
class MyQsort 
{ 
public:     
    MyQsort(T* arr, size_t size, Compare com=less<T>())
        :_vec(arr,arr+size),_cmp(com){}
    void sort();
    void print(); 
private:    
    void quick(int left, int right, Compare& com);     
    int partition(int left, int right, Compare& com);     
private:    
    vector<T> _vec;
    Compare _cmp;
};


template<typename T,typename Compare>
void MyQsort<T,Compare>::sort()
{
    if(_vec.size())
    {
        quick(0,_vec.size()-1,_cmp);
    } 
}

template<typename T,typename Compare>
void MyQsort<T,Compare>::print()
{
    for(auto& i:_vec)
    {
        cout<<i<<"  ";
    }
    cout<<endl;
}

template<typename T,typename Compare>
void MyQsort<T,Compare>::quick(int left, int right, Compare& com)
{
    if(left<right)
    {
        int MidIdx=partition(left,right,com);
        quick(left,MidIdx-1,com);
        quick(MidIdx+1,right,com);
    }
}

template<typename T,typename Compare>
int MyQsort<T,Compare>::partition(int left, int right, Compare& com)
{
    int mid=_vec[left];
    while(left<right)
    {
        while(left<right&&!com(_vec[right],mid)){--right;}
        _vec[left]=_vec[right];
        while(left<right&&com(_vec[left],mid)){++left;}
        _vec[right]=_vec[left];
    }
    _vec[left]=mid;
    return left;
}


void test()
{
    vector<int> v{29, 0, 12, 5, 18, 9, 2, 21, 15, 6, 8, 17, 1, 27, 10, 11, 3, 19, 20, 7, 23, 25, 4, 14, 22, 13, 16, 24, 28, 26};
    MyQsort<int> arr(&v[0], v.size());
    arr.sort();
    arr.print();
}

int main(void)
{
    test();
    return 0;
}
