//
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
        std::unique_lock<std::mutex> lk(m);
        while (!ready) cv.wait(lk);
        std::cout << "left" << std::endl;
        ready = false;
        cv.notify_all();
    }

    void StepRight() {
        std::unique_lock<std::mutex> lk(m);
        while (ready) cv.wait(lk);
        std::cout << "right" << std::endl;
        ready = true;
        cv.notify_all();
    }
private:
    std::condition_variable cv;
    std::mutex m;
    bool ready = true;
};