// STEPPING ROBOT - SEMAPHORES
// Created by nikita on 20.03.17.
//

#ifndef INC_2_SYNC_PRIMITIVES_STEPPING_ROBOT_H
#define INC_2_SYNC_PRIMITIVES_STEPPING_ROBOT_H

#endif //INC_2_SYNC_PRIMITIVES_STEPPING_ROBOT_H

#include <condition_variable>
#include <semaphore.h>
#include <iostream>
#include <mutex>
#include <atomic>

class Semaphore {
public:
    Semaphore(){
        cnt_ = 0;
    };
    void Set(int set_cnt){
        cnt_ = set_cnt;
    }
    void Wait(){
        std::unique_lock<std::mutex> lk(m_);
        while (cnt_ == 0) {
            cv_.wait(lk);
        }
        cnt_--;
    };
    void Signal(){
        std::unique_lock<std::mutex> lk(m_);
        cnt_++;
        cv_.notify_one();
    };
private:
    std::mutex m_;
    std::condition_variable cv_;
    int cnt_;
};

class Robot {
public:
    void StepLeft() {
        sem_left_.Wait();
        std::cout << "left" << std::endl;
        sem_right_.Signal();
    }

    void StepRight() {
        sem_right_.Wait();
        std::cout << "right" << std::endl;
        sem_left_.Signal();
    }
private:
    semaphore sem_left_{1}, sem_right_{0};
};