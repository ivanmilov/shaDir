#include <iostream>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <boost/filesystem.hpp>

#include "sha.h"
#include "reader.h"

std::deque<std::string> fileList(const std::string& path)
{
	boost::filesystem::path p{path};

	boost::filesystem::directory_iterator end;

	std::deque<std::string> files;
	for(boost::filesystem::directory_iterator i(p);
		i != end;
		++i)
	{
		if(boost::filesystem::is_regular_file(i->path()))
		{
			files.emplace_back(i->path().string());
		}
	}

	return files;
}

std::pair<std::string, std::string> shaSum(const std::string& file, const unsigned chankSize)
{
	Sha sha("sha512");
	std::function<void(const void*, unsigned)> shaUpdate = std::bind(&Sha::update,
															  std::ref(sha),
															  std::placeholders::_1,
															  std::placeholders::_2);

	try
	{
		Reader r(file);

		r.readByChank(chankSize, shaUpdate);

		return {sha.print(), file};
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return {"err", file};
}

void threadsSha(const std::string& dir, const unsigned chankSize, const unsigned threadsCount)
{
	auto files = fileList(dir);
	size_t count = files.size();

	std::vector<boost::shared_future<std::pair<std::string, std::string>>> futures;

	for(int i(0); i < threadsCount && !files.empty(); ++i)
	{
		futures.push_back(boost::async(boost::bind(shaSum, files.front(), chankSize)));
		files.pop_front();
	}

	while (!futures.empty())
	{
		auto j = boost::wait_for_any(futures.begin(), futures.end());
		auto fut = *j;
		futures.erase(j);
		if(!files.empty())
		{
			futures.push_back(boost::async(boost::bind(shaSum, files.front(), chankSize)));
			files.pop_front();
		}

		std::cout << fut.get().first << "\t" << fut.get().second << std::endl;
	}
}

int main(int argc, char* argv[])
{
	unsigned threadCount;
	unsigned chankSize;
	std::string dir;
	
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
	("help", "Shows this message")
	("threads,t", boost::program_options::value<unsigned>(&threadCount)->default_value(2), "threads count for calculating")
	("chanksize,s", boost::program_options::value<unsigned>(&chankSize)->default_value(10*1000*1000), "bytes to read per time")
	("dir,d", boost::program_options::value<std::string>(&dir)->default_value("../data"), "directory with files")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	
	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		return 1;
	}

	if(threadCount > boost::thread::hardware_concurrency() &&
			boost::thread::hardware_concurrency() > 0)
		std::cout << "Warning! You want to use more threads than available: " << boost::thread::hardware_concurrency() << std::endl;

	threadsSha(dir, chankSize, threadCount);

	return 0;
}
