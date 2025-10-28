#include "WicSave.hpp"

bool wicsave::SaveData::AddData(const std::string &key, const void *data, size_t data_size)
{

    std::vector<unsigned char> buffer(data_size);
    std::memmove(buffer.data(), data, data_size);
    _data[key] = std::move(buffer);

    return true;
}