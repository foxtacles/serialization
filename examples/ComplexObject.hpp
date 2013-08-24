#ifndef COMPLEXOBJECT_H
#define COMPLEXOBJECT_H

#include "../PacketFactory.h"

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
template<> struct pTypesMap<pTypes::ID_COMPLEX_OBJECT> { typedef pGeneratorDefault<pTypes::ID_COMPLEX_OBJECT, ComplexObject::nestedThing> type; };

#endif
