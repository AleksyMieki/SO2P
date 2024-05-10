#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>

class TrafficLight {
public:
    enum State { RED, GREEN, YELLOW };

private:

    bool _running = true;
    State _state;

    std::mutex mtx;
    std::condition_variable cv;
    State current_state;
    int sleep_time;
public:

    TrafficLight(int st) : _state(RED), sleep_time(st) {} 

    void automaticToggle() {
        while (_running) {
            
            std::this_thread::sleep_for(std::chrono::seconds(sleep_time)); 
            changeState(GREEN);
            std::this_thread::sleep_for(std::chrono::seconds(sleep_time)); 
            changeState(RED);
        
        }
    }

    void changeState(State new_state) {
        std::lock_guard<std::mutex> lock(mtx);
        current_state = new_state;
        if(current_state == State::GREEN)
        cv.notify_all();
        if(current_state == State::RED)
        cv.notify_all();
    }

    State getState()  {
        std::unique_lock<std::mutex> lock(mtx);
        return current_state;
    }

    bool isGreen() const {
        return current_state == GREEN;
    }

    bool isRed() const {
        return current_state == RED;
    }

    //swiatla rozna czestotliwosc przelaczania - oba swiatla na osobnych watkach
    // wiecej pojazdow na skrzyzowaniu 
    // pojazdy wjezdzaja na skrzyzowanie jeden po drugim, w zaleznosci od tego kiedy dojechaly 
    // na skrzyzowanie, uzaleznione od czasu 
    void waitForGreen() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock,  [this] { return this->isGreen(); });
    }

    void waitForRed() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock,  [this] { return this->isRed(); });
    }

    void end()
    {
        _running = false;
    }

};

#endif 
