#ifndef __NOCOPY_H__
#define __NOCOPY_H__

class NoCopy
{
protected:
    NoCopy()=default;
    ~NoCopy()=default;

    NoCopy(const NoCopy&)=delete;           //禁止拷贝构造
    NoCopy& operator=(const NoCopy&)=delete;//禁止拷贝复制

    NoCopy(NoCopy&&)=delete;                //禁止移动构造
    NoCopy& operator=(NoCopy&&)=delete;     //禁止移动赋值
};

#endif
