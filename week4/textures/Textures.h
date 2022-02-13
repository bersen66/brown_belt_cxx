//
// Created by ortur on 12.02.2022.
//

#ifndef BROWN_BELT_TEXTURES_H
#define BROWN_BELT_TEXTURES_H
#pragma once

#include "Common.h"

#include <memory>

std::unique_ptr<ITexture> MakeTextureSolid(Size size, char pixel);

std::unique_ptr<ITexture> MakeTextureCheckers(Size size, char pixel1,
                                              char pixel2);

std::unique_ptr<ITexture> MakeTextureCow();

#endif //BROWN_BELT_TEXTURES_H
