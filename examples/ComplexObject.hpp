#ifndef COMPLEXOBJECT_H
#define COMPLEXOBJECT_H

#include "../PacketFactory.hpp"

#include <utility>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <array>

class ComplexObject
{
	public:
		typedef std::array<std::vector<std::map<std::string, std::list<std::pair<std::string, std::string>>>>, 2> nestedThing;

	private:
		nestedThing justBecauseItWorks;

	public:
		ComplexObject();
		ComplexObject(const pPacket& packet);

		pPacket toPacket() const;

		bool operator==(const ComplexObject& c) { return justBecauseItWorks == c.justBecauseItWorks; }
		bool operator!=(const ComplexObject& c) { return !operator==(c); }
};

// Defining the packet
PF_PACKET(ID_COMPLEX_OBJECT, pGeneratorDefault, ComplexObject::nestedThing)

#endif
