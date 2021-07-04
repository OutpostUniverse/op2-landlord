#pragma once

#include <string>
#include <utility>


class StreamReader
{
public:
	StreamReader(std::string stream) : mStream(std::move(stream)) {}
	StreamReader() {}
	~StreamReader() {}

	int read(void* buffer, size_t size);
	void seek(int pos);

private:
	int								mPosition = 0;

	std::string		mStream;
};
