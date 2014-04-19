#include "precompiled.h"
#include "BitPacker.h"

BitPacker::BitPacker(void):m_bitCount(0), m_byteCount(0)
{
	// init our buffer to all zeros
	for(int i = 0; i < MAX_BUFFER; i++)
	{
		m_buffer[i] = 0;
	}
}

BitPacker::~BitPacker(void)
{
}
void BitPacker::WriteBit(int value)
{
	  //CurrentBit is 8, so reached the end of the CurrentByte
	  if (m_bitCount==8)
	  {
		    m_bitCount=0; //reset currentbit counter
		    m_byteCount++; //move on to next byte
		    m_buffer[m_byteCount]=0; //zero the current byte.
	  }		
     //write the bit to CurrentByte at the position CurrentBit
	  m_buffer[m_byteCount] |= (value<<m_bitCount);
     //move the bit counter along the CurrentByte
	  m_bitCount++;

}
int BitPacker::ReadBit()
{
	//if CurrentReadBit is 8, then reached the end of CurrentReadByte 
	if (m_bitCount ==8)
	{
		    m_bitCount =0; //reset CurrentReadBit counter
		    m_byteCount++; //move on to next byte
			if(m_byteCount> 300)
			{
				bool breakpoint = true;
			}
	}
	//shift current byte over by CurrentReadBit bits	
    unsigned int checkBit = m_buffer[m_byteCount]>> m_bitCount;
	m_bitCount++; 
	//logical AND with 1 to see if it’ set or not, and return that value

	return (checkBit &1);
}

void BitPacker::WriteInt(int value, int numBits)
{
	int bitsWritten = 0;
	while(bitsWritten <numBits)
	{            
		int bitToWrite = value>>bitsWritten;
	    WriteBit(bitToWrite);
		bitsWritten++;            
	}      
}

int BitPacker::ReadInt(int numBits)
{
	int returnVal = 0;
	int bitsRead = 0;
	while(bitsRead<numBits)
	{
		int bitToRead = ReadBit();
		returnVal = returnVal | (bitToRead<<bitsRead); 
		bitsRead++; 
	}  
	return returnVal;    
}
float BitPacker::ReadFloat(int numBits)
{
	float value = ReadInt(numBits) / 10000;

	return value;
}
void BitPacker::WriteFloat(float value, int numBits)
{
	value *= 10000;
	WriteInt(value,numBits);
}
