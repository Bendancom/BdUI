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
        std::size_t Hash_key;
        std::hash<std::string> Hash;
    public:
        Delegate() {}
        template<typename T>
        Delegate(T f) : function(f),Hash_key(Hash(std::to_string(reinterpret_cast<void*>(f)))) {}
        template<typename T>
        Delegate(Return(T::* f)(Param...), T* t) : function(_Bind(f, t)) {
            char buf[sizeof(void*) * 16] = { 0 };
            sprintf(buf, "%p%p", f, t);
            Hash_key = Hash(std::string(buf));
        }
        template<typename T>
        Delegate(T *t,Return (T::*f)(Param...)) : function(_Bind(f,t)) {
            char buf[sizeof(void*) * 16] = { 0 };
            sprintf(buf, "%p%p", f, t);
            Hash_key = Hash(std::string(buf));
        }
        template<typename Func,typename... Args>
        Delegate(Func f, Args... args) {
            function = std::bind(f, args...);
            char buf[sizeof(void*) * 16] = { 0 };
            sprintf(buf, "%p", f);
            Hash_key = Hash(std::string(buf));
        }
        Delegate(const Delegate<Return(Param...)>& d) {
            std::function<Return(Param...)> f(d.function);
            function.swap(f);
        }

        operator bool() {
            return function.operator bool();
        }
        Return operator()(Param... args) {
            return function(args...);
        }

        bool exist() {
            return function.operator bool();
        }
        void swap(const Delegate<Return(Param...)>& d) {
            function.swap(d.function);
            std::size_t&& key = d.Hash_key;
            d.Hash_key = std::move(Hash_key);
            Hash_key = key;
        }

        template<typename T>
        void bind(Return(T::* f)(Param...), T* t) {
            function = _Bind(f, t);
            char buf[sizeof(void*) * 16] = { 0 };
            sprintf(buf, "%p%p", f, t);
            Hash_key = Hash(std::string(buf));
        }
        template<typename Func,typename...Args>
        void bind(Func&& f, Args... args) {
            function = std::bind(std::forward<Func&&>(f), args...);
            char buf[sizeof(void*) * 16] = { 0 };
            sprintf(buf, "%p", f);
            Hash_key = Hash(std::string(buf));
        }
        
        Delegate<Return(Param...)>& operator=(const Delegate<Return(Param...)>& d) {
            function = d.function;
            Hash_key = d.Hash_key;
            return *this;
        }
        Delegate<Return(Param...)>& operator=(Return(*f)(Param...)) {
            function = f;
            Hash(std::to_string(reinterpret_cast<void*>(f)));
            return *this;
        }
        bool operator==(const Delegate<Return(Param...)>& d) {
            if (Hash_key == d.Hash_key && function == d.function) return true;
            else return false;
        }
        bool operator!=(const Delegate<Return(Param...)>& d) {
            if (Hash_key != d.Hash_key || function != d.function) return true;
            else return false;
        }
    };
}
#endif