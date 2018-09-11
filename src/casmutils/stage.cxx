#include "casmutils/stage.hpp"
#include "casm/clex/PrimClex.hh"
#include "casm/clex/ScelEnum.hh"
#include <casm/CASM_global_definitions.hh>
#include <casm/crystallography/Structure.hh>
#include <fstream>
#include <iostream>

#include "casmutils/exceptions.hpp"
#include "casmutils/structure.hpp"
#include <boost/filesystem.hpp>
#include <casm/casm_io/VaspIO.hh>
#include <casm/crystallography/Niggli.hh>
#include <set>

namespace Simplicity
{

// Enumerates all superlattices for a given volume and returns their niggli forms
std::vector<CASM::Lattice> make_niggli_superlattices(const CASM::Lattice& unit_lat, int vol)
{
    std::vector<CASM::Lattice> niggli_lattices;
    CASM::ScelEnumProps enum_props(vol, vol + 1); // Enumerates over a range of volumes
    CASM::SupercellEnumerator<CASM::Lattice> lat_enumerator(unit_lat, enum_props);

    for (const auto& lat : lat_enumerator)
    {
        auto lat_niggli = niggli(lat, CASM::TOL);
        niggli_lattices.push_back(lat_niggli);
    }

    return niggli_lattices;
}

// Finds the surface area from lattice paramteres
double lattice_surface_area(const CASM::Lattice& lat)
{
    Eigen::Vector3d a = lat[0];
    Eigen::Vector3d b = lat[1];
    Eigen::Vector3d c = lat[2];

    double ab = a.cross(b).norm();
    double bc = b.cross(c).norm();
    double ca = c.cross(b).norm();

    return std::abs(ab) + std::abs(bc) + std::abs(ca);
}

// Score for determining level of boxiness
double boxy_score(const CASM::Lattice& lat)
{
    // More volume per surface area means more boxy
    return std::abs(lat.vol()) / lattice_surface_area(lat);
}

// Finds the supercell with the highest volume/surface_area
// Assumes that the input has structures of same volume
CASM::Lattice most_boxy(std::vector<CASM::Lattice>& lattices)
{
    double running_score = 0;
    CASM::Lattice boxiest_lat = lattices[0];
    for (const auto& lat : lattices)
    {
        double candidate_score = boxy_score(lat);
        if (candidate_score > running_score)
        {
            running_score = candidate_score;
            boxiest_lat = lat;
        }
    }

    return boxiest_lat;
}

// Given a structure and volume, finds the most boxy supercell
Rewrap::Structure make_boxy_structure(const Rewrap::Structure& unit_struc, int vol)
{
    auto unit_lat = unit_struc.lattice();
    auto lattices = make_niggli_superlattices(unit_lat, vol);
    auto most_boxy_lat = most_boxy(lattices);
    Rewrap::Structure boxiest_struc = unit_struc.create_superstruc(most_boxy_lat);
    return boxiest_struc;
}

} // namespace Simplicity
