#include <memory>
#include "reader.h"

Reader::Reader(const std::string& filename): input( filename, std::ios::binary )
{
	if (!input.is_open())
		throw std::runtime_error("Unable to open " + filename);

	input.seekg (0, std::ios::end);
	length = input.tellg();
	input.seekg (0, std::ios::beg);
}

Reader::~Reader()
{
	input.close();
}

void Reader::readByChank(unsigned chankSize, std::function<void(const void*, unsigned)> shaUpdate)
{
	if(length < chankSize)
	{
		chankSize = length;
	}
	auto buf = std::make_unique<char[]>(chankSize);

	while(input.read(buf.get(), chankSize).gcount() > 0)
	{
		shaUpdate(buf.get(), input.gcount());
	}
}
