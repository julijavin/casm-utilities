#include "casmutils/definitions.hpp"
#include "casmutils/handlers.hpp"
#include "casmutils/stage.hpp"
#include "casmutils/structure.hpp"
#include <boost/program_options.hpp>
#include <casm/crystallography/Structure.hh>
#include <fstream>
#include <iostream>

namespace Utilities
{

void super_boxy_initializer(po::options_description& super_boxy_desc)
{
    UtilityProgramOptions::add_help_suboption(super_boxy_desc);
    UtilityProgramOptions::add_desc_suboption(super_boxy_desc);
    UtilityProgramOptions::add_output_suboption(super_boxy_desc);

    super_boxy_desc.add_options()("structure,s", po::value<fs::path>()->required(),
                                  "POS.vasp like file that you want to get the boxy supercell for.");
    super_boxy_desc.add_options()("volume,v", po::value<int>()->default_value(1),
                                  "Desired volume of the structure. Defaults to 1.");

    return;
}
} // namespace Utilities

using namespace Utilities;

int main(int argc, char* argv[])
{
    Handler super_boxy_launch(argc, argv, super_boxy_initializer);

    if (super_boxy_launch.count("help"))
    {
        std::cout << super_boxy_launch.desc() << std::endl;
        return 1;
    }

    try
    {
        super_boxy_launch.notify();
    }

    catch (po::required_option& e)
    {
        std::cerr << e.what() << std::endl;
        return 2;
    }

    // get structure input from POSCAR.vasp type file
    auto my_path = super_boxy_launch.fetch<fs::path>("structure");
    auto my_struc = Rewrap::Structure(my_path);

    // get volume input
    auto my_vol = super_boxy_launch.fetch<int>("volume");

    // find most boxy structure of given volume
    auto my_boxy_struc = Simplicity::make_boxy_structure(my_struc, my_vol);

    // output
    if (super_boxy_launch.vm().count("output"))
    {
        Simplicity::write_poscar(my_boxy_struc, super_boxy_launch.fetch<fs::path>("output"));
        return 0;
    }
    else
    {
        Simplicity::print_poscar(my_boxy_struc, std::cout);
        return 0;
    }
}
