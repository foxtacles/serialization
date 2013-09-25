#ifndef PACKETEXTENSIONS_H
#define PACKETEXTENSIONS_H

#include "RakNet.hpp"

class pReferenceDefault : public pPacket
{
		friend class PacketFactory;

	protected:
		pReferenceDefault(pTypes type) : pPacket(type)
		{

		}

		pReferenceDefault(pTypes type, RakNet::NetworkID id) : pPacket(type)
		{
			construct(id);
		}

		pReferenceDefault(const unsigned char* stream, unsigned int len) : pPacket(stream, len)
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
class pGeneratorReferenceExtend : public pReferenceDefault
{
		friend class PacketFactory;

	private:
		pGeneratorReferenceExtend(const pPacket& sub, const Args&... args) : pReferenceDefault(Type)
		{
			construct(sub, std::forward<const Args&>(args)...);
		}
		pGeneratorReferenceExtend(const unsigned char* stream, unsigned int len) : pReferenceDefault(stream, len)
		{

		}

		void access(Args&... args) const
		{
			deconstruct(std::forward<Args&>(args)...);
		}
};

#endif
