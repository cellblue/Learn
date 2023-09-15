#ifndef LFQ_H
#define LFQ_H

#include <iostream>
#include <atomic>
#include <thread>

template <typename T>
class LockFreeQueue{
public:
    LockFreeQueue() : head(new Node(T())), tail(head.load()), counter(0) {}
    // LockFreeQueue(std::atomic<Node*> head_, std::atomic<Node*> tail_) : head(head_),tail(tail_){};

    ~LockFreeQueue(){
        Node *cur = head.load(),*pre = nullptr;
        while(cur != nullptr){
            pre = cur;
            cur = cur->next.load();
            delete pre;
        }
    }

    size_t size(){
        return counter.load();
    }

    void push(const T& newData){
        Node* newNode = new Node(newData);
        newNode->next = nullptr;
        
        Node* oldTail = tail.load(std::memory_order_relaxed);
        while (!tail.compare_exchange_weak(oldTail, newNode)) {
            oldTail = tail.load(std::memory_order_relaxed);
        }
        counter.fetch_add(1, std::memory_order_relaxed);
    }

    void pop(){
        
        Node* oldHead = head.load(std::memory_order_relaxed);
        while(oldHead && !head.compare_exchange_weak(oldHead, oldHead->next));
        counter.fetch_sub(1,std::memory_order_relaxed);
        if(oldHead){
            delete oldHead;
        }
    }

private:
    struct Node{
        T data;
        std::atomic<Node*> next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    std::atomic<Node*> head, tail;
    std::atomic<size_t> counter;
};

#endif