#pragma once

#include <mutex>
#include <concepts>

template<typename T>
concept Comparable = requires(T a, T b) {
    { a == b } -> std::convertible_to<bool>;
};

template<Comparable T>
struct CoarseNode {
    T val;
    CoarseNode *next;

    explicit CoarseNode(T value) : val(value), next(nullptr) {};

    CoarseNode(T value, CoarseNode *next) : val(value), next(next) {};

    ~CoarseNode() {
        CoarseNode* temp = next;
        next = nullptr;  
        delete temp;
    };
};

template<Comparable T>
class CoarseList {
public:
    CoarseList() : head(new CoarseNode<T>(0)), tail(new CoarseNode<T>(0)) { 
        head->next = tail; 
    }

    ~CoarseList() {
        std::lock_guard<std::mutex> lock(mtx);
        CoarseNode<T>* current = head;

        while (current != tail) {
            CoarseNode<T>* toDelete = current;
            current = current->next;
            toDelete->next = nullptr; 
            delete toDelete;
        }
        delete tail;
    }

    bool add(T val) {
        std::lock_guard<std::mutex> lock(mtx);
        CoarseNode<T> *current = head;

        while (current->next != tail) {
            current = current->next;
        }

        CoarseNode<T>* new_node = new CoarseNode(val, tail);
        current->next = new_node;

        return true;
    }

    bool remove(T val) {
        std::lock_guard<std::mutex> lock(mtx);
        CoarseNode<T>* prev = head;
        CoarseNode<T>* curr = head->next;
        CoarseNode<T>* next = curr->next;

        while (curr != tail && curr->val != val) {
            prev = curr;
            curr = next;
            next = next->next;
        }

        if (curr == tail) {
            return false;
        }

        prev->next = next;
        curr->next = nullptr;  
        
        delete curr;

        return true;
    }

    bool contains(T val) {
        std::lock_guard<std::mutex> lock(mtx);

        CoarseNode<T> *current = head->next;

        while (current != tail) {
            if (current->val == val) {
                return true;
            }
            current = current->next;
        }

        return false;  
    }


private:
    CoarseNode<T> *head;
    CoarseNode<T> *tail;
    std::mutex mtx;
};
