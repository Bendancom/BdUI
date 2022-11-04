#ifndef BDUI_ATTRIBUTE
#define BDUI_ATTRIBUTE

#include <shared_mutex>
#include "error.hpp"
#include "event.hpp"
#include "delegate.hpp"

namespace BdUI{
    template<typename... T> class Attribute; 

    template<typename Date, typename GetData, typename SetData>
    class Attribute<Date,GetData,SetData>{
    private:
        Date* Value = nullptr;
        std::shared_mutex Mutex;
    public:
        EventArray<void(Date)> * ChangedEvent = nullptr;
        Delegate<GetData(const Date*)> get_func;
        Delegate<bool(SetData,Date*&)> set_func;
        Attribute() {}
        Attribute(const Delegate<GetData(Date*)>& g , const Delegate<bool(SetData,Date*&)>& s) : get_func(g),set_func(s) {}
        Attribute(const Date &v, const Delegate<GetData(Date*)>& g , const Delegate<bool(SetData,Date*&)>& s) : get_func(g),set_func(s),Value(new Date(v)) {}
        Attribute(const Attribute<Date, GetData, SetData>&) = delete;
        ~Attribute() {
            delete ChangedEvent;
            delete Value;
        }
        operator GetData() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value == nullptr) throw error::Class::Uninitialize();
            return get_func(Value);
        }
        operator Date() {
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
        void unlock() {
            Mutex.unlock();
        }
        //便于进行读写操作,给予引用并上锁,别忘解锁
        Date* getReference() {
            Mutex.lock();
            if (Value == nullptr) throw error::Class::Uninitialize();
            return Value;
        }
        bool set(SetData value) {
            Mutex.lock();
            if (set_func.exist()) {
                if (set_func(value, Value)) {
                    Date d = *Value;
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
        Attribute<Date,GetData,SetData> &operator=(SetData value){
            Mutex.lock();
            if(set_func.exist()){
                if (set_func(value, Value)) {
                    Date d = *Value;
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
        Attribute<Date,GetData,SetData> &operator=(const Attribute<Date,GetData,SetData>&) = delete;
    };

    template<typename Date>
    class Attribute<Date> {
    private:
        Date* Value = nullptr;
        std::shared_mutex Mutex;
    public:
        EventArray<void(Date)> *ChangedEvent = nullptr;
        Delegate<Date(const Date*)>* get_func = nullptr;
        Delegate<bool(Date,Date*&)>* set_func = nullptr;
        Attribute() {}
        Attribute(const Date& v) : Value(new Date(v)) {}
        Attribute(const Delegate<Date(Date*)>& g) : get_func(new Delegate<Date(Date*)>(g)) {}
        Attribute(const Delegate<bool(Date,Date*)>& s) : set_func(new Delegate<bool(Date,Date*)>(s)) {}
        Attribute(const Delegate<Date(Date*)>& g, const Delegate<bool(Date, Date*)>& s) : get_func(new Delegate<Date(Date*)>(g)),
            set_func(new Delegate<bool(Date, Date*)>(s)) {}
        Attribute(const Date& v, const Delegate<Date(Date*)>& g) : get_func(new Delegate<Date(Date*)>(g)),Value(new Date(v)) {}
        Attribute(const Date& v, const Delegate<bool(Date,Date*&)>& s) : set_func(new Delegate<bool(Date, Date*)>(s)),Value(new Date(v)) {}
        Attribute(const Date& v, const Delegate<Date(Date*)>& g, const Delegate<bool(Date,Date*&)>& s) : get_func(new Delegate<Date(Date*)>(g)),
            set_func(new Delegate<bool(Date, Date*)>(s)),Value(new Date(v)) {}
        Attribute(const Attribute<Date>&) = delete;
        ~Attribute() {
            delete ChangedEvent;
            delete get_func;
            delete set_func;
        }
        operator Date() {
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
        Date get() {
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
        bool set(Date value) {
            Mutex.lock();
            if (set_func != nullptr) {
                if ((*set_func)(value, Value)) {
                    Date d = *Value;
                    Mutex.unlock();
                    if (ChangedEvent != nullptr) ChangedEvent->operator()(d);
                    return true;
                }
            }
            else {
                if (Value == nullptr) Value = new Date(value);
                else *Value = value;
                Mutex.unlock();
                if (ChangedEvent != nullptr) ChangedEvent->operator()(value);
                return true;
            }
            return false;
        }
        bool setOnly(Date data) {
            Mutex.lock();
            if (Value == nullptr) Value = new Date(data);
            else *Value = data;
            Mutex.unlock();
            if (ChangedEvent != nullptr) ChangedEvent->operator()(data);
            return true;
        }
        Attribute<Date> &operator=(Date value){
            Mutex.lock();
            if (set_func != nullptr) {
                if ((*set_func)(value, Value)) {
                    Date d = *Value;
                    Mutex.unlock();
                    if (ChangedEvent != nullptr) ChangedEvent->operator()(d);
                }
            }
            else {
                if (Value == nullptr) Value = new Date(value);
                else *Value = value;
                Mutex.unlock();
                if (ChangedEvent != nullptr) ChangedEvent->operator()(value);
            }
            return *this;
        }
        Attribute<Date> &operator=(const Attribute<Date>&) = delete;
    };

    template<typename Date, typename GetData, typename SetData>
    class Attribute<Date*, GetData, SetData> {
    private:
        Date* Value = nullptr;
        std::shared_mutex Mutex;
    public:
        EventArray<void(Date*)>* ChangedEvent = nullptr;
        Delegate<GetData(Date*)> get_func;
        Delegate<bool(SetData, Date*&)> set_func;
        Attribute() {}
        Attribute(const Delegate<GetData(Date*)>& g, const Delegate<bool(SetData, Date*&)>& s) : get_func(g), set_func(s) {}
        Attribute(Date* v, const Delegate<GetData(Date*)>& g, const Delegate<bool(SetData, Date*&)>& s) : get_func(g), set_func(s), Value(v) {}
        Attribute(const Attribute<Date*, GetData, SetData>&) = delete;
        ~Attribute() {
            delete ChangedEvent;
            delete Value;
        }
        operator GetData() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (Value == nullptr) throw error::Class::Uninitialize();
            return get_func(Value);
        }
        operator Date*() {
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
                Date* d;
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
        Attribute<Date*, GetData, SetData>& operator=(SetData value) {
            Mutex.lock();
            if (set_func.exist()) {
                Date* d;
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
        Attribute<Date*, GetData, SetData>& operator=(const Attribute<Date*, GetData, SetData>&) = delete;
    };

    template<typename Date>
    class Attribute<Date*> {
    private:
        Date* Value = nullptr;
        std::shared_mutex Mutex;
    public:
        EventArray<void(Date*)>* ChangedEvent = nullptr;
        Delegate<Date*(Date*)>* get_func = nullptr;
        Delegate<bool(Date*, Date*&)>* set_func = nullptr;
        Attribute() {}
        Attribute(Date* v) : Value(v) {}
        Attribute(const Delegate<Date*(Date*)>& g) : get_func(new Delegate<Date*(Date*)>(g)) {}
        Attribute(const Delegate<bool(Date*, Date*&)>& s) : set_func(new const Delegate<bool(Date*, Date*&)>& (s)) {}
        Attribute(const Delegate<Date*(Date*)>& g, const Delegate<bool(Date*, Date*&)>& s) : get_func(new Delegate<Date*(Date*)>(g)),
            set_func(new Delegate<bool(Date*, Date*&)>(s)) {}
        Attribute(const Date& v, const Delegate<Date*(Date*)>& g) : get_func(new Delegate<Date(Date*)>(g)), Value(new Date(v)) {}
        Attribute(const Date& v, const Delegate<bool(Date*, Date*&)>& s) : set_func(new Delegate<bool(Date, Date*)>(s)), Value(new Date(v)) {}
        Attribute(const Date& v, const Delegate<Date*(Date*)>& g, const Delegate<bool(Date*, Date*&)>& s) : get_func(new Delegate<Date(Date*)>(g)),
            set_func(new Delegate<bool(Date, Date*)>(s)), Value(new Date(v)) {}
        Attribute(const Attribute<Date>&) = delete;
        ~Attribute() {
            delete ChangedEvent;
            delete get_func;
            delete set_func;
        }
        operator Date*() {
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
        Date* get() {
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
        bool set(Date* value) {
            Mutex.lock();
            if (set_func != nullptr) {
                Date* d;
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
        bool setOnly(Date* data) {
            Mutex.lock();
            Value = data;
            Mutex.unlock();
            if (ChangedEvent != nullptr) ChangedEvent->operator()(data);
            return true;
        }
        Attribute<Date*>& operator=(Date* value) {
            Mutex.lock();
            if (set_func != nullptr) {
                Date* d;
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
        Attribute<Date*>& operator=(const Attribute<Date*>&) = delete;
    };
}
#endif