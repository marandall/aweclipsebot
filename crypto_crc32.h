#pragma once

class CrcTool
{
protected: 
      ULONG crc32_table[256];  // Lookup table array 

public:
	void Init_CRC32_Table() 
	{
		  // This is the official polynomial used by CRC-32 
		  // in PKZip, WinZip and Ethernet. 
		  ULONG ulPolynomial = 0x04c11db7; 

		  // 256 values representing ASCII character codes. 
		  for(int i = 0; i <= 0xFF; i++) 
		  { 
				crc32_table[i]=Reflect(i, 8) << 24; 
				for (int j = 0; j < 8; j++) 
					  crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0); 
				crc32_table[i] = Reflect(crc32_table[i], 32); 
		  } 
	} 

	ULONG Reflect(ULONG ref, char ch) 
	{

		  ULONG value(0); 

		  // Swap bit 0 for bit 7 
		  // bit 1 for bit 6, etc. 
		  for(int i = 1; i < (ch + 1); i++) 
		  { 
				if(ref & 1) 
					  value |= 1 << (ch - i); 
				ref >>= 1; 
		  } 
		  return value; 
	} 

	int Get_CRC(std::string text) 
	{
		  // Start out with all bits set high. 
		  ULONG  ulCRC(0xffffffff); 
		  int len; 
		  unsigned char* buffer; 

		  // Get the length. 
		  len = (int)text.length(); 
		  // Save the text in the buffer. 
		  buffer = (unsigned char*)text.c_str(); 
		  // Perform the algorithm on each character 
		  // in the string, using the lookup table values. 
		  while(len--) 
				ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF) ^ *buffer++]; 
		  // Exclusive OR the result with the beginning value. 
		  return ulCRC ^ 0xffffffff; 
	} 

    CrcTool()
	{
		Init_CRC32_Table();
	}

};