#pragma once
#include <stdio.h>

namespace ConsoleUtils
{
    inline int GetIntegerInput(bool onlyPositive);
    inline int GetIntegerInput(int min, int max); // Both min and max are inclusive
};

inline int ConsoleUtils::GetIntegerInput(bool onlyPositive)
{
    while (true)
    {
        int result;
        int success = scanf ("Enter a number: %d", &result);
        if (onlyPositive == true && result < 0)
            printf("Only positive numbers allowed.\n");
        else if (success == 1)
            return 0;
        else
            printf("Not a valid number.\n");
    }
}

inline int ConsoleUtils::GetIntegerInput(int min, int max)
{
    while (true)
    {
        int result;
        int success = scanf ("Enter a number: %d", &result);
        if (result < min)
            printf("Number can't be below %d.\n", min);
        else if (result > max)
            printf("Number can't be above %d.\n", max);
        else if (success == 1)
            return 0;
        else
            printf("Not a valid number.\n");
    }
}