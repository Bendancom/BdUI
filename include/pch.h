#include <list>
#include <string>
#include <vector>
#include <math.h>
#include <typeinfo>
#include "glad/glad.h"
#ifdef _WIN32
#include "glad/glad_wgl.h"
#endif
#ifdef LINUX
#include "glad/glad_glx.h"
#endif
using namespace std;

#define GetLow_Order(variable) (variable & ((long)pow(2,sizeof(variable)/2) - 1))
#define GetHigh_Order(variable) (variable << (sizeof(variable)/2))

template<typename Return,typename... Param> class Delegate;
template<typename Return,typename... Param>
class Delegate<Return(Param...)> : private std::function<Return(Param...)>{
private:
    using F = Return(*)(Param...);
    template<typename T>
    using F = Return(T::*)(Param...);
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
public:
    using std::function<Return(Param...)>::function;
    using std::function<Return(Param...)>::operator=;
    using std::function<Return(Param...)>::swap;
    template<typename T>
    Delegate(F<T> f,T *t) {
        this->operator=(Bind(f,t));
    }
    template<typename T>
    Delegate(T *t,F<T> f) {
        this->operator=(Bind(f,t));
    }
};

template<typename Return,typename... Param>
class Event;

template<typename Return,typename... Param>
class Event<Return(Param...)>{
private:
    vector<Delegate<Return(Param...)>> EventList;
public:
    Return operator();
};
template<class T>
class Attribute{
public:
    Attribute();
    Attribute(const T& value) : Value(value);
    ~Attribute();
    operator T() => return Value;
    const T* operator->() => return &Value;
    void operator=(const T& value);
private:
    T Value;
    Event<T> Changed;
};

struct Point{
    Point(long,long);
    Point();
    long X;
    long Y;
};
struct Size{
    Size(long,long);
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