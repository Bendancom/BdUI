#ifndef BDUI_ATTRIBUTE
#define BDUI_ATTRIBUTE

#include <shared_mutex>
#include "error.hpp"
#include "event.hpp"
#include "delegate.hpp"

namespace BdUI{
    template<typename... T> class Attribute; 

    template<typename Data>
    class DataLock {
    private:
        std::unique_lock<std::shared_mutex> unique_lock;
        
    public:
        Data* const data;

        DataLock(Data* d, std::unique_lock<std::shared_mutex>&& lock) : data(d),unique_lock(std::forward<std::unique_lock<std::shared_mutex>>(lock)) {}
        DataLock(const DataLock&) = delete;

        DataLock& operator=(const DataLock&) = delete;

        Data* operator->() {
            return data;
        }
        Data* getDatePtr() {
            return data;
        }
        operator Data* () {
            return data;
        }
    };

    template<typename Data, typename GetData, typename SetData>
    class Attribute<Data,GetData,SetData>{
    private:
        Data* Value = nullptr;
        std::shared_mutex Mutex;
    public:
        Event<void(Data)> * ChangedEvent = nullptr;
        Delegate<GetData(const Data*)> get_func;
        Delegate<bool(SetData,Data*&)> set_func;
        Attribute() {}
        Attribute(const Delegate<GetData(Data*)>& g , const Delegate<bool(SetData,Data*&)>& s) : get_func(g),set_func(s) {}
        Attribute(const Data &v, const Delegate<GetData(Data*)>& g , const Delegate<bool(SetData,Data*&)>& s) : get_func(g),set_func(s),Value(new Data(v)) {}
        Attribute(const Attribute<Data, GetData, SetData>&) = delete;
        ~Attribute() {
            delete ChangedEvent;
            delete Value;
        }
        operator GetData() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value == nullptr) throw error::Class::Uninitialize();
            return get_func(Value);
        }
        operator Data() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value == nullptr) throw error::Class::Uninitialize();
            return *Value;
        }
        bool exist() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Value != nullptr ? true: false;
        }
        GetData get() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value == nullptr) throw error::Class::Uninitialize();
            return get_func(Value);
        }
        DataLock<Data>&& getReference() {
            if (Value == nullptr) throw error::Class::Uninitialize();
            DataLock<Data>&& data = DataLock<Data>(Value, std::move(std::unique_lock<std::shared_mutex>(Mutex)));
            return std::forward<DataLock<Data>>(data);
        }
        bool set(SetData value) {
            Mutex.lock();
            if (set_func.exist()) {
                if (set_func(value, Value)) {
                    Data d = *Value;
                    Mutex.unlock();
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
                if (set_func(value, Value)) {
                    Data d = *Value;
                    Mutex.unlock();
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
    };

    template<typename Data>
    class Attribute<Data> {
    private:
        Data* Value = nullptr;
        std::shared_mutex Mutex;
    public:
        Event<void(Data)> *ChangedEvent = nullptr;
        Delegate<Data(const Data*)>* get_func = nullptr;
        Delegate<bool(Data,Data*&)>* set_func = nullptr;
        Attribute() {}
        Attribute(const Data& v) : Value(new Data(v)) {}
        Attribute(const Delegate<Data(Data*)>& g) : get_func(new Delegate<Data(Data*)>(g)) {}
        Attribute(const Delegate<bool(Data,Data*)>& s) : set_func(new Delegate<bool(Data,Data*)>(s)) {}
        Attribute(const Delegate<Data(Data*)>& g, const Delegate<bool(Data, Data*)>& s) : get_func(new Delegate<Data(Data*)>(g)),
            set_func(new Delegate<bool(Data, Data*)>(s)) {}
        Attribute(const Data& v, const Delegate<Data(Data*)>& g) : get_func(new Delegate<Data(Data*)>(g)),Value(new Data(v)) {}
        Attribute(const Data& v, const Delegate<bool(Data,Data*&)>& s) : set_func(new Delegate<bool(Data, Data*)>(s)),Value(new Data(v)) {}
        Attribute(const Data& v, const Delegate<Data(Data*)>& g, const Delegate<bool(Data,Data*&)>& s) : get_func(new Delegate<Data(Data*)>(g)),
            set_func(new Delegate<bool(Data, Data*)>(s)),Value(new Data(v)) {}
        Attribute(const Attribute<Data>&) = delete;
        ~Attribute() {
            delete ChangedEvent;
            delete get_func;
            delete set_func;
        }
        operator Data() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value != nullptr) {
                if (get_func != nullptr) {
                    return (*get_func)(Value);
                }
                else {
                    return *Value;
                }
            }
            else throw error::Class::Uninitialize();
        }
        bool exist() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Value != nullptr ? true : false;
        }
        Data get() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value != nullptr) {
                if (get_func != nullptr) {
                    return (*get_func)(Value);
                }
                else {
                    return *Value;
                }
            }
            else throw error::Class::Uninitialize();
        }
        bool set(Data value) {
            Mutex.lock();
            if (set_func != nullptr) {
                if ((*set_func)(value, Value)) {
                    Data d = *Value;
                    Mutex.unlock();
                    if (ChangedEvent != nullptr) ChangedEvent->operator()(d);
                    return true;
                }
            }
            else {
                if (Value == nullptr) Value = new Data(value);
                else *Value = value;
                Mutex.unlock();
                if (ChangedEvent != nullptr) ChangedEvent->operator()(value);
                return true;
            }
            return false;
        }
        DataLock<Data>&& getReference() {
            if (Value == nullptr) throw error::Class::Uninitialize();
            DataLock<Data>&& data = DataLock<Data>(Value, std::move(std::unique_lock<std::shared_mutex>(Mutex)));
            return std::forward<DataLock<Data>>(data);
        }
        bool setOnly(Data data) {
            Mutex.lock();
            if (Value == nullptr) Value = new Data(data);
            else *Value = data;
            Mutex.unlock();
            if (ChangedEvent != nullptr) ChangedEvent->operator()(data);
            return true;
        }
        Attribute<Data> &operator=(Data value){
            Mutex.lock();
            if (set_func != nullptr) {
                if ((*set_func)(value, Value)) {
                    Data d = *Value;
                    Mutex.unlock();
                    if (ChangedEvent != nullptr) ChangedEvent->operator()(d);
                }
            }
            else {
                if (Value == nullptr) Value = new Data(value);
                else *Value = value;
                Mutex.unlock();
                if (ChangedEvent != nullptr) ChangedEvent->operator()(value);
            }
            return *this;
        }
        Attribute<Data> &operator=(const Attribute<Data>&) = delete;
    };

    template<typename Data, typename GetData, typename SetData>
    class Attribute<Data*, GetData, SetData> {
    private:
        Data* Value = nullptr;
        std::shared_mutex Mutex;
    public:
        Event<void(Data*)>* ChangedEvent = nullptr;
        Delegate<GetData(Data*)> get_func;
        Delegate<bool(SetData, Data*&)> set_func;
        Attribute() {}
        Attribute(const Delegate<GetData(Data*)>& g, const Delegate<bool(SetData, Data*&)>& s) : get_func(g), set_func(s) {}
        Attribute(Data* v, const Delegate<GetData(Data*)>& g, const Delegate<bool(SetData, Data*&)>& s) : get_func(g), set_func(s), Value(v) {}
        Attribute(const Attribute<Data*, GetData, SetData>&) = delete;
        ~Attribute() {
            delete ChangedEvent;
            delete Value;
        }
        operator GetData() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value == nullptr) throw error::Class::Uninitialize();
            return get_func(Value);
        }
        operator Data*() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value == nullptr) throw error::Class::Uninitialize();
            return Value;
        }
        bool exist() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Value != nullptr ? true : false;
        }
        GetData get() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value == nullptr) throw error::Class::Uninitialize();
            return get_func(Value);
        }
        bool set(SetData value) {
            Mutex.lock();
            if (set_func.exist()) {
                Data* d;
                if (set_func(value, d)) {
                    Value = d;
                    Mutex.unlock();
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
        Attribute<Data*, GetData, SetData>& operator=(SetData value) {
            Mutex.lock();
            if (set_func.exist()) {
                Data* d;
                if (set_func(value, d)) {
                    Value = d;
                    Mutex.unlock();
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
        Attribute<Data*, GetData, SetData>& operator=(const Attribute<Data*, GetData, SetData>&) = delete;
    };

    template<typename Data>
    class Attribute<Data*> {
    private:
        Data* Value = nullptr;
        std::shared_mutex Mutex;
    public:
        Event<void(Data*)>* ChangedEvent = nullptr;
        Delegate<Data*(Data*)>* get_func = nullptr;
        Delegate<bool(Data*, Data*&)>* set_func = nullptr;
        Attribute() {}
        Attribute(Data* v) : Value(v) {}
        Attribute(const Delegate<Data*(Data*)>& g) : get_func(new Delegate<Data*(Data*)>(g)) {}
        Attribute(const Delegate<bool(Data*, Data*&)>& s) : set_func(new const Delegate<bool(Data*, Data*&)>& (s)) {}
        Attribute(const Delegate<Data*(Data*)>& g, const Delegate<bool(Data*, Data*&)>& s) : get_func(new Delegate<Data*(Data*)>(g)),
            set_func(new Delegate<bool(Data*, Data*&)>(s)) {}
        Attribute(const Data& v, const Delegate<Data*(Data*)>& g) : get_func(new Delegate<Data(Data*)>(g)), Value(new Data(v)) {}
        Attribute(const Data& v, const Delegate<bool(Data*, Data*&)>& s) : set_func(new Delegate<bool(Data, Data*)>(s)), Value(new Data(v)) {}
        Attribute(const Data& v, const Delegate<Data*(Data*)>& g, const Delegate<bool(Data*, Data*&)>& s) : get_func(new Delegate<Data(Data*)>(g)),
            set_func(new Delegate<bool(Data, Data*)>(s)), Value(new Data(v)) {}
        Attribute(const Attribute<Data>&) = delete;
        ~Attribute() {
            delete ChangedEvent;
            delete get_func;
            delete set_func;
        }
        operator Data*() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value != nullptr) {
                if (get_func != nullptr) {
                    return (*get_func)(Value);
                }
                else {
                    return Value;
                }
            }
            else throw error::Class::Uninitialize();
        }
        bool exist() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Value != nullptr ? true : false;
        }
        Data* get() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value != nullptr) {
                if (get_func != nullptr) {
                    return (*get_func)(Value);
                }
                else {
                    return Value;
                }
            }
            else throw error::Class::Uninitialize();
        }
        bool set(Data* value) {
            Mutex.lock();
            if (set_func != nullptr) {
                Data* d;
                if ((*set_func)(value, d)) {
                    Value = d;
                    Mutex.unlock();
                    if (ChangedEvent != nullptr) ChangedEvent->operator()(d);
                    return true;
                }
            }
            else {
                Value = value;
                Mutex.unlock();
                if (ChangedEvent != nullptr) ChangedEvent->operator()(value);
                return true;
            }
            return false;
        }
        bool setOnly(Data* data) {
            Mutex.lock();
            Value = data;
            Mutex.unlock();
            if (ChangedEvent != nullptr) ChangedEvent->operator()(data);
            return true;
        }
        Attribute<Data*>& operator=(Data* value) {
            Mutex.lock();
            if (set_func != nullptr) {
                Data* d;
                if ((*set_func)(value, d)) {
                    Value = d;
                    Mutex.unlock();
                    if (ChangedEvent != nullptr) ChangedEvent->operator()(d);
                }
            }
            else {
                Value = value;
                Mutex.unlock();
                if (ChangedEvent != nullptr) ChangedEvent->operator()(value);
            }
            return *this;
        }
        Attribute<Data*>& operator=(const Attribute<Data*>&) = delete;
    };
}
#endif