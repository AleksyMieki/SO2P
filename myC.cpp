#include <SDL.h>
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <random>
#include <memory>
#include "config.h"
#include "Vehicle.h"
#include "TrafficLight.h"

void spawnVehiclesHorizontal(std::vector<std::shared_ptr<Vehicle>>& vehicles, std::vector<std::thread>& threads, std::shared_ptr<TrafficLight> tf);
void spawnVehiclesVertical(std::vector<std::shared_ptr<Vehicle>>& vehicles, std::vector<std::thread>& threads, std::shared_ptr<TrafficLight> tf);
void deleteVehicles(std::vector<std::shared_ptr<Vehicle>>& vehicles, std::vector<std::thread>& threads);
void drawTrack(SDL_Renderer* renderer);
void drawLights(std::shared_ptr<TrafficLight> tl, SDL_Renderer* renderer );

int main(int argc, char* argv[]) {

    srand(time(nullptr)); 
  
    auto lastToggleTime = std::chrono::high_resolution_clock::now(); 
    const std::chrono::milliseconds toggleInterval(3000);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Vehicle Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, config::WINDOW_WIDTH, config::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::vector<std::shared_ptr<Vehicle>> vehicles;
    std::vector<std::thread> vehicleThreads;

    std::shared_ptr<TrafficLight> tf = std::make_shared<TrafficLight>();
    std::thread tft = std::thread(&TrafficLight::automaticToggle, tf);

    bool spawnHorizontal = true;
    bool quit = false;

    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

        }

        for (const auto& vehicle : vehicles) {
            if (vehicle->isRunning()) {
                vehicle->draw(renderer);
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastToggleTime);
        
        auto randN = rand() % 10;
        drawLights(tf,renderer);
        
        if (elapsedTime >= toggleInterval && randN > 3) {
            lastToggleTime = currentTime; 
            if(spawnHorizontal)
            {
                spawnVehiclesHorizontal(vehicles,vehicleThreads, tf);
            }
            else
            {
                spawnVehiclesVertical(vehicles,vehicleThreads,tf);
            }

            spawnHorizontal=!spawnHorizontal;
        }
        SDL_RenderPresent(renderer);

        drawTrack(renderer);

        deleteVehicles(vehicles,vehicleThreads);

        SDL_Delay(20);
    }

    for (auto& vehicle : vehicles) {
        vehicle->stop();
    }
    tf->end();
    if(tft.joinable())
    {
        tft.join();
    }

    for (auto& thread : vehicleThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// TODO kończenie na spacji
// TODO światła na skrzyżwoaniach - co określony czas, 

void spawnVehiclesHorizontal(std::vector<std::shared_ptr<Vehicle>>& vehicles, std::vector<std::thread>& threads, std::shared_ptr<TrafficLight> tf) {
 
    std::shared_ptr<Vehicle> vehicle;

    vehicle = std::make_shared<Vehicle>(0, 0, Type::HORIZONTAL, tf);
        
    threads.push_back(std::thread(&Vehicle::move, vehicle));
    vehicles.push_back(vehicle);
        
}

void drawLights(std::shared_ptr<TrafficLight> tl, SDL_Renderer* renderer )
{
    if(tl->getState() == TrafficLight::GREEN)
    {
    SDL_Rect rect = {100, 100, 200, 200};
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    }
    else{
    SDL_Rect rect = {100, 100, 200, 200};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    }
}
void spawnVehiclesVertical(std::vector<std::shared_ptr<Vehicle>>& vehicles, std::vector<std::thread>& threads, std::shared_ptr<TrafficLight> tf) {
 
    std::shared_ptr<Vehicle> vehicle;
         
    vehicle = std::make_shared<Vehicle>(400, 0, Type::VERTICAL, tf);
        
    threads.push_back(std::thread(&Vehicle::move, vehicle));
    vehicles.push_back(vehicle);
        
}

void deleteVehicles(std::vector<std::shared_ptr<Vehicle>>& vehicles, std::vector<std::thread>& vehicleThreads)
{
    auto v = vehicles.begin();
    auto vth = vehicleThreads.begin();
    while (v != vehicles.end() && vth != vehicleThreads.end()) {
            if (!(*v)->isRunning()) {
                if(vth->joinable())
                {
                    vth->join();
                    v = vehicles.erase(v);
                    vth = vehicleThreads.erase(vth);
                }
            } else {
                v++,
                vth++;
            }
    }
}

void drawTrack(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear screen with black
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255); 
    SDL_Rect horizontalTrack = {0, 0, config::WINDOW_WIDTH, config::VEHICLE_HEIGHT}; 
    SDL_Rect horizontalTrack1 = {0, config::WINDOW_HEIGHT-config::VEHICLE_HEIGHT, config::WINDOW_WIDTH, config::VEHICLE_HEIGHT}; 

        SDL_RenderFillRect(renderer, &horizontalTrack);
        SDL_RenderFillRect(renderer, &horizontalTrack1);

        SDL_Rect verticalTrack = {400, 0, config::VEHICLE_WIDTH, config::WINDOW_HEIGHT}; 
        SDL_Rect verticalTrack1 = {0, 0, config::VEHICLE_WIDTH, config::WINDOW_HEIGHT}; 
        SDL_Rect verticalTrack2 = {config::WINDOW_WIDTH-config::VEHICLE_WIDTH, 0, config::VEHICLE_WIDTH, config::WINDOW_HEIGHT}; 

        SDL_RenderFillRect(renderer, &verticalTrack);
        SDL_RenderFillRect(renderer, &verticalTrack1);
        SDL_RenderFillRect(renderer, &verticalTrack2);
}