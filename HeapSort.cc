#include <iostream>
#include <vector>
using namespace std;

template <typename T,typename Compare=greater<T>> 
class HeapSort 
{ 
public:  
    HeapSort(T* arr,size_t size,Compare cmp=greater<T>());  
    void sort();
    void print();
private:  
    void heapAdjust(size_t,size_t);  
    vector<T> _vec;
    Compare _cmp;
};

//构造函数
template<typename T,typename Compare>
HeapSort<T,Compare>::HeapSort(T* arr,size_t size,Compare cmp)  
    :_vec(arr,arr+size),_cmp(cmp){}

//堆排序主入口
template<typename T,typename Compare>
void HeapSort<T,Compare>::sort()
{
    //初始化大根堆
    int idx=_vec.size()-2>>1;
    for(int i=idx;i>=0;i--)
    {
        //堆化处理
        heapAdjust(i,_vec.size());
    }

    //反复移除堆顶元素，并重构大根堆
    int temp_len=_vec.size();
    while(temp_len>1)
    {
        swap(_vec[0],_vec[--temp_len]);
        heapAdjust(0,temp_len);
    }
}

//打印数组
template<typename T,typename Compare>
void HeapSort<T,Compare>::print()
{
    for(auto&i:_vec)
    {
        cout<<i<<"  ";
    }
    cout<<endl;
}

//范围内堆排序
template<typename T,typename Compare>
void HeapSort<T,Compare>::heapAdjust(size_t idx,size_t arr_len)
{
    while(1)
    {
        size_t max_idx=idx;
        size_t lchild_idx=(idx<<1)+1;
        size_t rchild_idx=(idx<<1)+2;
        if(lchild_idx<arr_len && _cmp(_vec[lchild_idx],_vec[max_idx]))
            max_idx=lchild_idx;
        if(rchild_idx<arr_len && _cmp(_vec[rchild_idx],_vec[max_idx]))
            max_idx=rchild_idx;
        if(max_idx!=idx)
        {
            swap(_vec[idx],_vec[max_idx]);
            idx=max_idx;
        }
        else
            break;
    }
}

int main(void)
{
    vector<int> v{29, 0, 12, 5, 18, 9, 2, 21, 15, 6, 8, 17, 1, 27, 10, 11, 3, 19, 20, 7, 23, 25, 4, 14, 22, 13, 16, 24, 28, 26};
    HeapSort<int> arr(&v[0],v.size());
    arr.sort();
    arr.print();
    return 0;
}
