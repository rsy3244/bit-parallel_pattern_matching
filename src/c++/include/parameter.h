#pragma once

#include <cstddef>

using Data = std::size_t;
constexpr std::size_t SIGMA = 128; //alphabet size;
constexpr std::size_t WORDSIZE = sizeof(Data) * 8;
constexpr std::size_t LOG2_WORDSIZE = 6;
