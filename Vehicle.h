#ifndef VEHICLE_H
#define VEHICLE_H

#include <memory>
#include <atomic>
#include <SDL.h>
#include <mutex>
#include "TrafficLight.h"
enum Type { HORIZONTAL, VERTICAL };

class Vehicle {
private:

    int _x, _y;
    bool _running;
    int _direction;
    std::shared_ptr<TrafficLight> _trafficLight;  
    int _speed;
    Type _type;
    std::mutex _mtx;
    std::condition_variable _cv;
    unsigned char _colorR, _colorG, _colorB;

public:
    Vehicle(int x, int y, Type type, std::shared_ptr<TrafficLight> trafficLight);
    
    void move();
    void draw(SDL_Renderer* renderer) const;
    void stop();
    bool isRunning() const;
};

#endif
