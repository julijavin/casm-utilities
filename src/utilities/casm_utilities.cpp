#include <iostream>

#include "lib/completer/handlers.hpp"
#include "lib/splash/splash.hpp"
#include "lib/structure/structure.hpp"

using namespace casmUtilities;

/**
 * Construct a temporary Option object of a particular
 * type to determine what the appropriate invocation
 * of casm-utilities command should be.
 */

template <typename T>
std::string option_desc()
{
    T dumbopt;
    return dumbopt.tag();
}

void basic_help()
{
    std::cout<<"The available options for casm-utilities are:"<<std::endl;
    std::cout<<"    "<<option_desc<casmUtilitiesCompletion::SplashOption>()<<std::endl;
    std::cout<<std::endl;

    return;
}

int main(int argc, char *argv[])
{
    if(argc==1)
    {
        basic_help();
        return 0;
    }

    if(argv[1]==option_desc<casmUtilitiesCompletion::SplashOption>())
    {
        splash_utility_launch(argc, argv);
    }

    else if(argv[1]==option_desc<casmUtilitiesCompletion::StructureOption>())
    {
        structure_utility_launch(argc, argv);
    }

    else
    {
        std::cout<<"Unrecognized option"<<std::endl;
    }

    return 0;
}
