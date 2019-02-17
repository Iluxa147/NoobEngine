#pragma once

#include "NEstreams.h"

class ShaderManager
{
public:
	ShaderManager(const char* name) : fstream_(name), src_(nullptr) {}
	~ShaderManager() { delete[] src_; };

	const char* getSrc()
	{
		unsigned int length = fstream_.getFileBytesize();
		src_ = new char[length+1];
		fstream_.read(src_, length);
		src_[length] = '\0';
		return src_; 
	}


private:
	FStream fstream_;
	char* src_;
};