/*
* GOATED DOCUMENTATION
* http://jbremer.org/x86-api-hooking-demystified/
*/

#include <Windows.h>
#include <iostream>

int playerHealth = 100;

void func1(int a, int b)
{
    int res = a + b;
    std::cout << res << "\n";
}

void func2(int a, int b)
{
    int res = a + b;
    std::cout << res << "\n";
    std::cout << "function B" << "\n";
}

void DecreasePlayerHealth()
{
    playerHealth -= 10;    
}

int main()
{
    std::cout << "address of func1: " << &func1;
    std::cout << " address of func2: " << &func2;

    // SIMPLE UNCONDITIONAL JMP REDIRECT
    // x86 unconditional jmp is 5 bytes in length 
    // one byte is the opcode 
    // other four bytes are the 32 bit relative offset
    // relative offset = the place we want to execute desired code

    // CALCULATING RELATIVE OFFSET
    // the relative offset is the difference in memory (bytes)
    // between two functions (e.g. src function and dest function)
    // e.g. we want to go from funca to funcb
    // dest - src - 5
    // functionB - functionA - 5
    void* funcionA = &func1;
    void* functionB = &func2;
    uintptr_t relative_offset = (char*)functionB - (char*)funcionA - 5;

    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 1)
        {
            DecreasePlayerHealth();
            std::cout << "\nPlayer Health: " << playerHealth << "\n";
        }
    }

}

// to hook a desired function by inserting a jmp instruction 
// the original bytes at the location we are placing the jmp 
// will be overwritten
// the amount of bytes being overwritten depends on the size of the 
// jmp instruction being written
// the instructions being overwritten are known as stolen bytes