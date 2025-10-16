#pragma once

#define BOOST_REGEX_STANDALONE

#include <options.h>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/regex.hpp>

namespace fs = boost::filesystem;
namespace detail = boost::uuids::detail;

class Bayan
{
public:
    using vstr = std::vector<std::string>;
    using vvstr = std::vector<vstr>;

    Bayan(const Options& options);
    void process();
    void parse(const fs::path& dirpath, uint32_t scanLevel);
    void obrFiles();
    
    vvstr getGroups() { return _groups; }

    ~Bayan();
private:
    Options _options;
    vstr _files;
    vvstr _groups;
    std::map<std::string, vstr> _cache;

    bool isGoodDir(const std::string& dirpath);
    bool isGoodFile(const std::string& filename);

    bool isDuplicat(const std::string& file1, const std::string& file2);
    std::string calcMD5(const std::string& bytes); // - функция подсчета хэша

    size_t getFileSize(const std::string& file);

    void addToCache(const std::string& filename, const std::string& hash);
    std::string getFromCache(const std::string& filename, const size_t step);
};