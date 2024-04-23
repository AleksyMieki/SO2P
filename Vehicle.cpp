#include "Vehicle.h"
#include "config.h"
#include <chrono>
#include <memory>
#include <thread>
#include <iostream>
#include <random>

Vehicle::Vehicle(int x, int y, Type type, std::shared_ptr<TrafficLight> trafficLight) : _x(x), _y(y), _type(type), _running(true), _direction(0), _trafficLight(trafficLight)
{
    _colorR = rand() % 256;
    _colorG = rand() % 256;
    _colorB = rand() % 256;
    _speed = (rand() % 5 ) + 1;
}

void Vehicle::move() {

while(_running)
{   
    //petla goracego czekania wyeliminowac 
    //zmienna warunkowa

    if(_type == Type::HORIZONTAL)
    {
     if ( (_x == 350 && _y == 0) || (_x == 450 && _y == config::WINDOW_HEIGHT - config::VEHICLE_HEIGHT )) {
            _trafficLight->waitForGreen();
        }
        
            switch(_direction) {
                case 0: 
                    _x += 1;
                    if (_x >= config::WINDOW_WIDTH - config::VEHICLE_WIDTH) {
                        _x = config::WINDOW_WIDTH - config::VEHICLE_WIDTH; 
                        _direction = 1; 
                    }
                    break;
                case 1:
                    _y += 1;
                    if (_y >= config::WINDOW_HEIGHT - config::VEHICLE_HEIGHT) {
                        _y = config::WINDOW_HEIGHT - config::VEHICLE_HEIGHT; 
                        _direction = 2; 
                    }
                    break;
                case 2: 
                    _x -= 1;
                    if (_x <= 0) {
                        _x = 0; 
                        _direction = 3; 
                    }
                    break;
                case 3: 
                    _y -= 1;
                    if (_y <= 0) {
                        _y = 0; 
                        _direction = 0; 
                    }
                    break;
            }
    }
    
    if(_type == Type::VERTICAL)
    {
        if(_y == -30 || _y == 540)
        {
            _trafficLight->waitForRed();
        }
        _y+=1;
         
        if( _y > config::WINDOW_HEIGHT)
        {
            _running = false;
        }
    }
        std::this_thread::sleep_for(std::chrono::milliseconds(_speed * 3));

    }
}


void Vehicle::draw(SDL_Renderer* renderer) const {
    
    SDL_Rect rect = {_x, _y, config::VEHICLE_WIDTH, config::VEHICLE_HEIGHT};
    SDL_SetRenderDrawColor(renderer, _colorR, _colorG, _colorB, 255);
    SDL_RenderFillRect(renderer, &rect);

}

void Vehicle::stop() {
    _running = false;
}

bool Vehicle::isRunning() const {
    return _running;
}