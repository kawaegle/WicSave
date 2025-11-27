#ifndef _WICSAVE_
#define _WICSAVE_

#include "hash.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#ifdef _MSC_VER
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#define HASH_SIZE 32

namespace wicsave
{

using DataMap = std::unordered_map<std::string, std::vector<unsigned char>>;

class SaveData
{
  public:
    SaveData() {};

    SaveData(const std::filesystem::path &file) : _saveFile(file)
    {
        if (!std::filesystem::exists(_saveFile)) {
            std::ofstream ofs(_saveFile);
            if (ofs.is_open())
                ofs.close();
            else {
                throw std::runtime_error("you computer is on fire we can't create file");
            }
        }
    };

    SaveData(SaveData &&) = default;
    SaveData(const SaveData &) = default;
    SaveData &operator=(SaveData &&) = default;
    SaveData &operator=(const SaveData &) = default;
    ~SaveData() = default;

    /*
     * Add data to the save elements
     * @param key: the name of the key where we should insert data
     * @param data: raw brut data
     * @param data_size: how much data we pass as data
     * @return: true if we can insert data, throw otherwise
     *
     * the given data is copied inside
     */
    bool AddData(const std::string &key, const void *data, size_t data_size);

    /*
     * Save all data in the specified save file
     * @return: true if saved throw the reason otherwise
     */
    bool Save(void);

    /*
     * Save all data in the specified save file
     * @param file: the new file where we should save instead of official save file
     * @return: true if saved throw the reason otherwise
     */
    bool Save(const std::filesystem::path &file);

    /*
     * Load all data saved in the specified save file
     * @return: true if loaded without error, throw the error otherwise
     */
    bool Load(void);

    /*
     * Load all data saved in the specified save file
     * @param file: the new file to load instead of the official save file
     * @return: true if loaded without error, throw the error otherwise
     */
    bool Load(const std::filesystem::path &file);

    /*
     * Set where the save is saved
     * @param file: filepath where to save
     */
    void SetSaveFile(const std::filesystem::path &file)
    {
        this->_saveFile = file;
    };

    /*
     * Get the path where we want to save
     * @return: the path where we save
     */
    const std::filesystem::path GetSaveFile() const;

    /*
     * Get data from a given key
     * @param key: the key where you saved the data
     * @return: the element of data or throw if trying to access to invalid key
     */
    const void *GetData(const std::string &key);

    /*
     * Get data size for a given key
     * @param key: key where to retrieve the data
     * @return: the size of the data, throw otherwise if access to key that don't exist
     */
    ssize_t GetDataSize(const std::string &key);

  private:
    std::filesystem::path _saveFile;
    DataMap _data;
    MD5 _hash;

    /*
     * compute the size of every element to generate full buffer
     * @return: the total size need by the data and the key to be writted, -1 on error
     */
    ssize_t _ComputeSize(void);

    /*
     * fill a new vector with all data
     * @param data: void * element allocated to handle all data
     */
    void _FillData(void *data);

    /**
     * @brief Write data and hash to the save file
     *
     * Writes the serialized data followed by the 32-byte MD5 hash string.
     *
     * @param data Serialized data buffer
     * @param size Size of the data buffer in bytes
     * @return true if write was successful, false if file couldn't be opened
     */
    bool _Write(const void *data, size_t size);

    /**
     * @brief Read data and hash from the save file
     *
     * Reads the file contents, separating the data from the hash.
     *
     * @param data Buffer to store the data portion (size - HASH_SIZE bytes)
     * @param hash Buffer to store the hash (HASH_SIZE bytes)
     * @param size Total file size in bytes
     * @return true if read was successful, false if file couldn't be opened
     *
     * @note The data buffer should be allocated to (size - HASH_SIZE) bytes
     *       The hash buffer should be allocated to HASH_SIZE bytes
     */
    bool _Read(void *data, void *hash, size_t size);

    /**
     * @brief Parse serialized data and populate the internal data store
     *
     * Deserializes the buffer and reconstructs the key-value pairs.
     * Clears any existing data before parsing.
     *
     * @param data Serialized data buffer
     * @param size Size of the buffer in bytes
     *
     * @throws std::runtime_error if:
     *         - Data is corrupted (incomplete size fields)
     *         - Buffer overrun detected
     *         - Invalid zero-sized keys encountered
     *
     * @note Stops parsing if it encounters key_size==0 and data_size==0
     *       (used as padding/end marker)
     */
    void _Parse(const void *data, size_t size);
};

}; // namespace wicsave
#endif // !_WICSAVE_