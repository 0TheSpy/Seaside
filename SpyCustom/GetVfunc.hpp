#ifndef GETVFUNC
#define GETVFUNC

#pragma once

inline void**& getvtable(void* inst, size_t offset = 0)
{
    return *reinterpret_cast<void***>((size_t)inst + offset);
}

inline const void** getvtable(const void* inst, size_t offset = 0)
{
    return *reinterpret_cast<const void***>((size_t)inst + offset);
}

template<typename Fn>
inline Fn getvfunc(const void* inst, size_t index, size_t offset = 0)
{
    return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
}


template <typename Fn = void*>
Fn get_vfunc(void* class_base, const size_t index)
{
    return (*reinterpret_cast<Fn**>(class_base))[index];
}

#include <string>
#include <memory>
inline char* MakeControlChars(char str[1024]) { 
    char name[1024];
    strcpy(name, str);
    for (int i = 0; i < 1024; i++)
    {
        if (name[i] == 0x5C && name[i + 1] == 0x78) 
        {
            char controlchar[3]; controlchar[0] = name[i + 2]; controlchar[1] = name[i + 3]; controlchar[2] = 0x0;
            short x = std::stoul(controlchar, nullptr, 16);
            name[i] = x; 
            for (int j = i + 4; j < 1024; j++)
            {
                name[j - 3] = name[j];
                if (name[j] == 0x0)
                    break;
            }
        }
        if (name[i] == 0x0)
            break;
    }
     
    return name;
}



#endif