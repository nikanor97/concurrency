// STEPPING ROBOT - CONDITION VARIABLES
// Created by nikita on 20.03.17.
//

#ifndef INC_2_SYNC_PRIMITIVES_STEPPING_ROBOT_H
#define INC_2_SYNC_PRIMITIVES_STEPPING_ROBOT_H

#endif //INC_2_SYNC_PRIMITIVES_STEPPING_ROBOT_H

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <atomic>

class Robot {
public:
    void StepLeft() {
        std::unique_lock<std::mutex> lk(m_);
        while (!ready_) cv_.wait(lk);
        std::cout << "left" << std::endl;
        ready = false;
        cv_.notify_all();
    }

    void StepRight() {
        std::unique_lock<std::mutex> lk(m_);
        while (ready_) cv_.wait(lk);
        std::cout << "right" << std::endl;
        ready_ = true;
        cv_.notify_all();
    }
private:
    std::condition_variable cv_;
    std::mutex m_;
    bool ready_ = true;
};