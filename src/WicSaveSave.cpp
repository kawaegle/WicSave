#include "WicSave.hpp"

bool wicsave::SaveData::Save(void)
{
    if (_saveFile.empty())
        return false;
    ssize_t size = this->_ComputeSize();
    std::vector<unsigned char> data(size);
    _FillData(data.data());
    _hash.add(data.data(), size);
    _Write(data.data(), size);
    return true;
}

bool wicsave::SaveData::Save(const std::filesystem::path &file)
{
    std::filesystem::path old = this->_saveFile;
    bool ret = false;
    this->_saveFile = file;
    ret = this->Save();
    this->_saveFile = old;
    return ret;
}

bool wicsave::SaveData::_Write(const void *data, size_t size)
{
    std::ofstream ofs(_saveFile, std::ios::binary);
    if (!ofs)
        return false;
    ofs.write(reinterpret_cast<const char *>(data), size);
    ofs.write(reinterpret_cast<const char *>(_hash.getHash().c_str()), 32);
    ofs.close();
    return true;
}