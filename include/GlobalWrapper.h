#pragma once

#include "stdafx.h"

template <class T>
class GlobalWrapper
{
public:
    GlobalWrapper(GlobalWrapper &other) = delete;
    void operator=(const GlobalWrapper &other) = delete;
    static GlobalWrapper &GetInstance()
    {
        static GlobalWrapper theInstance;
        return theInstance;
    }
    T *Getter() {return obj_;}
    void Setter(T *obj) {obj_ = obj;}
private:
    GlobalWrapper() {}
    ~GlobalWrapper() {}
    T *obj_;
};