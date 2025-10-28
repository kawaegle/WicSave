#include "WicSave.hpp"

bool wicsave::SaveData::Load()
{
    if (_saveFile.empty())
        return false;
    if (!std::filesystem::exists(_saveFile)) {
        std::string err = "try to load ";
        err += _saveFile;
        err += " but don't exists";
        throw std::runtime_error(err);
    }

    size_t file_size = std::filesystem::file_size(_saveFile);

    if (file_size < HASH_SIZE)
        throw std::runtime_error("File too small for hash");

    std::vector<char> data(file_size - HASH_SIZE);
    std::vector<char> hash(HASH_SIZE);

    if (_Read(data.data(), hash.data(), file_size) == false)
        throw std::runtime_error("File cannot be read");

    MD5 tmp_hash;
    tmp_hash.add(data.data(), file_size - HASH_SIZE);
    if (std::memcmp(tmp_hash.getHash().data(), hash.data(), HASH_SIZE) != 0)
        throw std::runtime_error("Hash is invalid");

    _Parse(data.data(), data.size());
    return true;
}

void wicsave::SaveData::_Parse(const void *data, size_t size)
{
    _data.clear();
    size_t key_size;
    size_t data_size;

    const unsigned char *buff = static_cast<const unsigned char *>(data);
    while (buff < static_cast<const unsigned char *>(data) + size) {
        std::memcpy(&key_size, buff, sizeof(size_t));
        buff += sizeof(size_t);
        std::memcpy(&data_size, buff, sizeof(size_t));
        buff += sizeof(size_t);

        if (key_size == 0 || data_size == 0) {
            continue;
        }
        std::string key(reinterpret_cast<const char *>(buff), key_size);
        buff += key_size;

        std::vector<unsigned char> data_vector(buff, buff + data_size);
        buff += data_size;

        _data.insert({std::move(key.data()), std::move(data_vector)});
    }
    return;
}

bool wicsave::SaveData::Load(const std::filesystem::path &file)
{
    std::filesystem::path old = this->_saveFile;
    bool ret = false;
    this->_saveFile = file;
    ret = this->Load();
    this->_saveFile = old;
    return ret;
}

bool wicsave::SaveData::_Read(void *data, void *hash, size_t size)
{
    std::ifstream ifs(_saveFile, std::ios::binary);
    if (!ifs)
        return false;

    ifs.seekg(0);
    ifs.read((char *)data, size - HASH_SIZE);
    ifs.read((char *)hash, HASH_SIZE);

    ifs.close();
    return true;
}
