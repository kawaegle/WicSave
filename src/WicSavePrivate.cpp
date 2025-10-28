#include "WicSave.hpp"

ssize_t wicsave::SaveData::_ComputeSize(void)
{
    ssize_t size = 0;
    for (const auto &[key, data_vector] : _data) {
        size += sizeof(size_t) * 2;
        size += key.size();
        size += data_vector.size();
    }
    return size;
}

void wicsave::SaveData::_FillData(void *data)
{
    unsigned char *buff = (unsigned char *)data;

    for (const auto &[key, data_vector] : _data) {
        size_t key_size = key.size();
        size_t data_size = data_vector.size();

        // write key size
        std::memmove(buff, &key_size, sizeof(size_t));
        buff += sizeof(size_t);

        // write data size
        std::memmove(buff, &data_size, sizeof(size_t));
        buff += sizeof(size_t);

        // write key
        std::memmove(buff, key.data(), key_size);
        buff += key_size;

        // write data bytes
        std::memmove(buff, data_vector.data(), data_size);
        buff += data_size;
    }
}