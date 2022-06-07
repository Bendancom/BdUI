#ifndef BDUI_DELEGATE
#define BDUI_DELEGATE
#include <functional>
#include <algorithm>
#include <typeindex>

namespace BdUI{
    template<typename Return,typename... Param> class Delegate;
    template<typename Return,typename... Param>
    class Delegate<Return(Param...)>{
    private:
        template<int N, int...I> struct MakeSeqs : MakeSeqs<N - 1, N - 1, I...> {};
        template<int...I> struct MakeSeqs<1, I...>{
            template<typename T, typename R, typename...Args>
            #if _MSC_VER
            static auto bind(T* obj, R(T::*_Func)(Args...)) -> decltype(std::bind(_Func, obj, std::_Ph<I>{}...)){
                return std::bind(_Func, obj, std::_Ph<I>{}...);
            }
            #else
            static auto bind(T* obj, R(T::*_Func)(Args...)) -> decltype(std::bind(_Func, obj, std::_Placeholder<I>{}...)){
            
                return std::bind(_Func, obj, std::_Placeholder<I>{}...);
            }
            #endif
        };
        template <typename T, typename R, typename...Args>
        auto _Bind(R(T::*f)(Args...), T* t) -> decltype(MakeSeqs<sizeof...(Args)+1>::bind(t, f)){
            return MakeSeqs<sizeof...(Args)+1>::bind(t, f);
        }
        std::function<Return(Param...)> function;
    public:
        Delegate() {}
        template<typename Func>
        Delegate(Func f) : function(f){}
        template<typename T>
        Delegate(Return (T::*f)(Param...),T *t) :  function(_Bind(f,t)) {}
        template<typename T>
        Delegate(T *t,Return (T::*f)(Param...)) : function(_Bind(f,t)) {}
        Delegate(const Delegate<Return(Param...)> &d){
            std::function<Return(Param...)> f(d.function);
            function.swap(f);
        }
        operator bool(){
            return function.operator bool();
        }
        Return operator()(Param... args){
            return function(args...);
        }
        bool exist(){
            return function.operator bool();
        }
        template<typename Func,typename...Args>
        void bind(Func &&f,Args... args){
            function = std::bind(f,args...);
        }
        template<typename Func,typename obj,typename...Args>
        void bind(Func&& f , obj *t ,Args... args){
            if(sizeof...(Args) == 0) function = _Bind(f,t);
            else function = std::bind(f,t,args...);
        }
        void swap(const Delegate<Return(Param...)> &d){
            function.swap(d.function);
        }
        Delegate &operator=(const Delegate<Return(Param...)> &d){
            function = d.function;
            return *this;
        }
        Delegate &operator=(Return (*f)(Param...)){
            function = f;
            return *this;
        }
    };
}
#endif