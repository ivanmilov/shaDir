#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "sha.h"

Sha::Sha(const std::string& type):mdctx(nullptr)
{
 	reset();
}

Sha::~Sha()
{
	if(mdctx != nullptr)
	{
		EVP_MD_CTX_destroy(mdctx);
		mdctx = nullptr;
	}
}

void Sha::reset()
{
	if(mdctx != nullptr)
	{
		EVP_MD_CTX_destroy(mdctx);
		mdctx = nullptr;
	}
	mdctx = EVP_MD_CTX_create();
	EVP_DigestInit_ex(mdctx, EVP_sha512(), NULL);
}

void Sha::reset(const void* data, unsigned size)
{
	reset();
	update(data, size);
}

void Sha::update(const void* data, unsigned size)
{
	EVP_DigestUpdate(mdctx, data, size);
}

std::string Sha::print()
{
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);

	std::stringstream ss;

	for (int i(0); i < md_len; ++i)
		ss << std::hex << std::setfill('0') << std::setw(2) << (int)md_value[i];
		
	return ss.str();
}
