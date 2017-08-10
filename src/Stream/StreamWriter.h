#pragma once

#include <NAS2D/NAS2D.h>

class StreamWriter
{
public:
	StreamWriter(NAS2D::File::RawByteStream& stream) : mStream(stream) {}
	StreamWriter() {}
	~StreamWriter() {}

private:
	NAS2D::File::RawByteStream		mStream;
};
