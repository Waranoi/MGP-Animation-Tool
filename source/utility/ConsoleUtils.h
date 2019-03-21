#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>

namespace ConsoleUtils
{
    inline int GetIntegerInput();
    inline int GetIntegerInput(bool onlyPositive);
    inline int GetIntegerInput(int min, int max); // Both min and max are inclusive
};

inline int ConsoleUtils::GetIntegerInput()
{
    // How to get a number.
    int myNumber = 0;
    
    while (true) {
        std::string input = "";
        std::cout << "Please enter a valid number: ";
        while (input.length() == 0) 
            std::getline(std::cin, input);

        // This code converts from string to number safely.
        std::stringstream myStream(input);
        if (myStream >> myNumber)
            break;

        std::cout << "Invalid number, please try again :" << input << ":" << std::endl << std::endl;
    }

    return myNumber;
}

inline int ConsoleUtils::GetIntegerInput(bool onlyPositive)
{
    while (true)
    {   
        int result = GetIntegerInput();
        if (result < 0)
            printf("Only positive numbers allowed.\n");
        else
            return result;
    }
}

inline int ConsoleUtils::GetIntegerInput(int min, int max)
{
    while (true)
    {
        int result = GetIntegerInput();
        if (result < min)
            printf("Number can't be below %d.\n", min);
        else if (result > max)
            printf("Number can't be above %d.\n", max);
        else 
            return result;
    }
}