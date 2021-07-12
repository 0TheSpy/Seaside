#ifndef CONFIG
#define CONFIG
#pragma once

#include "Options.hpp"
#include "Singleton.hpp"

#include <fstream>
#include <sstream>

  
#include <filesystem>
namespace fs = std::filesystem;
 
static std::time_t to_time_t(FILETIME ft)
{
    std::time_t ret = 0; 
    ULARGE_INTEGER ull;
    ull.LowPart = ft.dwLowDateTime;
    ull.HighPart = ft.dwHighDateTime;
    ret = (ull.QuadPart / 10000000ULL - 11644473600ULL);
    return ret;
}

std::vector<char> HexToBytes(const std::string& hex) {
    std::vector<char> res;

    for (auto i = 0u; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = (char)strtol(byteString.c_str(), NULL, 16);
        res.push_back(byte);
    }

    return res;
}
std::string BytesToString(unsigned char* data, int len) {
    constexpr char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    std::string res(len * 2, ' ');
    for (int i = 0; i < len; ++i) {
        res[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
        res[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return res;
}
std::vector<std::string> Split(const std::string& str, const char* delim) {
    std::vector<std::string> res;
    char* pTempStr = _strdup(str.c_str());
    char* context = NULL;
    char* pWord = strtok_s(pTempStr, delim, &context);
    while (pWord != NULL) {
        res.push_back(pWord);
        pWord = strtok_s(NULL, delim, &context);
    }

    free(pTempStr);

    return res;
}

using namespace std;

class Config : public Singleton<Config> {
public:

    std::vector<std::string> configs = {  };
    std::vector<std::string> times = {  };
     
    bool file_exists(std::string filename) {
        std::ifstream ifile(filename);
        return (bool)ifile;
    }

    void Create(char* name) {
        string newname = string(name) + ".ss";
        if (!file_exists(newname)) {
            ofstream newcfg;  
            newcfg.open(newname);
            newcfg.close();
        }
        Refresh();
    }

    void Delete(int selected) {
        string name = configs[selected];
        if (remove(name.c_str()) != 0)
        {
#ifdef DEBUG
            printf("Error deleting file %s\n", name.c_str());
#endif
        }
        Refresh();
    }

    void Save(int selected, char* new_name) {
         
        string name = configs[selected];
        string newname = string(new_name) + ".ss";

        std::ofstream fout(name.c_str(), std::ios::binary);
        const auto sz = sizeof(Options);
        const auto var_sz = sizeof(Var<bool>);
        const auto cnt = sz / var_sz;
        for (auto i = 0; i < cnt; i++) {
            const auto el = &(*(Var<int>*)(&g_Options)) + i;
            auto name = el->name;
            auto val = el->value;
            auto sizeof_val = el->size;
            fout << name << "\t" << BytesToString((unsigned char*)*(int*)&val, sizeof_val) << std::endl;
        }
        fout.close();

        rename(name.c_str(), newname.c_str());
        Refresh();
    }


    void Load(int selected) {

        opt.loading = 1;
        string name = configs[selected];
         
        if (file_exists(name.c_str()) ) {
            std::ifstream fin(name.c_str(), std::ios::binary);
            std::stringstream ss;
            ss << fin.rdbuf();


            auto lines = Split(ss.str(), "\n");

            for (auto line : lines) {
                auto data = Split(line, "\t");
                const auto sz = sizeof(Options);
                const auto var_sz = sizeof(Var<bool>);
                const auto cnt = sz / var_sz;
                for (auto i = 0; i < cnt; i++) {
                    const auto& el = &(*(Var<bool>*)(&g_Options)) + i;
                    if (data[0] == el->name) {
                        auto bytes = HexToBytes(data[1]);
                        memcpy(*(void**)&el->value, bytes.data(), el->size);
                    }
                }
            }
            fin.close();

            OnLoadCfg();
        } 

        else PlaySoundA((char*)"null", opt.hModuleGlobal, SND_ASYNC);
    }

    void OpenFolder() {
        ShellExecute(NULL, NULL, "", NULL, NULL, SW_SHOWNORMAL);
    }
     
    void Refresh() { 
        configs.clear(); times.clear(); 
        std::string search_path = "*.ss";
        WIN32_FIND_DATA fd;
        HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
        std::string vremya;
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    configs.push_back(fd.cFileName); 
                    time_t epoch = to_time_t(fd.ftLastWriteTime);
                    time_t creation = to_time_t(fd.ftCreationTime);
                        if (epoch != creation) 
                            vremya = asctime(localtime(&epoch)); 
                        else
                            vremya = "empty";
                        times.push_back(vremya);
#ifdef DEBUG
                    printf("fn %s %s", fd.cFileName, vremya.c_str());
#endif
                }
            } while (::FindNextFile(hFind, &fd));
            ::FindClose(hFind);
        }


    }

    void FindAutoexec() {
        if (opt.autoconfig != -1) {
            ifstream loadcfg("seaside_autoload");
            if (loadcfg.is_open())
            {
                string line;
                getline(loadcfg, line);

                for (int i = 0; i < Config::Get().configs.size(); i++)
                    if (Config::Get().configs[i] == line)
                        opt.autoconfig = i;

            }
            else opt.autoconfig = -1;
        }
    }

};
 

#endif
