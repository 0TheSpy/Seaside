#pragma once

#include <fstream>
#include <string>
#include <optional>
#include <unordered_map>

#pragma pack(push,1)
struct vpk_header_t
{
    uint32_t signature;
    uint32_t version;
    uint32_t tree_size;
    uint32_t file_data_section_size;
    uint32_t archive_md5_section_size;
    uint32_t other_md5_section_size;
    uint32_t signature_section_size;
};

struct dir_entry_t
{
    uint32_t crc;
    uint16_t preload_bytes;
    uint16_t archive_index;
    uint32_t entry_offset;
    uint32_t entry_length;
    uint16_t terminator;
};
#pragma pack(pop)

class c_vpk_entry
{
public:
    c_vpk_entry() = default;
    c_vpk_entry(const std::string& base_file, const std::string& path, const dir_entry_t& entry) :_base_file(base_file), _path(path), _entry(entry) {}

    std::optional<std::vector<uint8_t>> get_data()
    {
        size_t _dir_pos = _base_file.find("dir");

        std::string old_number_string = std::to_string(_entry.archive_index);
        std::string vpk_file = _base_file.replace(_base_file.begin() + _dir_pos, _base_file.begin() + _dir_pos + 3, std::string(3 - old_number_string.length(), '0') + old_number_string);
        std::ifstream stream(vpk_file, std::ios::binary);
        if (!stream.is_open())
            return {};

        uint8_t* data = new uint8_t[_entry.entry_length];
        stream.seekg(_entry.entry_offset, stream.beg);
        stream.read((char*)data, _entry.entry_length);
        stream.close();

        std::vector<uint8_t> data_vector(data, data + _entry.entry_length);
        delete[] data;

        return data_vector;
    }

    std::vector<uint8_t> preload_bytes = {};
    std::string _base_file = {};
    std::string _path = {};
    dir_entry_t _entry = {};
};

class c_vpk_archive
{
public:
    bool load(const std::string& name)
    {
        std::ifstream stream(name, std::ios::binary);
        if (!stream.is_open())
            return false;

        try
        {
            vpk_header_t file_header = {};
            stream.read((char*)&file_header, sizeof(file_header));
            if (file_header.signature != 0x55aa1234)
            {
                stream.close();
                return false;
            }

            if (file_header.version != 2)
            {
                stream.close();
                return false;
            }

            while (true)
            {
                std::string extension;

                std::getline(stream, extension, '\x00');
                if (extension.empty())
                    break;

                while (true)
                {
                    std::string directory;
                    std::getline(stream, directory, '\x00');
                    if (directory.empty())
                        break;

                    while (true)
                    {
                        std::string file_name;
                        std::getline(stream, file_name, '\x00');
                        if (file_name.empty())
                            break;

                        std::string path = directory + '/' + file_name + '.' + extension;

                        dir_entry_t dir_entry;
                        stream.read((char*)&dir_entry, sizeof(dir_entry));

                        c_vpk_entry vpk_entry(name, path, dir_entry);
                        if (dir_entry.preload_bytes)
                        {
                            uint8_t* bytes = new uint8_t[dir_entry.preload_bytes];
                            stream.read((char*)bytes, dir_entry.preload_bytes);
                            vpk_entry.preload_bytes = std::vector<uint8_t>(bytes, bytes + dir_entry.preload_bytes);
                            delete[] bytes;
                        }

                        if (!files.count(path))
                            files[path] = vpk_entry;
                    }
                }
            }
        }
        catch (const std::exception& e) { return false; }
        stream.close();

        return true;
    }

    std::optional<c_vpk_entry> get_file(const std::string& name)
    {
        if (files.count(name))
            return files[name];

        return {};
    }
    std::unordered_map<std::string, c_vpk_entry> files = {};
};