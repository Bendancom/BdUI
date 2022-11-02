#ifndef BDUI_ATTRIBUTE_VECTOR
#define BDUI_ATTRIBUTE_VECTOR

#include <shared_mutex>
#include <vector>
#include <delegate.hpp>
#include <event.hpp>

namespace BdUI {
    template<typename Date>
    class AttributeVector {
    private:
        std::vector<Date> Vector;
        std::shared_mutex Mutex;
    public:
        EventArray<void(const std::vector<Date>)>* ChangedEvent = nullptr;
        AttributeVector() {}
        AttributeVector(const std::vector<Date>& v) : Vector(v) {}
        AttributeVector(const AttributeVector<Date>&) = delete;
        ~AttributeVector() {
            delete ChangedEvent;
        }
        operator std::vector<Date>{
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Vector;
        }
        bool empty() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Vector.empty();
        }
        Date at(int n) {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (!Vector.empty()) return Vector[n];
            else throw error::Class::Uninitialize();
        }
        std::vector<Date>::size_type size() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Vector.size();
        }

        void insert(Date&& value,std::size_t pos) {
            Mutex.lock();
            Vector.insert(Vector.cbegin()+pos,value);
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            Mutex.unlock();
        }
        void insert(Date&& value,int number,std::size_t pos) {
            Mutex.lock();
            Vector.insert(Vector.cbegin()+pos,n,value);
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            Mutex.unlock();
        }
        void insert(std::initializer_list<Date>&& value,std::size_t pos) {
            Mutex.lock();
            Vector.insert(Vector.cbegin()+pos,value);
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            Mutex.unlock();
        }
        void emplace(Date&& value,std::size_t pos){
            Mutex.lock();
            Vector.emplace(Vector.begin() + pos,value);
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            Mutex.unlock();
        }
        void push_back(Date&& value) {
            Mutex.lock();
            Vector.push_back(value);
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            Mutex.unlock();
            return false;
        }
        void Change(Date&& value,std::size_t n){
            Mutex.lock();
            if(Vector.size() > n) Vector[n] = value;
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            else throw error::Function::ParamError();
        }

        AttributeVector<Date>& operator=(std::vector<Date>&& vector) {
            Mutex.lock();
            Vector = vector;
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            Mutex.unlock();
            return *this;
        }
        Date operator[](std::size_t n){
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Vector[n];
        }
        AttributeVector<Date>& operator=(const AttributeVector<Date>&) = delete;
    };
}
#endif
