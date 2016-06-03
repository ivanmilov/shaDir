#pragma once

#include <fstream>
#include <functional>
#include "sha.h"

class Reader
{
public:
	Reader(const std::string& filename);
	~Reader();

	auto getChank(unsigned chankSize);

	void readByChank(unsigned chankSize, std::function<void(const void*, unsigned)> shaUpdate);

private:
	std::ifstream input;
	std::streampos length;
};
