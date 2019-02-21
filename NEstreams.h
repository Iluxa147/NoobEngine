#pragma once
#include <cstdio>

class FStream
{
public:
	FStream(const char* filename) : position_(0)
	{
		file_ = std::fopen(filename, "rb");
		std::fseek(file_, 0, SEEK_END);
		fileSize_ = ftell(file_);
		fseek(file_, 0, SEEK_SET);
	};
	~FStream() { fclose(file_); };

	int read(void* data, unsigned int count)
	{
		int result = std::fread(data, 1, count, file_);
		position_ += result;
		return position_;
	}
	void seek(int offset)
	{
		std::fseek(file_, offset, SEEK_CUR);
		position_ += offset;
	}

	const unsigned int getFileBytesize() const { return fileSize_; }

private:
	std::FILE *file_;
	unsigned int position_;
	unsigned int fileSize_; //in bytes
};