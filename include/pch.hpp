#include <list>
#include <string>
#include <vector>
#include <math.h>
#include <typeinfo>
#include <any>
#include <algorithm>
#include <map>
#include <typeindex>
#include <thread>
#include <mutex>
#include <future>
#include <atomic>
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

template<typename Return,typename... Param> class Delegate;
template<typename Return,typename... Param>
class Delegate<Return(Param...)> : private std::function<Return(Param...)>{
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
    std::type_index type;
public:
    using std::function<Return(Param...)>::operator();
    Delegate() {}
    Delegate(Return (*f)(Param...)) : type(typeid(f)) { std::function(f).swap(*this); }
    template<typename T>
    Delegate(Return (T::*f)(Param...),T *t) : type(typeid(f)) { std::function(Bind(f,t)).swap(*this); }
    template<typename T>
    Delegate(T *t,Return (T::*f)(Param...)) : type(typeid(f)) { std::function(Bind(f,t)).swap(*this); }
    const std::type_index &target_type() const {return type;}

    void swap(const Delegate<Return(Param...)> &d){
        std::any a = std::move(this->type);
        this->type = std::move(type);
        d.type = std::move(a);
        this->swap(d);
    }
    Delegate<Return(Param...)> &operator=(Return (*f)(Param...)){
        type = typeid(f);
        std::function(f).swap(*this);
        return *this;
    }
    bool operator==(const Delegate<Return(Param...)> &d){ return d.type == this->type; }
    bool operator!=(const Delegate<Return(Param...)> &d){ return d.type != this->type; }
};

template<typename Return,typename... Param> class Event;
template<typename Return,typename... Param>
class Event<Return(Param...)> : public std::vector<Delegate<Return(Param...)>>{
public:
    using std::vector<Delegate<Return(Param...)>>::vector;
    using std::vector<Delegate<Return(Param...)>>::operator=;
    void SetCheckFunc(bool (*checkptr)(Param...)) { check = checkptr; };
    void SetReturnCallBackFunc(void (*returncallbackptr)(const std::map<std::type_index,Return>&)) { returncallback = returncallbackptr; }
    void operator()(Param... args){
        if (check != nullptr){
            if (!check(args...)) return;
        }
        else if (!Check(args...)) return;
        std::map<std::type_index,Return> temp;
        for (auto iter = this->cbegin(); iter != this->cend(); iter++){
            temp.insert(std::pair<std::type_index,Return>((*iter).target_type(),(*iter)(args...)));
        }
        if (returncallback != nullptr) returncallback(temp);
        else ReturnCallBack(temp);
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
private:
    virtual bool Check(Param...) { return true; }
    virtual void ReturnCallBack(const std::map<std::type_index,Return>&) { return; }
    bool (*check)(Param...) = nullptr;
    void (*returncallback)(const std::map<std::type_index,Return>&) = nullptr;
};
template<typename... Param>
class Event<void(Param...)> : public std::vector<Delegate<void(Param...)>>{
    public:
    using std::vector<Delegate<void(Param...)>>::vector;
    using std::vector<Delegate<void(Param...)>>::operator=;
    void SetCheckFunc(bool (*checkptr)(Param...)) { check = checkptr; };
    void operator()(const Param &... args){
        if (check != nullptr){
            if (!check(args...)) return;
        }
        else if (!Check(args...)) return;
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
private:
    virtual bool Check(Param...) { return true; }
    bool (*check)(Param...) = nullptr;
};

template<typename Return,typename... Param> class EventList;
template<typename Return,typename... Param>
class EventList<Return(Param...)> : public std::vector<Event<Return(Param...)>>{
public:
    using std::vector<Event<Return(Param...)>>::vector;
    using std::vector<Event<Return(Param...)>>::operator=;
    void operator()(Param... args){
        for (auto iter = this->begin();iter != this->end();iter++) (*iter)(args...);
    }
    EventList<Return(Param...)> &operator+=(const Event<Return(Param...)> &e){
        if (this->size() == 0 || (*(std::find(this->begin(),this->end(),e))) != e) this->push_back(e);
        return *this;
    }
    EventList<Return(Param...)> &operator-=(const Event<Return(Param...)> &e){
        if(this->size() == 0) return *this;
        auto iter = std::find(this->begin(),this->end(),e);
        if ((*iter) == e) this->erase(iter);
        return *this;
    }
};

template<typename T>
class Attribute : private std::atomic<T>{
public:
    EventList<void(T)> Eventlist;
    Attribute(T value,T (*get)(T) = nullptr,bool (*set)(T&) = nullptr) : 
        getptr(get),setptr(set) {this->store(value);}
    Attribute(const Attribute<T> &a,T (*get)(T) = nullptr,bool (*set)(T&) = nullptr) : 
        Eventlist(a.Eventlist),getptr(get),setptr(set) {this->store(a.load());}
    operator T() { 
        if (getptr != nullptr) return getptr(this->load());
        else return get(this->load());
    }
    const T operator->() { return this->load(); }
    Attribute<T> &operator=(T value){
        if (setptr != nullptr) { if (!setptr(value)) return *this; }
        else { if (!set(value)) return *this; }
        this->store(value);
        Eventlist(value);
        return *this;
    }
    Attribute<T> &operator=(const Attribute<T> &a){
        T value = a.load();
        if (setptr != nullptr) { if (!setptr(value)) return *this; }
        else { if (!set(value)) return *this; }
        this->store(value);
        Eventlist(value);
        return *this;
    }
private:
    virtual T get(T t) { return t; };
    virtual bool set(T&) { return true; };
    T (*getptr)(T);
    bool (*setptr)(T&);
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