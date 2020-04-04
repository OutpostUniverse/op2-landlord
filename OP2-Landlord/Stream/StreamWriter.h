#pragma once

#include <vector>


class StreamWriter
{
public:
	StreamWriter() {}
	~StreamWriter() {}

	int write(const void* buffer, size_t size);
	void seek(int pos);

	size_t size() { return mStream.size(); }
	const unsigned char* data() { return mStream.data(); }

private:
	int mPosition = 0;
	std::vector<unsigned char> mStream;
};
