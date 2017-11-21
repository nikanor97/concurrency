//
// Created by nikita on 26.05.17.
//
#pragma once

#ifndef LF_STACK_SOLUTION_H
#define LF_STACK_SOLUTION_H

#endif //LF_STACK_SOLUTION_H

#include <atomic>
#include <thread>

///////////////////////////////////////////////////////////////////////

template <typename T>
class LockFreeStack {
    struct Node {
        T element;
        Node* next;
        Node(T elem){
            element = elem;
            next = nullptr;
        }
    };

    struct Trash {
        Node* element;
        Trash* next;
        Trash(Node* vertix) {
            element = vertix;
            next = nullptr;
        }
    };

public:
    explicit LockFreeStack() {
    }

    ~LockFreeStack() {
        Node* nd = top_;
        while (nd->next != nullptr){
            top_.store(top_.load()->next);
            delete(nd);
            nd = top_;
        }
        if (nd != nullptr) {
            delete(nd);
        }
        Trash* tr = trash_top_;
        while (tr->next != nullptr){
            trash_top_.store(trash_top_.load()->next);
            delete(tr->element);
            delete(tr);
            tr = trash_top_;
        }
        if (tr != nullptr) {
            delete(tr->element);
            delete(tr);
        }
    }

    void TrashPush(Node* element) {
        Trash* new_top = new Trash(element);
        Trash* curr_top = trash_top_.load();
        new_top->next = curr_top;
        while (!(trash_top_.compare_exchange_strong(curr_top, new_top))){
            new_top->next = curr_top;
        }
    }

    void Push(T element) {
        Node* new_top = new Node(element);
        Node* curr_top = top_.load();
        new_top->next = curr_top;
        while (!(top_.compare_exchange_strong(curr_top, new_top))){
                new_top->next = curr_top;
        }
    }

    bool Pop(T& element) {
        Node* curr_top = top_.load();
        while (true) {
            if (!curr_top) {
                return false;
            }
            if (top_.compare_exchange_strong(curr_top, curr_top->next)) {
                element = curr_top->element;
                TrashPush(curr_top);
                return true;
            }
        }
    }

private:
    std::atomic<Node*> top_{nullptr};
    std::atomic<Trash*> trash_top_{nullptr};
};

///////////////////////////////////////////////////////////////////////

template <typename T>
using ConcurrentStack = LockFreeStack<T>;

///////////////////////////////////////////////////////////////////////