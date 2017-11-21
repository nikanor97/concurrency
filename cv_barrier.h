//
// Created by nikita on 20.03.17.
//

#include <cstdio>
#include <condition_variable>

#ifndef INC_2_SYNC_PRIMITIVES_CYCLIC_BARRIER_H
#define INC_2_SYNC_PRIMITIVES_CYCLIC_BARRIER_H

#endif //INC_2_SYNC_PRIMITIVES_CYCLIC_BARRIER_H

template <class ConditionVariable = std::condition_variable>
//typedef std::condition_variable ConditionVariable;
class CyclicBarrier {
public:
    CyclicBarrier(std::size_t num_threads);

    void Pass();
private:
    //ConditionVariable t;
    std::condition_variable cv;
};

void CyclicBarrier::CyclicBarrier(std::size_t num_threads) {
    cv
}

void CyclicBarrier::Pass() {

}