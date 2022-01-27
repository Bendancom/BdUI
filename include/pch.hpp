#include <list>
#include <string>
#include <vector>
#include <math.h>
#include <typeinfo>
#include <algorithm>
#include <map>
#include <typeindex>
#include <atomic>

#include "delegate.hpp"
#include "event.hpp"

#include "glad/glad.h"

#ifdef _WIN32
#include "glad/glad_wgl.h"
#endif
#ifdef LINUX
#include "glad/glad_glx.h"
#endif

#if _MSC_VER
#define PlaceHolder std::_Ph
#else
#define PlaceHolder std::_Placeholder
#endif

#define GetLow_Order(variable) (variable & ((long)pow(2,sizeof(variable)/2) - 1))
#define GetHigh_Order(variable) (variable << (sizeof(variable)/2))

namespace BdUI
{
    template<typename T>
    class Attribute : private std::atomic<T>{
    public:
        EventArray<void(T)> EventList;
        T (*get)(T);
        bool (*set)(T&);

        Attribute() {}
        Attribute(T value,T (*getptr)(T) = nullptr,bool (*setptr)(T&) = nullptr) : 
            get(getptr),set(setptr) {this->store(value);}
        Attribute(const Attribute<T> &a,T (*getptr)(T) = nullptr,bool (*setptr)(T&) = nullptr) : 
            EventList(a.EventList),get(getptr),set(setptr) {this->store(a.load());}
        operator T() { 
            if (get != nullptr) return get(this->load());
            else return Get(this->load());
        }
        const T* operator->() { return &(this->load()); }
        Attribute<T> &operator=(T value){
            if (set != nullptr) { if (!set(value)) return *this; }
            else { if (!Set(value)) return *this; }
            this->store(value);
            EventList(value);
            return *this;
        }
        Attribute<T> &operator=(const Attribute<T> &a){
            T value = a.load();
            if (set != nullptr) { if (!set(value)) return *this; }
            else { if (!Set(value)) return *this; }
            this->store(value);
            EventList(value);
            return *this;
        }
    private:
        virtual T Get(T t) { return t; };
        virtual bool Set(T&) { return true; };
    };

    struct Point{
        Point(long x = 0,long y = 0) : X(x),Y(y) {}
        long X;
        long Y;
    };
    struct Size{
        Size(long w = 0,long h = 0) : Width(w),Heigth(h) {}
        long Width;
        long Heigth;
    };
}