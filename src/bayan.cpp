#include <bayan.h>

Bayan::Bayan(const Options& options)
: _options(options)
{

}

void 
Bayan::process()
{
    auto& dirs = _options.getIncludeDirs();

    for (auto it = dirs.begin(); it != dirs.end(); ++it)
    {
        fs::path p(*it);

        if (fs::exists(p))
        {
            if (fs::is_regular_file(p))
            {
                std::cerr << *it
                    << " - это файл, а не директория"
                    << std::endl;
            }
            else if (fs::is_directory(p))
            {
                if (isGoodDir(p.c_str()))
                {
                    parse(p, _options.getScanLevel());
                }
            }
        } 
        else
        {
            std::cerr << "Директории " 
                << *it
                << " не существует"
                << std::endl;
        }
    }
    obrFiles();
}

void Bayan::parse(const fs::path &dirpath, uint32_t scanLevel)
{
    auto minSize = _options.getMinFileSize();
    auto dirEntry = fs::directory_iterator(dirpath);

    for (auto it = fs::begin(dirEntry); it != fs::end(it); ++it)
    {
        if (fs::is_regular_file(*it)) {
            if (isGoodFile(fs::path(*it).filename().string())
                && fs::file_size(*it) >= minSize)
            {
                _files.push_back(it->path().c_str());
            }
        }
        else if (fs::is_directory(*it))
        {
            if (isGoodDir(it->path().c_str()))
            {
                if (scanLevel != 0)
                {
                    parse(*it, scanLevel - 1);
                }
            }
        }
    }
}

void Bayan::obrFiles()
{
    for (auto ffile = _files.begin(); ffile < _files.end(); ++ffile)
    {
        vstr group{*ffile};
        for (auto sfile = ffile + 1; sfile < _files.end(); ++sfile)
        {
            if (isDuplicat(*ffile, *sfile))
            {
                group.push_back(std::move(*sfile));
                sfile = _files.erase(sfile);
                ffile = _files.begin();
            }
        }

        if (group.size() != 1)
        {
            _groups.push_back(std::move(group));
        }
    }
}

Bayan::~Bayan()
{

}

bool Bayan::isGoodDir(const std::string &dirpath)
{
    boost::smatch what;
    for (auto& regx : _options.getExcludeDirs())
    {
        boost::regex reg(regx);

        if (boost::regex_search(dirpath, what, reg, boost::match_any))
        {
            return false;
        }
    }
    return true;
}

bool Bayan::isGoodFile(const std::string &filename)
{
    boost::smatch what;
    for (auto& regx : _options.getFileMasks())
    {
        try 
        {
            boost::regex reg(regx, boost::regex_constants::icase);
            auto r = boost::regex_match(filename, what, reg, boost::match_extra);
            if (r)
            {
                return true;
            }
        } catch(const std::exception& ec)
        {
            std::cout << "exception: " << ec.what() << std::endl;
        }
    }

    return _options.getFileMasks().empty();
}

bool Bayan::isDuplicat(const std::string &file1, const std::string &file2)
{
    const auto& blockSize = _options.getBlockSize();

    std::string buf1(blockSize, '\0'), buf2(blockSize, '\0');

    bool result = false;

    std::ifstream ifs1(file1);
    std::ifstream ifs2(file2);

    for(size_t i = 0; ; ++i)
    {
        auto c1 = getFromCache(file1, i);
        auto c2 = getFromCache(file2, i);

        size_t rSize1 = c1.empty() ? ifs1.readsome((char*)buf1.data(), blockSize) : blockSize;
        size_t rSize2 = c2.empty() ?  ifs2.readsome((char*)buf2.data(), blockSize) : blockSize;

        if (rSize1 == 0 && rSize2 == 0) break;

        auto hash1 = c1.empty() ? calcMD5(buf1) : getFromCache(file1, i);
        auto hash2 = c2.empty() ? calcMD5(buf2) : getFromCache(file2, i); 

        if (c1.empty()) addToCache(file1, hash1);
        if (c2.empty()) addToCache(file2, hash2);

        if (hash1 != hash2) 
        {
            result = false;
            break;
        } 
        else
        {
            result = true;
        }
    }

    return result;
}

std::string Bayan::calcMD5(const std::string& bytes)
{
    detail::md5 hash;
    detail::md5::digest_type digest;

    hash.process_bytes(bytes.data(), bytes.size());
    hash.get_digest(digest);

    const auto charDigest = reinterpret_cast<const char *>(&digest);
    std::string result;
    boost::algorithm::hex(charDigest, charDigest + sizeof(detail::md5::digest_type), std::back_inserter(result));
    return result;
}

size_t Bayan::getFileSize(const std::string &file)
{
    std::ifstream ifs(file, std::fstream::ate | std::fstream::in);

    return ifs.tellg();
}

void Bayan::addToCache(const std::string &filename, const std::string &hash)
{
    auto it = _cache.find(filename);

    if (it == _cache.end())
    {
        auto res = _cache.insert(std::make_pair(filename, vstr()));
        if (!res.second) return;
        it = res.first;
    }

    it->second.push_back(hash);
}

std::string Bayan::getFromCache(const std::string &filename, const size_t step)
{
    std::string result;
    
    if (auto it = _cache.find(filename); it != _cache.end())
    {
        if (it->second.size() > step)
        {
            result = it->second.at(step);
        }
    }

    return result;
}
