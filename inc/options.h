#pragma once 

#include <boost/program_options.hpp>

namespace po = boost::program_options;
#include <vector>
#include <cstdint>
#include <string>

using vstring = std::vector<std::string>;

const std::string OPTION_NAME_HELP = "help";
const std::string OPTION_NAME_INC_PATH = "include-path,I";
const std::string OPTION_NAME_EXC_PATH = "exclude-path,E";
const std::string OPTION_NAME_FILE_MASK = "file-mask,F";
const std::string OPTION_NAME_HASH = "hash,H";
const std::string OPTION_NAME_MIN_SIZE = "min-size,M";
const std::string OPTION_NAME_BLOCK_SIZE = "block-size,B";
const std::string OPTION_NAME_SCAN_LEVEL = "scan-level,S";

class Options {
public:
    Options();

    void parse(const int argc, char** argv);

    const vstring& getIncludeDirs();
    const vstring& getIncludeDirs() const;

    const vstring& getExcludeDirs();
    const vstring& getExcludeDirs() const; 

    const vstring& getFileMasks();    
    const vstring& getFileMasks() const;

    const std::string& getHashName();
    const std::string& getHashName() const;

    const uint32_t getMinFileSize();
    const uint32_t getMinFileSize() const;

    const uint32_t getBlockSize();    
    const uint32_t getBlockSize() const;

    const uint32_t getScanLevel();
    const uint32_t getScanLevel() const;

    const bool needHelp();
    const bool needHelp() const;

    const po::options_description& getOptions();
    const po::options_description& getOptions() const;

    ~Options() = default;
private:
    po::options_description _desc;
    po::variables_map _vars;

    vstring _includeDirs;
    vstring _excludeDirs;
    vstring _fileMasks;
    std::string _hashName;
    uint32_t _minFileSize;
    uint32_t _blockSize;
    uint32_t _scanLevel;
};