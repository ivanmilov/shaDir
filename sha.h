#pragma once

#include <openssl/evp.h>
#include <string>

class Sha
{
public:
	Sha(const std::string& type);
	~Sha();

	void reset();
	void reset(const void* data, unsigned size);

	void update(const void* data, unsigned size);

	std::string print();

private:
	EVP_MD_CTX* mdctx;
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;
};
