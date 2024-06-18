#include <random>
#include <algorithm>

#include "Engine.h"

Engine::Engine(Coord winSize, int maxNum, int AlgoNum, bool flag)
    : m_winSize(winSize), m_algoID(AlgoNum), negativeFlg(flag)
{
    m_numbers = genRand(maxNum, winSize.Y);
    calcNumbers();

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        throw std::runtime_error("Could not initialize SDL");
    }

    // Initialize SDL_ttf
    if (TTF_Init() != 0) 
    {
        throw std::runtime_error("Unable to initialize SDL_ttf");
    }

    init(winSize, winTitle);
}

Engine::~Engine()
{
    m_numbers.clear();
    m_numbers.shrink_to_fit();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(m_textTexture);
    SDL_DestroyRenderer(m_render);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Engine::Run()
{
    sortingThread = std::thread(&Engine::startSorting, this);
    while (m_running)
    {
        handleEvents();
        std::unique_lock<std::mutex> lock(mtx);
        if (updateAvailable)
        {
            Draw();
            updateAvailable = false;
            cv.notify_all();
        }
        lock.unlock();
    }

    if (sortingThread.joinable())
    {
        sortingThread.join();
    }
}

void Engine::startSorting()
{
    auto start = std::chrono::high_resolution_clock::now();
    switch (m_algoID)
    {
    case 1:
        std::cout << "Running Bubble Sort\n";
        renderText("Bubble Sort", { 0, 255, 255 }); // Cyan
        sortObj.bubbleSort(this); //, sortObj(new SortingAlgo())
        break;
    case 2:
        std::cout << "Running Selection Sort\n";
        renderText("Selection Sort", { 255, 255, 0 }); // Bright Yellow
        sortObj.selectionSort(this);
        break;
    case 3:
        std::cout << "Running Insertion Sort\n";
        renderText("Insertion Sort", { 255, 0, 255 }); // Magenta
        sortObj.insertionSort(this);
        break;
    case 4:
        std::cout << "Running Binary Insertion Sort\n";
        renderText("Binary Insertion Sort", { 127, 255, 0 }); // Chartreuse
        sortObj.binaryInsertionSort(this);
        break;
    case 5:
        std::cout << "Running Shell Sort\n";
        renderText("Shell Sort", { 155, 0, 255 }); // rand
        sortObj.shellSort(this);
        break;
    case 6:
        std::cout << "Running Heap Sort\n";
        renderText("Heap Sort", { 128, 0, 128 }); // Electric Purple
        sortObj.heapSort(this);
        break;
    case 7:
        std::cout << "Running Quick Sort\n";
        renderText("Quick Sort", { 255, 102, 178 }); // Hot Pin
        sortObj.quickSort(this, m_numbers, 0, m_numbers.size() - 1);
        break;
    case 8:
        std::cout << "Running Merge Sort\n";
        renderText("Merge Sort", { 255, 165, 0 }); // Orange
        sortObj.mergeSort(this, m_numbers, 0, m_numbers.size() - 1);
        break;
    case 9:
        std::cout << "Running Counting Sort\n";
        renderText("Counting Sort", { 255, 215, 0 }); // Golden Yellow
        sortObj.countingSort(this);
        break;
    case 10:
        std::cout << "Running Radix Sort\n";
        renderText("Radix Sort", { 255, 127, 80 }); // Bright Coral
        sortObj.radixSort(this);
        break;
    case 11:
        std::cout << "Running Bucket Sort\n";
        renderText("Bucket Sort", { 0, 255, 127 }); // Bright Lime Green
        sortObj.bucketSort(this);
        break;
    default:
        break;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken: " << elapsed.count() << " seconds\n";

    std::lock_guard<std::mutex> lock(mtx);
    sortingCompleted = true;
    cv.notify_all();
}

void Engine::init(Coord winSize, const char* winTitle)
{
    // initialize Window
    m_window = SDL_CreateWindow(winTitle, 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED, 
                                m_winSize.X, m_winSize.Y, 
                                SDL_WINDOW_SHOWN);

    if (m_window == nullptr)
    {
        throw std::runtime_error("Could NOT initialize SDL window!");
    }

    // initialize renderer
    m_render = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (m_render == nullptr)
    {
        throw std::runtime_error("Could NOT initialize SDL Renderer!");
    }

    font = TTF_OpenFont("fonts/font.ttf", 24);

    if (font == nullptr) 
    {
        throw std::runtime_error("Could NOT Load SDL TTF font file!");
    }
}

void Engine::resetColorIdx()
{
    m_leftIndex = -1;
    m_rightIndex = -1;
    m_pivotIndex = -1;
}

void Engine::calcNumbers()
{
    int n = m_numbers.size();
    m_columnWidth = static_cast<float>(m_winSize.X) / n;

    // Calculate maximum column height based on window size and maximum value
    m_maxValue = *std::max_element(m_numbers.begin(), m_numbers.end(), [](int a, int b) { return std::abs(a) < std::abs(b); });
    m_maxValue = std::abs(m_maxValue);
    m_startColPos = std::max((m_winSize.X - (m_columnWidth * n)), 0);
}

void Engine::handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            m_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_running = false;
            }
            break;
        }
    }
}

void Engine::renderText(const std::string& text, SDL_Color color)
{
    // Create a surface from the text
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (textSurface == nullptr)
    {
        // Handle error (e.g., could not render text)
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Create texture from surface pixels
    m_textTexture = SDL_CreateTextureFromSurface(m_render, textSurface);
    if (m_textTexture == nullptr)
    {
        // Handle error (e.g., could not create texture)
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    }

    // Get rid of the old surface
    SDL_FreeSurface(textSurface);
}

void Engine::Draw()
{
    SDL_RenderClear(m_render);

    // Draw columns
    DrawColumns();

    // Draw text
    if (m_textTexture != nullptr)
    {
        SDL_Rect textRect = { 10, 10, 0, 0 }; // Adjust position as needed
        SDL_QueryTexture(m_textTexture, nullptr, nullptr, &textRect.w, &textRect.h);
        SDL_RenderCopy(m_render, m_textTexture, nullptr, &textRect);
    }

    SDL_RenderPresent(m_render);
}

void Engine::DrawColumns()
{
    SDL_Rect Column{ m_startColPos/2, 0, m_columnWidth, 0 };
    int midPoint = m_winSize.Y / 2;

    // Draw Horizontal line
    if (negativeFlg)
    {
        SDL_SetRenderDrawColor(m_render, 255, 255, 255, 255);
        SDL_RenderDrawLine(m_render, 0, midPoint, m_winSize.X, midPoint);
    }

    for (int i = 0; i < m_numbers.size(); i++)
    {
        if (negativeFlg)
        {
            int colH = m_numbers[i] * midPoint / m_maxValue;
            if (m_numbers[i] >= 0)
            {
                Column.h = colH;
                Column.y = midPoint - colH;
            }
            else
            {
                Column.h = -colH;
                Column.y = midPoint;
            }
        }
        else
        {
            Column.h = m_numbers[i] * m_winSize.Y / m_maxValue;
            Column.y = m_winSize.Y - Column.h;
        }
        
        // Set color based on swap tracking
        if (i == m_pivotIndex)
        {
            SDL_SetRenderDrawColor(m_render, 0, 0, 255, 255); // Blue for pivot index
        }
        else if (i == m_leftIndex)
        {
            SDL_SetRenderDrawColor(m_render, 255, 0, 0, 255); // Red for left index
        }
        else if (i == m_rightIndex)
        {
            SDL_SetRenderDrawColor(m_render, 0, 255, 0, 255); // Green for right index
        }
        else 
        {
            SDL_SetRenderDrawColor(m_render, 255, 255, 255, 255);
        }

        SDL_RenderFillRect(m_render, &Column);
        Column.x += m_columnWidth;
    }

    SDL_SetRenderDrawColor(m_render, 0, 0, 0, 255);
}

std::vector<int> Engine::genRand(int maxNum, int winH)
{
    std::vector<int> num(maxNum);

    // Initialize vector with random numbers within [0, maxNum)
    static std::mt19937 rng(std::random_device{}());
    for (int i = 0; i < num.size(); ++i)
    {
        if (negativeFlg)
        {
            num[i] = std::uniform_int_distribution<int>(-maxNum / 2, maxNum / 2)(rng);
        }
        else
        {
            num[i] = std::uniform_int_distribution<int>(1, maxNum)(rng);
        }
    }

    for (int& n : num)
    {
        n = n * winH / maxNum;
    }
    return num;
}