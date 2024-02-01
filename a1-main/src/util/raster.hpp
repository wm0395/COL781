#ifndef RASTER_HPP
#define RASTER_HPP

#include "helper.hpp"
#include "geometric.hpp"
#include<vector>
#include<iostream>
namespace raster{
    void anti_alias(Geometric::triangle& T, int spp, std::vector<std::vector<Software::Attribs>>& pointBuffer);
}

#endif