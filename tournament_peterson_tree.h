//
// Created by nikita on 06.03.17.
//
#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <atomic>
#include <thread>
#include <cmath>
using namespace std;

class PetersonMutex {
public:
    array<atomic<bool>, 2> want;
    atomic<size_t> vict;

    PetersonMutex() {
        want[0].store(false);
        want[1].store(false);
    }
    void lock(size_t current_thread) {
        want[current_thread].store(true);
        vict.store(current_thread);
        while (want[1 - current_thread].load() && vict.load() == current_thread) {
            this_thread::yield();
        }
    }
    void unlock(size_t current_thread) {
        want[current_thread].store(false);
    }
};

class TreeMutex {
private:
    size_t height;
    size_t num_ver;
    vector<PetersonMutex> tree;
public:
    TreeMutex(std::size_t n_threads): height(ceil(log2(n_threads))), num_ver((2 << height) -1), tree(vector<PetersonMutex> (num_ver)) {}

    void lock(std::size_t current_thread) {
        for (size_t i = num_ver + current_thread - 1; i > 0; i = (i - 1) >> 1)
            tree[(i - 1) >> 1].lock(i % 2);
    }
    void unlock(std::size_t current_thread) {
        vector<bool> order;
        for (size_t i = num_ver + current_thread - 1; i > 0; i = (i - 1) >> 1)
            order.push_back(i % 2);
        reverse(order.begin(), order.end());

        int ind = 0;
        for (size_t i = 0; i < order.size(); i++) {
            tree[ind].unlock(order[i]);
            ind = (ind << 1) + 1 + (int)(!order[i]);
        }
    }
};
