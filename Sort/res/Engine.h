#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "SortingAlgo.h"

struct Coord
{
    int X;
    int Y;
};

class Engine
{
public:
    Coord m_winSize;
    std::vector<int> m_numbers;
    bool m_running = true;
    bool m_sorted = false;
    float m_startColPos;
    int m_algoID;
    int m_columnWidth;
    int m_maxValue;
    int m_pivotIndex = -1;
    int m_leftIndex = -1;
    int m_rightIndex = -1;
    int m_numElements = 1280;
    bool negativeFlg;
    const char* winTitle = "Sorting Visualizer";

    std::thread sortingThread;
    std::mutex mtx;
    std::condition_variable cv;
    bool sortingCompleted = false;
    bool updateAvailable = false;


    Engine(Coord winSize, int maxNum, int AlgoNum, bool flag);
    ~Engine();
    void Run();
    void handleEvents();
    void resetColorIdx();
    void Draw();

    SDL_Window* getWindow() { return m_window; }
    SDL_Renderer* getRenderer() { return m_render; }
    TTF_Font* getFont() { return font; }

private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_render = nullptr;
    TTF_Font* font = nullptr;
    SDL_Texture* m_textTexture = nullptr;
    SortingAlgo sortObj;

    void init(Coord winSize, const char* winTitle);
    void startSorting();
    void renderText(const std::string& text, SDL_Color color);
    void DrawColumns();
    void calcNumbers();
    std::vector<int> genRand(int maxNum, int winH);
};

#endif // ENGINE_H