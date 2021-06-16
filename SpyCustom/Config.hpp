#ifndef CONFIG
#define CONFIG
#pragma once

#include "Options.hpp"
#include "Singleton.hpp"

#include <fstream>
#include <sstream>

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


class Config : public Singleton<Config> {
public:
    void Save() {
        std::ofstream fout("seaside210616.cfg", std::ios::binary);
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
    }

    bool file_exists(std::string filename) {
        std::ifstream ifile(filename);
        return (bool)ifile;
    }

    void Load() {

        opt.loading = 1;
         
        if (file_exists("seaside210616.cfg") ) {
            std::ifstream fin("seaside210616.cfg", std::ios::binary);
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
        else iff.g_pGameUI->CreateCommandMsgBox("Seaside Error", "Unable to find configuration file!", true, false, "", "", "", "", "");

    }
};




#endif
