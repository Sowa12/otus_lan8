#include <options.h>

Options::Options()
    : _desc("Example")
{
    _desc.add_options()
    (OPTION_NAME_HELP.c_str(), "produce help message")
    (OPTION_NAME_INC_PATH.c_str(), po::value<vstring>(&_includeDirs))
    (OPTION_NAME_EXC_PATH.c_str(), po::value<vstring>(&_excludeDirs))
    (OPTION_NAME_FILE_MASK.c_str(), po::value<vstring>(&_fileMasks))
    (OPTION_NAME_HASH.c_str(), po::value<std::string>(&_hashName)->default_value("md5"))
    (OPTION_NAME_MIN_SIZE.c_str(), po::value<uint32_t>(&_minFileSize)->default_value(1))
    (OPTION_NAME_BLOCK_SIZE.c_str(), po::value<uint32_t>(&_blockSize)->default_value(64))
    (OPTION_NAME_SCAN_LEVEL.c_str(), po::value<uint32_t>(&_scanLevel)->default_value(0));
}

void Options::parse(const int argc, char **argv)
{
    try
    {
        po::store(po::parse_command_line(argc, argv, _desc), _vars);
        po::notify(_vars);
    }
    catch(const std::exception& e)
    {
        throw e;
    }
}

const vstring &
Options::getIncludeDirs()
{
    return _includeDirs;
}

const vstring &
Options::getIncludeDirs() const
{
    return _includeDirs;
}

const vstring &
Options::getExcludeDirs()
{
    return _excludeDirs;
}

const vstring &
Options::getExcludeDirs() const
{
    return _excludeDirs;
}

const vstring &
Options::getFileMasks()
{
    return _fileMasks;
}

const vstring &
Options::getFileMasks() const
{
    return _fileMasks;
}


const std::string &
Options::getHashName() 
{
    return _hashName;
}

const std::string &
Options::getHashName() const
{
    return _hashName;
}

const uint32_t 
Options::getMinFileSize()
{
    return _minFileSize;
}

const uint32_t 
Options::getMinFileSize() const
{
    return _minFileSize;
}

const uint32_t 
Options::getBlockSize()
{
    return _blockSize;
}

const uint32_t 
Options::getBlockSize() const
{
    return _blockSize;
}

const uint32_t 
Options::getScanLevel()
{
    return _scanLevel;
}

const uint32_t
Options::getScanLevel() const
{
    return _scanLevel;
}

const bool
Options::needHelp()
{
    return _vars.count("help");
}

const bool Options::needHelp() const
{
    return _vars.count("help");
}

const po::options_description& 
Options::getOptions() 
{
    return _desc;
}

const po::options_description&
Options::getOptions() const
{
    return _desc;
}
