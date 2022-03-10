#pragma once
#include "bytestrimer.cpp"
class LoginMessage
{
	public:
	int client;
	void encode();
};
void LoginMessage::encode()
{
	ByteStream bs;
	bs.writeInt(0);
	bs.writeInt(0);//lowID
	bs.writeString("");//token
	bs.writeInt(-1);//major
	bs.writeInt(25565);//minor
	bs.writeInt(1);//build
	bs.write(10101, client);
}