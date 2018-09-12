#ifndef UTILS_STAGE_HH
#define UTILS_STAGE_HH

#include "casmutils/definitions.hpp"
#include "casmutils/structure.hpp"
#include <casm/CASM_global_definitions.hh>
#include <iostream>

namespace Simplicity
{
/// Find all the niggli super lattices for a given volume
std::vector<CASM::Lattice> make_niggli_superlattices(const CASM::Lattice& unit_lat, int vol);

/// Find the most boxy lattice of the ones given based on volume/surface_area ratio
CASM::Lattice most_boxy(const std::vector<CASM::Lattice>& lattices);

/// Find the supercell of given volume with the highest volume/surface_area ratio
Rewrap::Structure make_boxy_superstructure(const Rewrap::Structure& unit_struc, int vol);

} // namespace Simplicity
#endif
