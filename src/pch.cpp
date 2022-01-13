#include "pch.h"

template<typename Return,typename... Param>
Return Event<Return(Param...)>:: 

template<class T>
void Attribute<T>::operator=(const T &value){
    if(Value != value){
        Value = value;
        Changed.CarryOut(value);
    }
}

Point::Point(long x = 0,long y = 0){
    X = x;
    Y = y;
}

Size::Size(long width = 0,long heigth = 0){
    Width = width;
    Heigth = heigth;
}