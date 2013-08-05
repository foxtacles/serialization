#include "ComplexObject.h"

ComplexObject::ComplexObject() : justBecauseItWorks({{
	{ // array
		{ // vector
			{ // map
				{ // value_type (pair)
					"key1", // string
					{
						// pair
						{"first", "second"},
						{"first", "second"},
						{"first", "second"},
						{"first", "second"},
					}
				},
				{
					"key2",
					{
						{"first", "second"},
						{"first", "second"},
						{"first", "second"},
						{"first", "second"},
					}
				},
			}
		}
	},
	{
		{
			{
				{
					"key1",
					{
						{"first", "second"},
						{"first", "second"},
						{"first", "second"},
						{"first", "second"},
					}
				},
				{
					"key2",
					{
						{"first", "second"},
						{"first", "second"},
						{"first", "second"},
						{"first", "second"},
					}
				},
			}
		}
	},
}})
{ }

ComplexObject::ComplexObject(const pPacket& packet)
{
	// reading the packet
	PacketFactory::Access<pTypes::ID_COMPLEX_OBJECT>(packet, justBecauseItWorks);
}

pPacket ComplexObject::toPacket() const
{
	// creating the packet
	return PacketFactory::Create<pTypes::ID_COMPLEX_OBJECT>(justBecauseItWorks);
}
