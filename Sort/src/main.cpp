#include <iostream>
#include "../res/Engine.h"


int validateMaxNum(int num) {
	if (num < 10 || num > 1440) {
		std::cout << "Invalid maxNum value: " << num <<". Using default value 500.\n";
		return 500;
	}
	return num;
}

int validateAlgo(int num) {
	if (num < 1 || num > 11) {
		std::cout << "Invalid algo number. Using default algorithm (10. Radix Sort).\n";
		return 10;
	}
	return num;
}

int main(int argc, char** argv) {
    int maxNum;
    int Algo;
    char userChoice;

    do {
        try {
            // Input for maxNum
            std::cout << "Enter the value of maxNum (between 10 and 1440): ";
            std::cin >> maxNum;
            maxNum = validateMaxNum(maxNum);

            // Algorithm selection text
            std::string algoText = "Enter the value of Algo (1-11):\n"
                "1. Bubble Sort\n"
                "2. Selection Sort\n"
                "3. Insertion Sort\n"
                "4. Binary Insertion Sort\n"
                "5. Shell Sort\n"
                "6. Heap Sort\n"
                "7. Quick Sort\n"
                "8. Merge Sort\n"
                "9. Counting Sort\n"
                "10. Radix Sort\n"
                "11. Bucket Sort\n";

            std::cout << algoText;
            std::cin >> Algo;
            Algo = validateAlgo(Algo);

            Engine* Visualizer = new Engine({ 1440, 800 }, maxNum, Algo);
            Visualizer->Run();
            delete Visualizer;

        }
        catch (std::runtime_error& Error) {
            std::cerr << Error.what() << std::endl;
            return 1;
        }

        std::cout << "Do you want to reset? (Y/N): ";
        std::cin >> userChoice;
        userChoice = std::toupper(userChoice);

    } while (userChoice == 'Y');

    return 0;
}