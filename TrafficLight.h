#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>

class TrafficLight {
public:
    enum State { RED, GREEN };

private:
    bool _running = true;
    State _state;
    std::mutex _mutex;
    std::condition_variable _condition;

public:
    TrafficLight() : _state(RED) {}  // Default state is RED

    // Automatically toggles the light state between RED and GREEN
    void automaticToggle() {
        while (_running) {
            
            {
                std::lock_guard<std::mutex> lock(_mutex);
                if (_state == RED) {
                    _state = GREEN;
                } else {
                    _state = RED;
                }
            
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));  // Wait for 5 seconds
        }
    }

    // Returns the current state of the light
    State getState() const {
        return _state;
    }

    void end()
    {
        _running = false;
    }
    // Wait function for vehicles to use to wait for GREEN light
    void waitForGreen() {
        std::unique_lock<std::mutex> lock(_mutex);
        _condition.wait(lock, [this]{ return _state == GREEN; });
    }
};

#endif // TRAFFICLIGHT_H
