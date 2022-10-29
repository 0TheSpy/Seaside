#pragma once

#include <cstdint>
#include <Windows.h>

#include <iostream>
#include "Interfaces.hpp"

class VMTHook
{
public:
    VMTHook(void* classptr)
    {
        this->class_pointer = reinterpret_cast<uintptr_t**>(classptr);

        int table_size = 0;
        while (reinterpret_cast<uintptr_t*>(*this->class_pointer)[table_size])
            table_size++;

        original_pointer = *this->class_pointer;

        new_table_pointer = new uintptr_t[sizeof(uintptr_t) * table_size];
        memcpy(new_table_pointer, original_pointer, sizeof(uintptr_t) * table_size);
    }

    ~VMTHook()
    {
        RestoreOldTable();
        delete original_pointer;
        delete new_table_pointer;
    }

    void SwapPointer(size_t index, void* new_function)
    {
        printfdbg("Swapping pointer 0x%X to 0x%X\n", new_table_pointer[index], reinterpret_cast<uintptr_t>(new_function));
        new_table_pointer[index] = reinterpret_cast<uintptr_t>(new_function);
        printfdbg("Swapped pointer!\n");

    }

    void ApplyNewTable()
    {
        printfdbg("Applying new table... (0x%X to 0x%X)\n", reinterpret_cast<uintptr_t>(*class_pointer), reinterpret_cast<uintptr_t>(new_table_pointer));
        * class_pointer = new_table_pointer;
        printfdbg("New table applyed!\n");
    }

    void RestoreOldTable()
    {
        printfdbg("Restoring old table... (0x%X to 0x%X)\n", reinterpret_cast<uintptr_t>(*class_pointer), reinterpret_cast<uintptr_t>(original_pointer));
        * class_pointer = original_pointer;
        printfdbg("Old table restored!\n");
    }

    template<typename T>
    T GetOriginal(size_t index)
    {
        return reinterpret_cast<T>(original_pointer[index]);
    }

private:
    uintptr_t** class_pointer = nullptr;
    uintptr_t* original_pointer = nullptr;
    uintptr_t* new_table_pointer = nullptr;
};
