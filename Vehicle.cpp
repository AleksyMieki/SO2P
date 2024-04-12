#include "Vehicle.h"
#include "config.h"
#include <chrono>
#include <memory>
#include <thread>
#include <random>
Vehicle::Vehicle(int x, int y, Type type, std::shared_ptr<TrafficLight> trafficLight) : _x(x), _y(y), _type(type), _running(true), _direction(0), _trafficLight(trafficLight)
{
    _colorR = rand() % 256;
    _colorG = rand() % 256;
    _colorB = rand() % 256;
    _speed = rand() % 10;
}

void Vehicle::move() {

while(_running)
{
   if(_x > 400 || _x < 300 )
   {
    if (_type == HORIZONTAL) {
    switch(_direction) {
        case 0: // prawo
            _x += _speed;
            if (_x >= config::WINDOW_WIDTH - config::VEHICLE_WIDTH) {
                _x = config::WINDOW_WIDTH - config::VEHICLE_WIDTH; 
                _direction = 1; 
            }
            break;
        case 1: // dół
            _y += _speed;
            if (_y >= config::WINDOW_HEIGHT - config::VEHICLE_HEIGHT) {
                _y = config::WINDOW_HEIGHT - config::VEHICLE_HEIGHT; 
                _direction = 2; 
            }
            break;
        case 2: // lewo
            _x -= _speed;
            if (_x <= 0) {
                _x = 0; 
                _direction = 3; 
            }
            break;
        case 3: // góra
            _y -= _speed;
            if (_y <= 0) {
                _y = 0; 
                _direction = 0; 
            }
            break;
        }
    }else
    {
        _y += _speed;      
        if (_y >= config::WINDOW_HEIGHT) {
            _running = false;
        }
    }
   }
   else if( _trafficLight->getState() == TrafficLight::GREEN )
   {
    if (_type == HORIZONTAL) {
    switch(_direction) {
        case 0: // prawo
            _x += _speed;
            if (_x >= config::WINDOW_WIDTH - config::VEHICLE_WIDTH) {
                _x = config::WINDOW_WIDTH - config::VEHICLE_WIDTH; 
                _direction = 1; 
            }
            break;
        case 1: // dół
            _y += _speed;
            if (_y >= config::WINDOW_HEIGHT - config::VEHICLE_HEIGHT) {
                _y = config::WINDOW_HEIGHT - config::VEHICLE_HEIGHT; 
                _direction = 2; 
            }
            break;
        case 2: // lewo
            _x -= _speed;
            if (_x <= 0) {
                _x = 0; 
                _direction = 3; 
            }
            break;
        case 3: // góra
            _y -= _speed;
            if (_y <= 0) {
                _y = 0; 
                _direction = 0; 
            }
            break;
        }
    }else
    {
        _y += _speed;      
        if (_y >= config::WINDOW_HEIGHT) {
            _running = false;
        }
    }
   }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

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
