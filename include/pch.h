#include <list>
#include <string>
#include <vector>
#include <math.h>
#include <typeinfo>
#include <any>
#include <algorithm>
#include <map>
#include "glad/glad.h"
#ifdef _WIN32
#include "glad/glad_wgl.h"
#endif
#ifdef LINUX
#include "glad/glad_glx.h"
#endif

#define GetLow_Order(variable) (variable & ((long)pow(2,sizeof(variable)/2) - 1))
#define GetHigh_Order(variable) (variable << (sizeof(variable)/2))

template<typename Return,typename... Param> class Delegate;
template<typename Return,typename... Param>
class Delegate<Return(Param...)> : public std::function<Return(Param...)>{
private:
    template<int N, int...I> struct MakeSeqs : MakeSeqs<N - 1, N - 1, I...> {};
    template<int...I> struct MakeSeqs<1, I...>{
	    template<typename T, typename R, typename...Args>
	    static auto bind(T* obj, R(T::*_Func)(Args...)) -> decltype(std::bind(_Func, obj, PlaceHolder<I>{}...)){
		    return std::bind(_Func, obj, PlaceHolder<I>{}...);
	    }
    };
    template <typename T, typename R, typename...Args>
    auto Bind(R(T::*f)(Args...), T* t) -> decltype(MakeSeqs<sizeof...(Args)+1>::bind(t, f)){
	    return MakeSeqs<sizeof...(Args)+1>::bind(t, f);
    }
    std::any FuncPtr;
public:
    Delegate() {}
    Delegate(Return (*f)(Param...)) : FuncPtr(std::make_any<Return(*)(Param...)>(f)) { std::function(f).swap(*this); }
    template<typename T>
    Delegate(Return (T::*f)(Param...),T *t) : FuncPtr(std::make_any<Return(T::*)(Param...)>(f)) { std::function(Bind(f,t)).swap(*this); }
    template<typename T>
    Delegate(T *t,Return (T::*f)(Param...)) : FuncPtr(std::make_any<Return(T::*)(Param...)>(f)) { std::function(Bind(f,t)).swap(*this); }
    const std::type_info &target_type() const {return FuncPtr.type();}
    auto target() -> Return(*)(Param...) {return std::any_cast<Return(*)(Param...)>(FuncPtr);}
    template<typename T>
    auto target() -> Return(T::*)(Param...) {return std::any_cast<Return (T::*)(Param...)>(FuncPtr);}
    void swap(const Delegate<Return(Param...)> &d){
        this->FuncPtr = d.FuncPtr;
        this->swap(d);
    }
    Delegate<Return(Param...)> &operator=(Return (*f)(Param...)){
        FuncPtr = std::make_any<Return(*)(Param...)>(f);
        std::function(f).swap(*this);
        return *this;
    }
    bool operator==(const Delegate<Return(Param...)> &d){ return d.target_type() == this->target_type(); }
    bool operator!=(const Delegate<Return(Param...)> &d){ return d.target_type() != this->target_type(); }
};

template<typename Return,typename... Param> class Event;
template<typename Return,typename... Param>
class Event<Return(Param...)> : public std::vector<Delegate<Return(Param...)>>{
public:
    using std::vector<Delegate<Return(Param...)>>::vector;
    using std::vector<Delegate<Return(Param...)>>::operator=;
    std::map<std::type_index,Return> operator()(Param... args){
        std::map<std::type_index,Return> temp;
        for (auto iter = this->cbegin(); iter != this->cend(); iter++){
            Delegate<Return(Param...)> d(*iter);
            temp.insert(std::pair<std::type_index,Return>(std::type_index(d.target_type()),d(args...)));
        }
        return temp;
    }
    Event<Return(Param...)> &operator+=(const Delegate<Return(Param...)> &d){
        if (this->size() == 0 || (*(std::find(this->begin(),this->end(),d))) != d) this->push_back(d);
        return *this;
    }
    Event<Return(Param...)> &operator-=(const Delegate<Return(Param...)> &d){
        if(this->size() == 0) return *this;
        auto iter = std::find(this->begin(),this->end(),d);
        if ((*iter) == d) this->erase(iter);
        return *this;
    }
};
template<typename... Param>
class Event<void(Param...)> : public std::vector<Delegate<void(Param...)>>{
    public:
    using std::vector<Delegate<void(Param...)>>::vector;
    using std::vector<Delegate<void(Param...)>>::operator=;
    void operator()(Param... args){
        for (auto iter = this->begin();iter != this->end();iter++) (*iter)(args...);
    }
    Event<void(Param...)> &operator+=(const Delegate<void(Param...)> &d){
        if (this->size() == 0 || (*(std::find(this->begin(),this->end(),d))) != d) this->push_back(d);
        return *this;
    }
    Event<void(Param...)> &operator-=(const Delegate<void(Param...)> &d){
        if(this->size() == 0) return *this;
        auto iter = std::find(this->begin(),this->end(),d);
        if ((*iter) == d) this->erase(iter);
        return *this;
    }
};

template<typename T>
class Attribute{
public:
    Event<void(T)> Changed;
    Attribute(const T &value) : Value(value) {}
    operator T() { return Value; }
    const T* operator->() { return &Value; }
    Attribute<T> &operator=(const T &value){
        if(Value != value){
            Changed(value);
            Value = value;
        }
        return *this;
    }
private:
    T Value;
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

struct MouseEvent{
    Point MousePoint;
    bool MouseKeyDown_Or_Up;
    int Mouse_Wheel;
    int MouseKey = Mouse_NULL;
};

struct KeyBoardEvent{
    int value_key;
    bool KeyDown_Or_Up;
    int Shift;          //0b0:NULL; 0b1:LShift; 0b10:RShift; 0b11:AllShift
    int Ctrl;          //0b0:NULL; 0b1:LCtrl; 0b10:RCtrl; 0b11:AllCtrl
    int Alt;           //0b0:NULL; 0b1:LAlt; 0b10:RAlt; 0b11:AllAlt
};

enum MouseKey{
    Mouse_NULL = 0,
    Mouse_Left = 1,
    Mouse_Right = 2,
    Muuse_Middle = 3,
};

enum Key{
    
};