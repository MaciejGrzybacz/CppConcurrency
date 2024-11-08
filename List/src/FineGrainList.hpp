#pragma once

#include <mutex>
#include <concepts>

template<Comparable T>
struct FineNode {
    T val;
    FineNode* next;
    std::mutex mtx;

    explicit FineNode(T value) : val(value), next(nullptr) {}
    FineNode(T value, FineNode* next) : val(value), next(next) {}

    ~FineNode() {
        FineNode* temp = next;
        next = nullptr;
        delete temp;
    }
};

template<Comparable T>
class FineList {
public:
    FineList() : head(new FineNode<T>(0)), tail(new FineNode<T>(0)) {
        head->next = tail;
    }

    ~FineList() {
        FineNode<T>* current = head;
        while (current != tail) {
            FineNode<T>* toDelete = current;
            current = current->next;
            toDelete->next = nullptr;
            delete toDelete;
        }
        delete tail;
    }

    bool add(T val) {
        FineNode<T>* new_node = new FineNode(val, tail);
        FineNode<T>* current = head;
        FineNode<T>* next = current->next;

        current->mtx.lock();
        next->mtx.lock();

        while (next != tail) {
            current->mtx.unlock();
            current = next;
            next = next->next;
            next->mtx.lock();
        }

        current->next = new_node;
        
        current->mtx.unlock();
        next->mtx.unlock();

        return true;
    }

    bool remove(T val) {
        FineNode<T>* prev = head;
        FineNode<T>* current = head->next;

        prev->mtx.lock();
        current->mtx.lock();

        while (current != tail && current->val != val) {
            prev->mtx.unlock(); 
            prev = current;
            current = current->next;
            current->mtx.lock();
        }

        if (current == tail) { 
            prev->mtx.unlock();
            current->mtx.unlock();
            return false;
        }
        
        prev->next = current->next;
        current->mtx.unlock();
        prev->mtx.unlock();
        delete current;

        return true;
    }

    bool contains(T val) {
        FineNode<T>* current = head->next;
        current->mtx.lock();

        while (current != tail) {
            if (current->val == val) {
                current->mtx.unlock();
                return true;
            }

            FineNode<T>* next = current->next;
            next->mtx.lock();
            current->mtx.unlock();
            current = next;
        }

        current->mtx.unlock(); 
        return false;
    }

private:
    FineNode<T>* head;
    FineNode<T>* tail;
};
