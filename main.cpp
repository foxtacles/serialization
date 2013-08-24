#include "examples/ComplexObject.hpp"

#include <cassert>

int main()
{
	// initializes with default data
	ComplexObject C1;

	// creates packet
	auto packet = C1.toPacket();

	// initializes with packet
	ComplexObject C2(packet);

	// to initialize from network data:
	// PacketFactory::Init

	assert(C1 == C2);

	return 0;
}
