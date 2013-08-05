#include "examples/ComplexObject.h"

using namespace std;

pPacket PacketFactory::Init(const unsigned char* stream, unsigned int len)
{
	pDefault* packet;

	switch (static_cast<pTypes>(stream[0]))
	{
		case pTypes::ID_COMPLEX_OBJECT:
			packet = new typename pTypesMap<pTypes::ID_COMPLEX_OBJECT>::type(stream, len);
			break;

		default:
			throw runtime_error("Unhandled packet type");
	}

	return pPacket(packet);
}
