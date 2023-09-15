#include <benchmark/benchmark.h>
#include <queue>
#include <mutex>
#include <thread>
#include "lockFreeQueue.hpp"


void lock_free(){
    LockFreeQueue<int> lock_free_que;
    std::vector<std::thread> producer1_threads_(15);
    auto producer1 = [&](){
        for(int i = 1;i <= 100;i++){
            lock_free_que.push(10);
        }
    };
    for (auto& thread : producer1_threads_){
        thread = std::thread(producer1);
    }
    for (auto& thread : producer1_threads_){
        thread.join();
    }

}
void lock(){
    std::queue<int> lock_que;
    std::mutex mtx;
    std::vector<std::thread> producer2_threads_(15);
    

    auto producer2 = [&](){
        for(int i = 1;i <= 100;i++){
            std::lock_guard<std::mutex> lk(mtx);
            lock_que.push(10);
        }
    };

    for (auto& thread : producer2_threads_){
        thread = std::thread(producer2);
    }
    
    for (auto& thread : producer2_threads_){
        thread.join();
    }

}

static void use_lock_free_queue(benchmark::State& state){
    for(auto _ : state){
        lock_free();
    }
}
BENCHMARK(use_lock_free_queue);
 

static void use_std_queue(benchmark::State& state){
    for(auto _ : state){
        lock();
    }
}
BENCHMARK(use_std_queue);
 
BENCHMARK_MAIN();