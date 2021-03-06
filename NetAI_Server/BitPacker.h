#ifndef BITPACKER_H
#define BITPACKER_H

#define MAX_BUFFER 1000

class BitPacker
{
private:
	char  m_buffer[MAX_BUFFER];
	int m_byteCount;
	int m_bitCount;
	void WriteBit(int value);
	int ReadBit();
public:
	BitPacker(void);
	~BitPacker(void);
	void WriteInt(int value, int numBits);
	int ReadInt(int numBits);
	float ReadFloat(int numBits);
	void WriteFloat(float value, int numBits);

	// set the data
	inline void SetData(char * buffer)
	{
		//m_buffer = buffer;
		memcpy(&m_buffer[0],buffer,MAX_BUFFER);
	}
	inline char * GetData()
	{
		return m_buffer;
	}
	
	inline void Reset()
	{
		m_bitCount = 0;
		m_byteCount = 0;
	}
};
#endif