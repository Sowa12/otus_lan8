#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <options.h>
#include <bayan.h>
#include <format>

TEST(TestOptins, Positive)
{
    Options options;

    std::vector<std::string> inc_dirs {
        "/test",
        "/test2",
        "/test3"
    };

    std::vector<std::string> exc_dirs {
        "/usr",
        "/var",
        "/tmp",
        "/lib"
    };

    std::vector<std::string> file_masks {
        ".*txt",
        "test.txt",
        "f.*log",
        ".*xml"
    };

    const size_t block_size = 1024;

    std::vector<std::string> test_args{"program-name"};

    for (auto& i : inc_dirs){
        test_args.push_back("-I");
        test_args.push_back(i);
    }
    
    for (auto& i : exc_dirs){
        test_args.push_back("-E");
        test_args.push_back(i);
    }

    for (auto& i : file_masks){
        test_args.push_back("-F");
        test_args.push_back(i);
    }

    test_args.push_back("--block-size");
    test_args.push_back(std::to_string(block_size));

    char** argv;

    size_t argc = test_args.size();

    argv = new char*[argc];

    for (size_t i = 0; i < argc; ++i)
    {
        auto& test_arg = test_args.at(i);
        argv[i] = new char[test_arg.size()];
        std::strcpy(argv[i], test_arg.c_str());
    }

    options.parse(argc, argv);

    EXPECT_EQ(options.getBlockSize(), block_size);
    
    auto& inc = options.getIncludeDirs();

    EXPECT_EQ(inc.size(), inc_dirs.size());

    for (auto& i : inc_dirs)
    {
        auto fit = std::find(inc.begin(), inc.end(), i);

        EXPECT_NE(fit, inc.end());
    }
}


int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}