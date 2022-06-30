#ifndef BDUI_ATTRIBUTE
#define BDUI_ATTRIBUTE
#include <mutex>
#include <type_traits>
#include "error.hpp"
#include "event.hpp"
#include "delegate.hpp"

namespace BdUI{
    template<typename... T> class Attribute; 
    template<typename Data,typename GetData,typename SetData>
    class Attribute<Data,GetData,SetData>{
    public:
        EventArray<void(Data)> * ChangedEvent = nullptr;
        Delegate<GetData(Data)> get_func;
        Delegate<bool(SetData,Data&)> set_func;
        Attribute() {}
        Attribute(const Delegate<GetData(Data)>& g , const Delegate<bool(SetData,Data&)>& s) : get_func(g),set_func(s) {}
        Attribute(const Data &v, const Delegate<GetData(Data)>& g , const Delegate<bool(SetData,Data&)>& s) : get_func(g),set_func(s),Value(v),_exist(true) {}
        Attribute(const Attribute<Data,GetData,SetData>&) = delete;
        operator GetData() {
            Mutex.lock();
            GetData&& g = get_func(Value);
            Mutex.unlock();
            return g;
        }
        operator Data(){
            return Value;
        }
        operator const Data*() {
            return &Value;
        }
        operator bool(){
            return _exist;
        }
        bool exist(){
            return _exist;
        }
        const Data* getPointer() {
            return &Value;
        }
        GetData get() {
            return get_func(Value);
        }
        bool set(SetData value) {
            Mutex.lock();
            if (set_func.exist()) {
                Data d;
                if (set_func(value, d)) {
                    Value = d;
                    Mutex.unlock();
                    _exist = true;
                    if (ChangedEvent != nullptr) ChangedEvent->operator()(d);
                    return true;
                }
                else Mutex.unlock();
            }
            else {
                Mutex.unlock();
                throw error::Class::Uninitialize();
            }
            return false;
        }
        Attribute<Data,GetData,SetData> &operator=(SetData value){
            Mutex.lock();
            if(set_func.exist()){
                Data d;
                if (set_func(value, d)) {
                    Value = d;
                    Mutex.unlock();
                    _exist = true;
                    if (ChangedEvent != nullptr) ChangedEvent->operator()(d);
                }
                else Mutex.unlock();
            }
            else {
                Mutex.unlock();
                throw error::Class::Uninitialize();
            }
            return *this;
        }
        Attribute<Data,GetData,SetData> &operator=(const Attribute<Data,GetData,SetData>&) = delete;
    private:
        Data Value;
        bool _exist = false;
        std::mutex Mutex;
    };
    template<typename Data>
    class Attribute<Data>{
    public:
        EventArray<void(Data)> *ChangedEvent = nullptr;
        Delegate<Data(Data)>* get_func = nullptr;
        Delegate<bool(Data,Data&)>* set_func = nullptr;
        Attribute() {}
        Attribute(const Data& v) : Value(v),_exist(true) {}
        Attribute(const Delegate<Data(Data)>& g) : get_func(new Delegate<Data(Data)>(g)) {}
        Attribute(const Data& v, const Delegate<Data(Data)>& g) : get_func(new Delegate<Data(Data)>(g)),Value(v),_exist(true) {}
        Attribute(const Delegate<bool(Data,Data&)>& s) : set_func(new Delegate<bool(Data,Data&)>(s)) {}
        Attribute(const Data& v, const Delegate<bool(Data,Data&)>& s) : set_func(new Delegate<bool(Data, Data&)>(s)),Value(v),_exist(true) {}
        Attribute(const Delegate<Data(Data)>& g, const Delegate<bool(Data,Data&)>& s) : get_func(new Delegate<Data(Data)>(g)),set_func(new Delegate<bool(Data, Data&)>(s)) {}
        Attribute(const Attribute<Data>&) = delete;
        operator Data() {
            if (get_func != nullptr) {
                Mutex.lock();
                Data&& d = (*get_func)(Value);
                Mutex.unlock();
                return d;
            }
            else return Value;
        }
        operator const Data* () {
            return &Value;
        }
        bool exist(){
            return _exist;
        }
        const Data* getPointer() {
            return &Value;
        }
        Data get() {
            if (get_func != nullptr) {
                Mutex.lock();
                Data&& d = (*get_func)(Value);
                Mutex.unlock();
                return d;
            }
            else return Value;
        }
        bool set(Data value) {
            Mutex.lock();
            if (set_func != nullptr) {
                Data d;
                if ((*set_func)(value, d)) {
                    Value = d;
                    Mutex.unlock();
                    _exist = true;
                    if (ChangedEvent != nullptr) ChangedEvent->operator()(d);
                    return true;
                }
                else Mutex.unlock();
            }
            else {
                Value = value;
                Mutex.unlock();
                _exist = true;
                if (ChangedEvent != nullptr) ChangedEvent->operator()(value);
                return true;
            }
            return false;
        }
        bool setOnly(Data data) {
            Value = data;
            if (ChangedEvent != nullptr) ChangedEvent->operator()(data);
            return true;
        }
        Attribute<Data> &operator=(Data value){
            Mutex.lock();
            if (set_func != nullptr) {
                Data d;
                if ((*set_func)(value, d)) {
                    Value = d;
                    Mutex.unlock();
                    _exist = true;
                    if (ChangedEvent != nullptr) ChangedEvent->operator()(d);
                }
                else Mutex.unlock();
            }
            else {
                Value = value;
                Mutex.unlock();
                _exist = true;
                if (ChangedEvent != nullptr) ChangedEvent->operator()(value);
            }
            return *this;
        }
        Attribute<Data> &operator=(const Attribute<Data>&) = delete;
    private:
        Data Value;
        bool _exist = false;
        std::mutex Mutex;
    };
}
#endif