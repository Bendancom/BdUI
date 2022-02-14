#ifndef BDUI_DELEGATE
#define BDUI_DELEGATE
#include <functional>
#include <algorithm>
#include <typeindex>

#if _MSC_VER
#define PlaceHolder std::_Ph
#else
#define PlaceHolder std::_Placeholder
#endif
namespace BdUI{
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
        bool operator==(const Delegate<Return(Param...)> &d) { return d.Ptr == this->Ptr; }
        bool operator==(Return (*f)(Param...)) { return this->Ptr == reinterpret_cast<void*>(f); }
        template<typename T>
        bool operator==(Return (T::*f)(Param...)) { return this->Ptr == reinterpret_cast<void*>(f); }
        bool operator!=(const Delegate<Return(Param...)> &d) { return d.Ptr != this->Ptr; }
        bool operator!=(Return (*f)(Param...)) { return this->Ptr != reinterpret_cast<void*>(f); }
        template<typename T>
        bool operator!=(Return (T::*f)(Param...)) { return this->Ptr != reinterpret_cast<void*>(f); }
    };
}
#endif