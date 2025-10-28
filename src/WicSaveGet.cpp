#include "WicSave.hpp"

const void *wicsave::SaveData::GetData(const std::string &key)
{
    auto it = _data.find(key);
    if (it == _data.end())
        throw std::runtime_error("wicsave::SaveData::GetData() — invalid key: " + key);

    return it->second.data();
}

ssize_t wicsave::SaveData::GetDataSize(const std::string &key)
{
    auto it = _data.find(key);
    if (it == _data.end()) {
        throw std::runtime_error("wicsave::SaveData::GetDataSize() - invalid key: " + key);
    }

    return it->second.size();
}