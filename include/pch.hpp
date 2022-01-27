#include <list>
#include <string>
#include <vector>
#include <math.h>
#include <typeinfo>
#include <algorithm>
#include <map>
#include <typeindex>
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

namespace BdUI
{
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
        void *Ptr = nullptr;
        std::type_index *type = nullptr;
    public:
        using std::function<Return(Param...)>::operator();
        Delegate() {}
        Delegate(Return (*f)(Param...)) : Ptr(reinterpret_cast<void*>(f)),type(new std::type_index(typeid(f))) { std::function<Return(Param...)>(f).swap(*this); }
        template<typename T>
        Delegate(Return (T::*f)(Param...),T *t) : Ptr(reinterpret_cast<void*>(f)),type(new std::type_index(typeid(f))) { std::function<Return(Param...)>(Bind(f,t)).swap(*this); }
        template<typename T>
        Delegate(T *t,Return (T::*f)(Param...)) : Ptr(reinterpret_cast<void*>(f)),type(new std::type_index(typeid(f))) { std::function<Return(Param...)>(Bind(f,t)).swap(*this); }
        Delegate(const Delegate<Return(Param...)> &d) : Ptr(d.Ptr) { if (d.type != nullptr) { type = new std::type_index(*d.type); } }
        const std::type_index target_type() {return *type;}
        void swap(const Delegate<Return(Param...)> &d){
            this->swap(d);
            void *temp = std::move(Ptr);
            Ptr = std::move(d.Ptr); 
            d.Ptr = std::move(temp);
            if (type != nullptr || d.type != nullptr){
                std::type_index *temp_type = type;
                type = d.type;
                d.type = temp_type;
            }
        }
        Delegate &operator=(const Delegate<Return(Param...)> &d){
            Ptr = d.Ptr;
            if (d.type != nullptr){
                type = new std::type_index(*d.type);
            }
            std::function<Return(Param...)>(d).swap(*this);
            return *this;
        }
        bool operator==(const Delegate<Return(Param...)> &d){ return d.Ptr == this->Ptr; }
        bool operator!=(const Delegate<Return(Param...)> &d){ return d.Ptr != this->Ptr; }
    };

    template<typename Return,typename... Param> class Event;
    template<typename Return,typename... Param>
    class Event<Return(Param...)> : public std::vector<Delegate<Return(Param...)>>{
    public:
        bool (*check)(const Param &...) = nullptr;
        void (*returnCallback)(const std::map<std::type_index,Return>&) = nullptr;
        using std::vector<Delegate<Return(Param...)>>::vector;
        using std::vector<Delegate<Return(Param...)>>::operator=;
        void operator()(Param... args){
            if (check != nullptr){
                if (!check(args...)) return;
            }
            else if (!Check(args...)) return;
            std::map<std::type_index,Return> temp;
            for (auto iter = this->cbegin(); iter != this->cend(); iter++){
                temp.insert(std::pair<std::type_index,Return>((*iter).target_type(),(*iter)(args...)));
            }
            if (returnCallback != nullptr) returnCallback(temp);
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
        virtual bool Check(const Param &...) { return true; }
        virtual void ReturnCallBack(const std::map<std::type_index,Return>&) { return; }
    };
    template<typename... Param>
    class Event<void(Param...)> : public std::vector<Delegate<void(Param...)>>{
    public:
        bool (*check)(const Param &...) = nullptr;
        using std::vector<Delegate<void(Param...)>>::vector;
        using std::vector<Delegate<void(Param...)>>::operator=;
        void operator()(Param... args){
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
        virtual bool Check(const Param &...) { return true; }
    };

    template<typename Return,typename... Param> class EventArray;
    template<typename Return,typename... Param>
    class EventArray<Return(Param...)> : public std::vector<Event<Return(Param...)>>{
    public:
        using std::vector<Event<Return(Param...)>>::vector;
        using std::vector<Event<Return(Param...)>>::operator=;
        void operator()(Param... args){
            for (auto iter = this->begin();iter != this->end();iter++) (*iter)(args...);
        }
        EventArray<Return(Param...)> &operator+=(const Event<Return(Param...)> &e){
            if (this->size() == 0 || (*(std::find(this->begin(),this->end(),e))) != e) this->push_back(e);
            return *this;
        }
        EventArray<Return(Param...)> &operator-=(const Event<Return(Param...)> &e){
            if(this->size() == 0) return *this;
            auto iter = std::find(this->begin(),this->end(),e);
            if ((*iter) == e) this->erase(iter);
            return *this;
        }
    };

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