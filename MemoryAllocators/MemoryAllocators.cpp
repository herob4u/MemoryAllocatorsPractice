// MemoryAllocators.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AllocUtils.h"
#include "PoolAllocator.h"

int main()
{
    std::cout << "Hello World!\n";
    
    PoolAllocator<2048, 3> alloc;
    void* p1 = alloc.Allocate(4);
    std::cout << alloc.GetMemoryDump() << "\n";

    void* p2 = alloc.Allocate(100); // Should get us the next chunk
    void* p3 = alloc.Allocate(2048); // Should still fit in last chunk
    void* p4 = alloc.Allocate(300); // Should create new block;

    printf("Pointers:\n %p\n, %p\n, %p\n, %p\n", p1, p2, p3, p4);

    while(1)
    {
        int num;
        std::cin >> num;

        if(num > 0)
        {
            std::cout << "Next Pow2 " << AllocUtils::NextPow2Fast(num) << "\n";
            std::cout << "Align16 " << AllocUtils::Align16(num) << "\n";
            std::cout << "Align8 " << AllocUtils::Align8(num) << "\n";
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
