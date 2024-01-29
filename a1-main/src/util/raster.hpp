#ifndef RASTER_HPP
#define RASTER_HPP

#include "helper.hpp"
#include "geometric.hpp"
#include<vector>
#include<iostream>
namespace raster{
    void anti_alias(Geometric::triangle& T, int spp, std::vector<float>& color, std::vector<std::vector<std::vector<float>>>& pointBuffer);
}

#endif