#include <iostream>
#include <options.h>
#include <bayan.h>

namespace po = boost::program_options;

int main(int argc, char** argv)
{   
    Options opt;

    try
    {
        opt.parse(argc, argv);

        if (opt.needHelp()) 
        {
            std::cout << opt.getOptions() << std::endl;
        } 
        else 
        {
            if (opt.getHashName() != "md5")
            {
                std::cout << "only md5!" << std::endl;
                return EXIT_FAILURE;
            }

            Bayan bayan(opt);

            bayan.process();

            auto group = bayan.getGroups();

            for (auto fitem = group.begin(); fitem != group.end(); ++fitem)
            {
                for (auto sitem = fitem->begin(); sitem != fitem->end(); ++sitem)
                {
                    std::cout << *sitem << std::endl;
                }
                std::cout << std::endl;
            }
        }
    } 
    catch(...)
    {
        std::cout << opt.getOptions() << std::endl;
    }
    
    return EXIT_SUCCESS;
}