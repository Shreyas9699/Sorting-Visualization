#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "SortingAlgo.h"

struct Coord
{
    int X;
    int Y;
};

class Engine
{
public:
    Engine(Coord winSize, int maxNum, int AlgoNum, const char* winTitle = "Sorting Visualizer");
    ~Engine();
    void Run();
    void handleEvents();
    void resetColorIdx();
    void Draw();

    SDL_Window* getWindow() { return m_window; }
    SDL_Renderer* getRenderer() { return m_render; }
    TTF_Font* getFont() { return font; }

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

private:
    void init(Coord winSize, const char* winTitle);
    
    void renderText(const std::string& text, SDL_Color color);
    void DrawColumns();
    void calcNumbers();
    std::vector<int> genRand(int maxNum, int winH);
    

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_render = nullptr;
    TTF_Font* font = nullptr;
    SDL_Texture* m_textTexture = nullptr;
    SortingAlgo sortObj;
};

#endif // ENGINE_H