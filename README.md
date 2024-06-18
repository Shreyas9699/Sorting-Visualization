# Sorting Visualization

Sorting Visualization is a graphical application that visualizes various sorting algorithms using SDL (Simple DirectMedia Layer) and SDL_ttf (SDL TrueType Font). The application demonstrates how different sorting algorithms work by providing a visual representation of the sorting process. The latest update introduces multi-threading to separate the sorting and rendering processes, improving performance and responsiveness.

## Video Demonstration

https://github.com/Shreyas9699/Sorting-Visualization/assets/63774467/625be771-1627-437b-9098-31c8d469c0f7



## Features

- Visualizes the following sorting algorithms:
  - Bubble Sort
  - Selection Sort
  - Insertion Sort
  - Binary Insertion Sort
  - Shell Sort
  - Heap Sort
  - Quick Sort
  - Merge Sort
  - Counting Sort (*Positive Integers Only*)
  - Radix Sort (*Positive Integers Only*)
  - Bucket Sort (*Positive Integers Only*)
- Allows both positive and negative integers (*flag to include negavtive integers*)
- Adjustable number of elements to sort
- Customizable window size
- Real-time sorting process visualization
- Time measurement for sorting process

## Prerequisites

### General

- [SDL2](https://github.com/libsdl-org/SDL/releases)
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf/releases)
- [SDL_image](https://github.com/libsdl-org/SDL_image/releases)
- C++20 compatible compiler

### Windows (Visual Studio)

1. **Update C++ Version to 20:**

   - Go to your project properties.
   - Under `Configuration Properties` > `General`, set the `C++ Language Standard` to `ISO C++20 Standard (/std:c++20)`.

2. **Add Include Directories:**

   - Go to `Configuration Properties` > `VC++ Directories` > `Include Directories`.
   - Add the following paths: (Might differ, select where you have copied the downloaded SDL files)
     ```
     C:\libraries\SDL2\SDL2-2.30.3\include
     C:\libraries\SDL2\SDL2_image-2.8.2\include
     C:\libraries\SDL2\SDL2_ttf-2.22.0\include
     ```

3. **Add Library Directories:**

   - Go to `Configuration Properties` > `VC++ Directories` > `Library Directories`.
   - Add the following paths: (Might differ, select where you have copied the downloaded SDL files)
     ```
     C:\libraries\SDL2\SDL2-2.30.3\lib\x64
     C:\libraries\SDL2\SDL2_image-2.8.2\lib\x64
     C:\libraries\SDL2\SDL2_ttf-2.22.0\lib\x64
     ```

4. **Add Linker Dependencies:**

   - Go to `Configuration Properties` > `Linker` > `Input` > `Additional Dependencies`.
   - Add the following libraries:
     ```
     SDL2.lib
     SDL2main.lib
     SDL2_ttf.lib
     SDL2_image.lib
     ```

5. **Update System Environment Path:**

   - If you encounter `SDL2_ttf.dll not found` error when running the project, add the SDL library directory to your system environment variable `PATH`.
     ```
     C:\libraries\SDL2\SDL2-2.30.3\lib\x64
     C:\libraries\SDL2\SDL2_image-2.8.2\lib\x64
     C:\libraries\SDL2\SDL2_ttf-2.22.0\lib\x64
     ```

   Refer to [Setting up SDL2 in Windows (Visual Studio)](https://www.studyplan.dev/sdl-dev/sdl-setup-windows) for detailed steps.

## Getting Started

### Installation

1. **Clone the repository:**

    ```sh
    git clone https://github.com/Shreyas9699/Sorting-Visualization.git
    cd Sorting-Visualization
    ```

### Build and Run

1. **Build the project:**

    - Open the solution file in Visual Studio.
    - Build the project by selecting `Build` > `Build Solution` or pressing `Ctrl+Shift+B`.

2. **Run the application:**

    - Run the project by selecting `Debug` > `Start Debugging` or pressing `F5`.

## Usage

1. **Select the number of elements to sort:**
   - Enter a value between 10 and 1440 in console.

2. **Select the sorting algorithm:**
   - Choose an algorithm by entering the corresponding number:
     ```
     1. Bubble Sort
     2. Selection Sort
     3. Insertion Sort
     4. Binary Insertion Sort
     5. Shell Sort
     6. Heap Sort
     7. Quick Sort
     8. Merge Sort
     9. Counting Sort
     10. Radix Sort
     11. Bucket Sort
     ```

3. **Visualization:**
   - Watch the sorting process in real-time.
   - The application will display the time taken for the sorting process upon completion.

4. **Reset:**
   - After the sorting is done, you can close the SDL window by clicking on close button or ESC key.
   - You can then choose to reset and try another configuration by entering 'Y' when prompted.

## Code Overview

- **Engine.h & Engine.cpp:**
  - Contains the `Engine` class which manages the application state, rendering, and interaction with sorting algorithms.

- **main.cpp:**
  - Handles user input to configure and run the sorting visualization.
  - Validates input and initializes the `Engine` object.

- **SortingAlgo.h:**
  - Contains implementations of various sorting algorithms.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any bug fixes or enhancements.

## Acknowledgements

- [SDL](https://www.libsdl.org/)
- [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/)
- [SDL_image](https://www.libsdl.org/projects/SDL_image/)

