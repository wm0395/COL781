#ifndef RASTER_HPP
#define RASTER_HPP

#include "helper.hpp"
#include "geometric.hpp"

namespace raster{
    void anti_alias(Geometric::triangle& T, int spp, float color, helper::frameData* frame);
}

#endif