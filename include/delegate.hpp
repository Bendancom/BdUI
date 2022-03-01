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
        auto Bind(R(T::*f)(Args...), T* t) -> decltype(MakeSeqs<sizeof...(Args)+1>::bind(t, f)){
            return MakeSeqs<sizeof...(Args)+1>::bind(t, f);
        }
        std::function<Return(Param...)> function;
        std::type_index type;
    public:
        Delegate() {}
        Delegate(Return (*f)(Param...)) : function(f),type(typeid(f)){}
        template<typename T>
        Delegate(Return (T::*f)(Param...),T *t) :  function(Bind(f,t)),type(typeid(f)) {}
        template<typename T>
        Delegate(T *t,Return (T::*f)(Param...)) : function(Bind(f,t)),type(typeid(f)) {}
        Delegate(const Delegate<Return(Param...)> &d) : type(d.type){
            std::function<Return(Param...)> f(d.function);
            function.swap(f);
        }
        const std::type_index &target_type() {return type;}
        Return operator()(Param... args){
            return function(args...);
        }
        void swap(const Delegate<Return(Param...)> &d){
            function.swap(d.function);
            std::type_index &&t = std::move(type);
            type = std::move(d.type);
            d.type = t;
        }
        Delegate &operator=(const Delegate<Return(Param...)> &d){
            type = d.type;
            function = d.function;
            return *this;
        }
    };
}
#endif