#include "pch.h"

template<class T>
void Event<T>::CarryOut(const T& value){
    for(const EventCallBack& iter: EventList) iter(value);
}

template<class T>
void Event<T>::operator=(const EventCallBack& e){
    if(EventList.empty()) EventList += e;
    else{
        EventList.clear();
        EventList += e;
    }
}

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