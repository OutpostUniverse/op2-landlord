#include "StreamReader.h"

const int	INT_4BYTE_SIZE = 4;

/**
 * Reads a specified number of bytes into a buffer.
 * 
 * \note	The buffer provided must remain valid throughout the operation.
 */
int StreamReader::read(void* buffer, size_t size)
{
	unsigned char* buf = (unsigned char*)(buffer);

	for (size_t i = 0; i < size; ++i)
	{
		buf[i] = mStream[i + mPosition];
	}

	mPosition += size;

	return size;
}


/**
 * Moved read position to \c pos.
 */
void StreamReader::seek(int pos)
{
	mPosition += pos;
}
