//
// Created by nikita on 26.03.17.
//

#ifndef INC_3_W_BLOCKING_QUEUE_H
#define INC_3_W_BLOCKING_QUEUE_H

#endif //INC_3_W_BLOCKING_QUEUE_H

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <atomic>
#include <deque>
#include <stdexcept>

template <class T, class Container = std::deque<T>>
class BlockingQueue {
public:
    explicit BlockingQueue(const size_t& cap){
        capacity_ = cap;
    }
    void Put(T&& element){
        std::unique_lock<std::mutex> lk(m_);
        if (!power_){
            throw std::runtime_error("Queue is turned off");
        }
        while (q_.size() == capacity_){
            cvput_.wait(lk);
        }
        q_.push_back(std::move(element));
        cvget_.notify_all();
    }
    bool Get(T& result){
        std::unique_lock<std::mutex> lk(m_);
        if (!power_ && q_.size() == 0) return false;
        while (q_.size() == 0)
            cvget_.wait(lk);
        cvput_.notify_all();
        result = std::move(q_.front());
        q_.pop_front();
        return true;
    }
    void Shutdown(){
        std::unique_lock<std::mutex> lk(m_);
        power_ = false;
        cvput_.notify_all();
        cvget_.notify_all();
    }
private:
    Container q_;
    std::size_t capacity_;
    std::condition_variable cvput_, cvget_;
    std::mutex m_;
    bool power_ = true;
};
