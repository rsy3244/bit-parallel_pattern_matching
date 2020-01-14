#include "Bitmask.h"

Bitmask::Bitmask(std::size_t size_): size(size_), BLK(size_), SRC(size_), DST(size_) {}
Bitmask::Bitmask(const std::vector<bool> &blk, const std::vector<bool> &src, const std::vector<bool> &dst):
	size(blk.size()), BLK(std::vector<bool>(blk)), SRC(std::vector<bool>(src)), DST(std::vector<bool>(dst)) {}
