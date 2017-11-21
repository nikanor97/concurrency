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
#include <vector>

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
    Robot(const std::size_t n_foots): semaphores_(n_foots), num_foots_(n_foots){
//        for (std::size_t i = 0; i < num_foots; i++){
//            sem.push_back(new semaphore);
//        }
        if (num_foots_ > 0)
            semaphores_[0].Signal();
//            (*sem[0]).set(1);
    }

    void Step(const std::size_t foot) {
//        (*sem[foot]).wait();
        semaphores_[foot].Wait();
        std::cout << "foot " << foot << std::endl;
//        (*sem[(foot + 1) % num_foots]).signal();
        semaphores_[(foot + 1) % num_foots_].Signal();
    }

    ~Robot(){
        for (std::size_t i = 0; i < num_foots_; i++) {
//            delete sem[i];
            semaphores_[i];
        }
    }

private:
    std::vector<semaphore> semaphores_;
    const std::size_t num_foots_;
};