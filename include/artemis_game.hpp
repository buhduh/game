#ifndef ARTEMIS_GAME_HPP
#define ARTEMIS_GAME_HPP

#include <iostream>

#define kilobytes(SIZE) (1024 * (SIZE))
#define megabytes(SIZE) (1024 * kilobytes(SIZE))
#define gigabytes(SIZE) (1024 * megabytes(SIZE))

#define STD_LOG(MSG) (std::cout << MSG << std::endl)

//I would much rather put this in artemis_asset.hpp
//but this causes forward declaration headaches from
//the statically defined MeshHeader list
#define MAX_ASSET_NAME UINT8_MAX

#endif
