#ifndef PACKETEXTENSIONS_H
#define PACKETEXTENSIONS_H

#include "RakNet.h"

class pReferenceDefault : public pDefault
{
		friend class PacketFactory;

	protected:
		pReferenceDefault(pTypes type) : pDefault(type)
		{

		}

		pReferenceDefault(pTypes type, RakNet::NetworkID id) : pDefault(type)
		{
			construct(id);
		}

		pReferenceDefault(const unsigned char* stream, unsigned int len) : pDefault(stream, len)
		{

		}
};

class pReferenceNewDefault : public pReferenceDefault
{
		friend class PacketFactory;

	protected:
		pReferenceNewDefault(pTypes type) : pReferenceDefault(type)
		{

		}

		pReferenceNewDefault(pTypes type, RakNet::NetworkID id, unsigned int refID, unsigned int baseID) : pReferenceDefault(type, id)
		{
			construct(refID, baseID);
		}

		pReferenceNewDefault(const unsigned char* stream, unsigned int len) : pReferenceDefault(stream, len)
		{

		}
};

template<pTypes Type, typename... Args>
class pGeneratorReference : public pReferenceDefault
{
		friend class PacketFactory;

	private:
		pGeneratorReference(const RakNet::NetworkID& id, const Args&... args) : pReferenceDefault(Type, id)
		{
			construct(std::forward<const Args&>(args)...);
		}
		pGeneratorReference(const unsigned char* stream, unsigned int len) : pReferenceDefault(stream, len)
		{

		}

		void access(RakNet::NetworkID& id, Args&... args) const
		{
			deconstruct(id, std::forward<Args&>(args)...);
		}
};

template<pTypes Type, typename... Args>
class pGeneratorReferenceExtend : public pReferenceNewDefault
{
		friend class PacketFactory;

	private:
		pGeneratorReferenceExtend(const pPacket& sub, const Args&... args) : pReferenceNewDefault(Type)
		{
			construct(sub, std::forward<const Args&>(args)...);
		}
		pGeneratorReferenceExtend(const unsigned char* stream, unsigned int len) : pReferenceNewDefault(stream, len)
		{

		}

		void access(Args&... args) const
		{
			deconstruct(std::forward<Args&>(args)...);
		}
};

#endif
