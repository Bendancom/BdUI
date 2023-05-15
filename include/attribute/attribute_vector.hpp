#ifndef BDUI_ATTRIBUTE_VECTOR
#define BDUI_ATTRIBUTE_VECTOR

#include <shared_mutex>
#include <vector>
#include "delegate.hpp"
#include <event.hpp>

namespace BdUI {
    template<typename Data>
    class AttributeVector {
    private:
        std::vector<Data> Vector;
        std::shared_mutex Mutex;
    public:
        Event<void(const std::vector<Data>)>* ChangedEvent = nullptr;
        AttributeVector() {}
        AttributeVector(const std::vector<Data>& v) : Vector(v) {}
        AttributeVector(const AttributeVector<Data>&) = delete;
        ~AttributeVector() {
            delete ChangedEvent;
        }
        operator std::vector<Data>() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Vector;
        }
        bool empty() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Vector.empty();
        }
        Data at(int n) {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            if (!Vector.empty()) return Vector[n];
            else throw error::Class::Uninitialize();
        }
        size_t size() {
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Vector.size();
        }

        bool Isfind(Data data) {
            std::lock_guard<std::shared_mutex> lock(Mutex);
            if (std::find(Vector.begin(), Vector.end(), data) != Vector.end()) return true;
            return false;
        }

        bool insert(Data value,std::size_t pos) {
            std::lock_guard<std::shared_mutex> lock(Mutex);
            if (pos < Vector.size()) Vector.insert(Vector.cbegin() + pos, value);
            else return false;
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            else return false;
        }
        bool insert(Data value,int number,std::size_t pos) {
            std::lock_guard<std::shared_mutex> lock(Mutex);
            if (pos < Vector.size()) Vector.insert(Vector.cbegin() + pos, number, value);
            else return false;
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            return true;
        }
        bool insert(std::initializer_list<Data> value,std::size_t pos) {
            std::lock_guard<std::shared_mutex> lock(Mutex);
            if (pos < Vector.size()) Vector.insert(Vector.cbegin() + pos, value);
            else return false;
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            return true;
        }
        bool emplace(Data value,std::size_t pos){
            std::lock_guard<std::shared_mutex> lock(Mutex);
            if (pos < Vector.size()) Vector.emplace(Vector.begin() + pos, value);
            else return false;
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            return true;
        }
        void push_back(Data value) {
            std::lock_guard<std::shared_mutex> lock(Mutex);
            Vector.push_back(value);
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
        }
        bool Change(Data value,std::size_t n){
            std::lock_guard<std::shared_mutex> lock(Mutex);
            if(Vector.size() > n) Vector[n] = value;
            else return false;
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            return true;
        }
        bool erase(std::size_t pos) {
            std::lock_guard<std::shared_mutex> lock(Mutex);
            if (pos < Vector.size()) Vector.erase(Vector.begin() + pos);
            else return false;
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            return true;
        }
        bool erase(Data data) {
            std::lock_guard<std::shared_mutex> lock(Mutex);
            auto iter = std::find(Vector.begin(), Vector.end(), data);
            if (iter != Vector.end()) Vector.erase(iter);
            else return false;
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            return true;
        }

        AttributeVector<Data>& operator=(std::vector<Data>&& vector) {
            Mutex.lock();
            Vector = vector;
            if (ChangedEvent != nullptr) ChangedEvent->operator()(Vector);
            Mutex.unlock();
            return *this;
        }
        Data operator[](std::size_t n){
            std::shared_lock<std::shared_mutex> lock(Mutex);
            return Vector[n];
        }
        AttributeVector<Data>& operator=(const AttributeVector<Data>&) = delete;
    };
}
#endif
