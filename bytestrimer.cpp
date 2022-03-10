#pragma once
#include <vector>
#include<sys/socket.h>
#include <iostream>
class ByteStream 
{
	public:
	std::vector<char> buffer;
	void writeInt(int i);
	void writeVInt(int i);
	void writeString(std::string s);
	void writeByte(int i);
	void writeShort(int i);
	void writeInt24(int i);
	void resize(int size);
	void write(int ID, int client);
		
	int readInt();
	int readVInt();
	std::string readString();
	int readByte();
	int readInt24();
	int readShort();
	
	void destroy();
};
void ByteStream::destroy()
{
	buffer.clear();
}
void ByteStream::write(int ID, int client)
{
	auto packet = buffer;
	buffer.clear();
	writeShort(ID);
	writeInt24(packet.size());
	writeShort(10);//version
	std::copy(packet.begin(), packet.end(), std::back_inserter(buffer));
	send(client, buffer.data(), 7+packet.size(), 0);
}
void ByteStream::resize(int size)
{
	buffer.resize(size);
}
void ByteStream::writeInt(int i)
{
	buffer.push_back(i>>24);
	buffer.push_back(i>>16);
	buffer.push_back(i>>8);
	buffer.push_back(i);
}
void ByteStream::writeVInt(int i)
{
	bool rotate = true;if (i == 0){writeByte(0);return;}i = (i << 1) ^ (i >> 31);while (i)
	{int tmp = i & 0x7f;if (i >= 0x80){tmp |= 0x80;}if (rotate == true){rotate = false;int l = tmp & 0x1;int b = (tmp & 0x80) >> 7;tmp >>= 1;tmp = tmp & ~0xC0;tmp = tmp | (b << 7) | (l << 6);};buffer.push_back(tmp & 0xFF);i >>= 7;}//dudnik methods but ok
}
void ByteStream::writeString(std::string s)
{
	writeInt(s.length());
	std::copy(s.begin(), s.end(), std::back_inserter(buffer));
}
void ByteStream::writeByte(int i)
{
	buffer.push_back(i);
}
void ByteStream::writeShort(int i)
{
	buffer.push_back(i>>8);
	buffer.push_back(i);
}
void ByteStream::writeInt24(int i)
{
	buffer.push_back(i>>16);
	buffer.push_back(i>>8);
	buffer.push_back(i);
}
int ByteStream::readByte()
{
	int b=buffer[0];
	buffer.erase(buffer.begin());
	return b;
}
int ByteStream::readInt()
{
	int b=buffer[0] << 24+buffer[1] << 16+buffer[2] << 8+buffer[3];
	buffer.erase(buffer.begin(),buffer.begin()+4);
	return b;
}
int ByteStream::readInt24()
{
	int b=buffer[0] << 16+buffer[1] << 8+buffer[2];
	buffer.erase(buffer.begin(),buffer.begin()+3);
	return b;
}
int ByteStream::readShort()
{
	int b=buffer[0] << 8+buffer[1];
	buffer.erase(buffer.begin(),buffer.begin()+2);
	return b;
}
std::string ByteStream::readString()
{
	int len = readInt();
	if (len == -1 || len == 65535)
	{
		return "";
	}
	std::string s;
	std::copy(buffer.begin(), buffer.begin()+len, std::back_inserter(s));
	return s;
}
int ByteStream::readVInt()
{
	int result = 0,shift = 0,r,msb,bit7;while (true){int b = readByte();if (shift == 0){bit7 = (b & 0x40) >> 6;msb = (b & 0x80) >> 7;r = b << 1;r = r & ~0x181;b = r | (msb << 7) | bit7;}result |= (b & 0x7f) << shift;shift += 7;if (!(b & 0x80)){break;}}result = (result >> 1) ^ (-(result & 1));return result;
}
//int main()
//{
//	ByteStream bs;
//	bs.writeInt(9);
//	bs.writeVInt(9339);
//	for (int i=0; i<12; i++)
//	{
//		printf("%02x", bs.buffer[i]);
//	}
//}