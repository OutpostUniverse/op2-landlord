#pragma once

#include <NAS2D/NAS2D.h>

class StreamReader
{
public:
	StreamReader(NAS2D::File::RawByteStream& stream) : mStream(stream) {}
	StreamReader() {}
	~StreamReader() {}

	int read(void* buffer, size_t size);
	void seek(int pos);

private:
	int								mPosition = 0;

	NAS2D::File::RawByteStream		mStream;
};
