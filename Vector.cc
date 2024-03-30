#include <iostream>
#include <memory>
using namespace std;

template<typename T> 
class Vector 
{ 
public:     
    Vector();     
    ~Vector();          
    void push_back(const T&);      
    void pop_back();
    int size();
    int capacity();
    void print();
private:     
    void reallocate();
private:         
    static std::allocator<T> _alloc;          
    T *_start;
    T *_finish;  
    T *_end_of_storage;
};

template<typename T>
allocator<T> Vector<T>::_alloc;

template<typename T>
Vector<T>::Vector():_start(nullptr),_finish(nullptr),_end_of_storage(nullptr){}

template<typename T>
Vector<T>::~Vector()
{
    while(_start!=_finish){_alloc.destroy(--_finish);}
    _alloc.deallocate(_start,capacity());
}

template<typename T>
void Vector<T>::push_back(const T& value)
{
    if(_finish==_end_of_storage){reallocate();}
    _alloc.construct(_finish++,value);
}

template<typename T>
void Vector<T>::pop_back()
{
    if(_finish!=_start){_alloc.destory(--_finish);}
}

template<typename T>
int Vector<T>::size(){return _finish - _start;}

template<typename T>
int Vector<T>::capacity(){return _end_of_storage - _start;}

template<typename T>
void Vector<T>::reallocate()
{
    int len=size();
    int newsize=(_start==nullptr)?1:2*size();
    T* newspace=_alloc.allocate(newsize);
    for(int i=0;i<len;++i)
    {
        _alloc.construct(newspace+i,move(_start[i]));
        _alloc.destroy(_start+i);
    }
    _alloc.deallocate(_start,capacity());
    _start=newspace;
    _finish=_start+len;
    _end_of_storage=_start+newsize;
}

template<typename T>
void Vector<T>::print()
{
    for(int i=0;i<size();++i)
    {
        cout<<_start[i]<<"  ";
    }
    cout<<endl;
}

int main(void)
{
    Vector<int> v;
    for(int i=0;i<10;++i)
    {
        v.push_back(i);
        v.print();
        cout<<"size="<<v.size()<<"  "<<"capacity="<<v.capacity()<<endl;
    }
    return 0;
}

