#include "StreamWriter.h"


int StreamWriter::write(const void* buffer, size_t size)
{
	auto buf = (const unsigned char*)(buffer);

	if (mStream.size() < mPosition + size)
	{
		mStream.resize(mPosition + size);
	}

	for (size_t i = 0; i < size; ++i)
	{
		mStream[i + mPosition] = buf[i];
	}

	mPosition += size;

	return size;
}


/**
 * Moved read position to \c pos.
 */
void StreamWriter::seek(int pos)
{
	mPosition += pos;
}
