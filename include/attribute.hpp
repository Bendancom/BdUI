#ifndef BDUI_ATTRIBUTE
#define BDUI_ATTRIBUTE
#include <mutex>
#include "event.hpp"

namespace BdUI{
    template<typename... T> class Attribute; 

    template<typename Data,typename GetData,typename SetData>
    class Attribute<Data,GetData,SetData>{
    public:
        EventArray<void(Data)> EventList;
        GetData (*get)(const Data&);
        bool (*set)(Data&,const SetData&);
        Attribute(GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) : get(getptr),set(setptr) {}
        Attribute(const Data &t,GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(const EventArray<void(Data)> &e,GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) :
        EventList(e),get(getptr),set(setptr) {}
        Attribute(const Data &t,const EventArray<void(Data)> &e,GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(const Attribute<Data,GetData,SetData>&) = delete;
        operator GetData() {
            Mutex.lock();
            GetData &&data = (*get)(Value);
            Mutex.unlock();
            return data;
        }
        operator Data(){
            return Value;
        }
        GetData Get(){
            Mutex.lock();
            GetData &&data = (*get)(Value);
            Mutex.unlock();
            return data;
        }
        void Set(SetData value){
            Mutex.lock();
            if (!(*set)(Value,value)){
                Mutex.unlock();
                return;
            }
            EventList(Value);
            Mutex.unlock();
        }
        Attribute<Data,GetData,SetData> &operator=(SetData value){
            Mutex.lock();
            if (!(*set)(Value,value)){
                Mutex.unlock();
                return *this; 
            }
            EventList(Value);
            Mutex.unlock();
            return *this;
        }
        Attribute<Data,GetData,SetData> &operator=(const Attribute<Data,GetData,SetData>&) = delete;
    private:
        Data Value;
        std::mutex Mutex;
    };
    template<typename Data,typename GetData,typename SetData>
    class Attribute<Data*,GetData,SetData>{
        public:
        EventArray<void(Data*)> EventList;
        GetData (*get)(Data* const&);
        bool (*set)(Data*&,Data* const&);
        Attribute(Data* (*getptr)(Data* const&),bool (*setptr)(Data*&,Data* const&)) : get(getptr),set(setptr) {}
        Attribute(Data* const &t,Data* (*getptr)(Data* const&),bool (*setptr)(Data*&,Data* const&)) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(const EventArray<void(Data)> &e,Data* (*getptr)(Data* const&),bool (*setptr)(Data*&,Data* const&)) :
        EventList(e),get(getptr),set(setptr) {}
        Attribute(Data* const &t,const EventArray<void(Data)> &e,Data* (*getptr)(Data* const&),bool (*setptr)(Data*&,Data* const&)) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(const Attribute<Data*,GetData,SetData>&) = delete;
        operator GetData() {
            GetData *data;
            Mutex.lock();
            data = (*get)(Value);
            Mutex.unlock();
            return data;
        }
        operator Data*(){
            return Value;
        }
        Data* Get() {
            Data *data;
            Mutex.lock();
            data = (*get)(Value);
            Mutex.unlock();
            return data;
        }
        void Set(Data *value){
            Mutex.lock();
            if (!(*set)(Value,value)) {
                Mutex.unlock();
                return;
            }
            EventList(Value);
            Mutex.unlock();
        }
        Attribute<Data*,GetData,SetData> &operator=(Data *value){
            Mutex.lock();
            if (!(*set)(Value,value)) {
                Mutex.unlock();
                return *this; 
            }
            EventList(Value);
            Mutex.unlock();
            return *this;
        }
        Attribute<Data*,GetData,SetData> &operator=(const Attribute<Data*,GetData,SetData>&) = delete;
    private:
        Data *Value;
        std::mutex Mutex;
    };
    template<typename Data,typename GetData,typename SetData>
    class Attribute<Data&,GetData,SetData>{
        public:
        EventArray<void(Data)> EventList;
        GetData (*get)(const Data&);
        bool (*set)(Data&,const SetData&);
        Attribute(GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) : get(getptr),set(setptr) {}
        Attribute(Data &t,GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(const EventArray<void(Data)> &e,GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) :
        EventList(e),get(getptr),set(setptr) {}
        Attribute(Data &t,const EventArray<void(Data)> &e,GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(const Attribute<Data&,GetData,SetData> &a) = delete;
        operator GetData() {
            Mutex.lock();
            GetData &&data = get(Value);
            Mutex.unlock();
            return data;
        }
        operator Data(){
            return Value;
        }
        GetData Get(){ 
            Mutex.lock();
            GetData &&data = get(Value);
            Mutex.unlock();
            return data;
        }
        void Set(SetData value){
            Mutex.lock();
            if (!(*set)(Value,value)){
                Mutex.unlock();
                return;
            }
            EventList(Value);
            Mutex.unlock();
        }
        Attribute<Data&,GetData,SetData> &operator=(SetData value){
            Mutex.lock();
            if (!(*set)(Value,value)) {
                Mutex.unlock();
                return *this; 
            }
            EventList(Value);
            Mutex.unlock();
            return *this;
        }
        Attribute<Data&,GetData,SetData> &operator=(const Attribute<Data&,GetData,SetData>&) = delete;
    private:
        Data &Value;
        std::mutex Mutex;
    };
    template<typename Data>
    class Attribute<Data>{
        public:
        EventArray<void(Data)> EventList;
        Data (*get)(const Data&);
        bool (*set)(Data&,const Data&);
        Attribute(Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) : get(getptr),set(setptr) {}
        Attribute(bool (*setptr)(Data&,const Data&)) : set(setptr),get(nullptr) {}
        Attribute(const Data &t,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(const Data &t,bool (*setptr)(Data&,const Data&)) :
        Value(t),set(setptr),get(nullptr) {}
        Attribute(const EventArray<void(Data)> &e,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        EventList(e),get(getptr),set(setptr) {}
        Attribute(const EventArray<void(Data)> &e,bool (*setptr)(Data&,const Data&)) :
        EventList(e),set(setptr),get(nullptr) {}
        Attribute(const Data &t,const EventArray<void(Data)> &e,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(const Data &t,const EventArray<void(Data)> &e,bool (*setptr)(Data&,const Data&)) :
        Value(t),EventList(e),set(setptr),get(nullptr) {}
        Attribute(const Attribute<Data>&) = delete;
        operator Data() {
            Data data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        Data Get() {
            Data data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        void Set(Data value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return; 
                }
            }
            else Value = value;
            EventList(Value);
            Mutex.unlock();
        }
        Attribute<Data> &operator=(Data value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return *this;
                }
            }
            else Value = value;
            EventList(Value);
            Mutex.unlock();
            return *this;
        }
        Attribute<Data> &operator=(const Attribute<Data>&) = delete;
    private:
        Data Value;
        std::mutex Mutex;
    };
    template<typename Data>
    class Attribute<Data*>{
        public:
        EventArray<void(Data*)> EventList;
        Data* (*get)(Data* const&);
        bool (*set)(Data*&,Data* const&);
        Attribute(Data* (*getptr)(Data* const&) = nullptr,bool (*setptr)(Data*&,Data* const&) = nullptr) : get(getptr),set(setptr) {}
        Attribute(bool (*setptr)(Data*&,Data* const&)) : set(setptr),get(nullptr) {}
        Attribute(Data* const &t,Data* (*getptr)(Data* const&) = nullptr,bool (*setptr)(Data*&,Data* const&) = nullptr) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(Data* const &t,bool (*setptr)(Data*&,Data* const&)) :
        Value(t),set(setptr),get(nullptr) {}
        Attribute(const EventArray<void(Data)> &e,Data* (*getptr)(Data* const&) = nullptr,bool (*setptr)(Data*&,Data* const&) = nullptr) :
        EventList(e),get(getptr),set(setptr) {}
        Attribute(const EventArray<void(Data)> &e,bool (*setptr)(Data*&,Data* const&)) :
        EventList(e),set(setptr),get(nullptr) {}
        Attribute(Data* const &t,const EventArray<void(Data)> &e,Data* (*getptr)(Data* const&) = nullptr,bool (*setptr)(Data*&,Data* const&) = nullptr) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(Data* const &t,const EventArray<void(Data)> &e,bool (*setptr)(Data*&,Data* const&)) :
        Value(t),EventList(e),set(setptr),get(nullptr) {}
        Attribute(const Attribute<Data*>&) = delete;
        operator Data*() {
            Data *data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        Data* Get() {
            Data *data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        void Set(Data *value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return; 
                }
            }
            else Value = value;
            EventList(Value);
            Mutex.unlock();
        }
        Attribute<Data*> &operator=(Data *value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return *this; 
                }
            }
            else Value = value;
            EventList(Value);
            Mutex.unlock();
            return *this;
        }
        Attribute<Data*> &operator=(const Attribute<Data*>&) = delete;
    private:
        Data *Value;
        std::mutex Mutex;
    };
    template<typename Data>
    class Attribute<Data&>{
        public:
        EventArray<void(Data)> EventList;
        Data (*get)(const Data&);
        bool (*set)(Data&,const Data&);
        Attribute(const Data &t,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(const Data &t,bool (*setptr)(Data&,const Data&)) :
        Value(t),set(setptr),get(nullptr) {}
        Attribute(const Data &t,const EventArray<void(Data)> &e,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(const Data &t,const EventArray<void(Data)> &e,bool (*setptr)(Data&,const Data&)) :
        Value(t),EventList(e),set(setptr),get(nullptr) {}
        Attribute(const Attribute<Data&>&) = delete;
        operator Data() {
            Data data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        Data Get() {
            Data data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        void Set(Data value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return; 
                }
            }
            else Value = value;
            Mutex.unlock();
            EventList(value);
        }
        Attribute<Data&> &operator=(Data value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return *this; 
                }
            }
            else Value = value;
            Mutex.unlock();
            EventList(value);
            return *this;
        }
        Attribute<Data&> &operator=(const Attribute<Data&>&) = delete;
    private:
        Data &Value;
        std::mutex Mutex;
    };
    template<typename Data>
    class Attribute<Data*&>{
        public:
        EventArray<void(Data*)> EventList;
        Data* (*get)(Data* const&);
        bool (*set)(Data*&,Data* const&);
        Attribute(Data*&t,Data* (*getptr)(Data* const&) = nullptr,bool (*setptr)(Data*&,Data* const&) = nullptr) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(Data*&t,bool (*setptr)(Data*&,Data* const&)) :
        Value(t),set(setptr),get(nullptr) {}
        Attribute(Data*&t,const EventArray<void(Data)> &e,Data* (*getptr)(Data* const&) = nullptr,bool (*setptr)(Data*&,Data* const&) = nullptr) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(Data*&t,const EventArray<void(Data)> &e,bool (*setptr)(Data*&,const Data*)) :
        Value(t),EventList(e),set(setptr),get(nullptr) {}
        Attribute(const Attribute<Data*&>&) = delete;
        operator Data*() {
            Data *data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        Data* Get() {
            Data *data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        void Set(Data* value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return; 
                }
            }
            else Value = value;
            Mutex.unlock();
            EventList(value);
        }
        Attribute<Data*&> &operator=(Data* value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return *this; 
                }
            }
            else Value = value;
            Mutex.unlock();
            EventList(value);
            return *this;
        }
        Attribute<Data*&> &operator=(const Attribute<Data*&>&) = delete;
    private:
        Data* &Value;
        std::mutex Mutex;
    };
    template<typename Data>
    class Attribute<const Data*>{
        public:
        EventArray<void(const Data*)> EventList;
        Data (*get)(const Data&);
        bool (*set)(Data&,const Data&);
        Attribute(Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) : get(getptr),set(setptr) {}
        Attribute(bool (*setptr)(Data&,const Data&)) : set(setptr),get(nullptr) {}
        Attribute(const Data *t,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(const Data *t,bool (*setptr)(Data&,const Data&)) :
        Value(t),set(setptr),get(nullptr) {}
        Attribute(const EventArray<void(Data)> &e,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        EventList(e),get(getptr),set(setptr) {}
        Attribute(const EventArray<void(Data)> &e,bool (*setptr)(Data&,const Data&)) :
        EventList(e),set(setptr),get(nullptr) {}
        Attribute(const Data *t,const EventArray<void(Data)> &e,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(const Data *t,const EventArray<void(Data)> &e,bool (*setptr)(Data&,const Data&)) :
        Value(t),EventList(e),set(setptr),get(nullptr) {}
        Attribute(const Attribute<const Data*>&) = delete;
        operator Data*() const {
            const Data *data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        const Data *Get() {
            const Data *data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        void Set(const Data *value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return; 
                }
            }
            else Value = value;
            Mutex.unlock();
            EventList(value);
        }
        Attribute<const Data*> &operator=(const Data *value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return *this; 
                }
            }
            else Value = value;
            EventList(Value);
            Mutex.unlock();
            return *this;
        }
        Attribute<const Data*> &operator=(const Attribute<const Data*>&) = delete;
    private:
        const Data *Value;
        std::mutex Mutex;
    };
    template<typename Data>
    class Attribute<Data* const&>{
        public:
        EventArray<void(Data)> EventList;
        Data (*get)(const Data&);
        bool (*set)(Data&,const Data&);
        Attribute(Data* const &t,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(Data* const &t,bool (*setptr)(Data&,const Data&)) :
        Value(t),set(setptr),get(nullptr) {}
        Attribute(Data* const &t,const EventArray<void(Data)> &e,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(Data* const &t,const EventArray<void(Data)> &e,bool (*setptr)(Data&,const Data&)) :
        Value(t),EventList(e),set(setptr),get(nullptr) {}
        Attribute(const Attribute<Data* const&>&) = delete;
        operator Data*() const {
            const Data *data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        Data* Get() const {
            const Data *data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        void Set(const Data *value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return; 
                }
            }
            else Value = value;
            Mutex.unlock();
            EventList(value);
        }
        Attribute<Data* const&> &operator=(const Data *value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return *this; 
                }
            }
            else Value = value;
            Mutex.unlock();
            EventList(value);
            return *this;
        }
        Attribute<Data* const&> &operator=(const Attribute<Data* const&>&) = delete;
    private:
        Data* const &Value;
        std::mutex Mutex;
    };
}
#endif