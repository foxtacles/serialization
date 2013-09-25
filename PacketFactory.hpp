#ifndef PACKETFACTORY_H
#define PACKETFACTORY_H

#include "PacketTypes.hpp"

#include <list>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <array>
#include <tuple>
#include <stdexcept>
#include <cstring>
#include <memory>

template<pTypes>
struct pTypesMap;

class pDefault;
typedef pDefault pPacket;

class pDefault
{
		friend class PacketFactory;

	private:
		pDefault(const pDefault&) = delete;
		pDefault& operator=(const pDefault&) = delete;

		template<size_t>
		struct tuple_count {};

		template<typename... T, size_t N>
		void unpack_tuple(const std::tuple<T...>&, tuple_count<N>);
		template<typename... T>
		void unpack_tuple(const std::tuple<T...>&, tuple_count<0>);
		template<typename T, size_t N, size_t I>
		void unpack_array(const std::array<T, N>&, tuple_count<I>);
		template<typename T, size_t N>
		void unpack_array(const std::array<T, N>&, tuple_count<0>);

		template<typename... T, size_t N>
		void pack_tuple(std::tuple<T...>&, tuple_count<N>) const;
		template<typename... T>
		void pack_tuple(std::tuple<T...>&, tuple_count<0>) const;
		template<typename T, size_t N, size_t I>
		void pack_array(std::array<T, N>&, tuple_count<I>) const;
		template<typename T, size_t N>
		void pack_array(std::array<T, N>&, tuple_count<0>) const;

		union
		{
			std::vector<unsigned char> write;
			const unsigned char* read;
		};

		unsigned int len;
		mutable unsigned int location;

	protected:
		pDefault(pTypes type) : len(0), location(sizeof(pTypes))
		{
			new(&write) decltype(write)();
			construct(type);
		}

		pDefault(const unsigned char* stream, unsigned int len) : read(stream), len(len), location(sizeof(pTypes)) {}

		template<typename T, typename... Args>
		void construct(const T&, const Args&...);
		void construct() {}

		template<typename... Args>
		void construct(const pPacket&, const Args&...);

		template<typename... Args>
		void construct(const std::string&, const Args&...);

		template<typename T, typename... Args>
		void construct(const std::vector<T>&, const Args&...);

		template<typename T, typename... Args>
		void construct(const std::list<T>&, const Args&...);

		template<typename K, typename V, typename... Args>
		void construct(const std::map<K, V>&, const Args&...);

		template<typename K, typename V, typename... Args>
		void construct(const std::unordered_map<K, V>&, const Args&...);

		template<typename T1, typename T2, typename... Args>
		void construct(const std::pair<T1, T2>&, const Args&...);

		template<typename... T, typename... Args>
		void construct(const std::tuple<T...>&, const Args&...);

		template<typename T, size_t N, typename... Args>
		void construct(const std::array<T, N>&, const Args&...);

		template<typename T, typename... Args>
		void deconstruct(T&, Args&...) const;
		template<typename T>
		T deconstruct_single() const;
		void deconstruct() const {}

		template<typename... Args>
		void deconstruct(std::string&, Args&...) const;

		template<typename T, typename... Args>
		void deconstruct(std::vector<T>&, Args&...) const;

		template<typename T, typename... Args>
		void deconstruct(std::list<T>&, Args&...) const;

		template<typename K, typename V, typename... Args>
		void deconstruct(std::map<K, V>&, Args&...) const;

		template<typename K, typename V, typename... Args>
		void deconstruct(std::unordered_map<K, V>&, Args&...) const;

		template<typename T1, typename T2, typename... Args>
		void deconstruct(std::pair<T1, T2>&, Args&...) const;

		template<typename... T, typename... Args>
		void deconstruct(std::tuple<T...>&, Args&...) const;

		template<typename T, size_t N, typename... Args>
		void deconstruct(std::array<T, N>&, Args&...) const;

	public:
		bool readonly() const { return len; }

		pDefault() noexcept : pDefault(static_cast<pTypes>(0)) {}

		pDefault(pDefault&& p) noexcept
		{
			if (!p.readonly())
				new(&write) decltype(write)(std::move(p.write));
			else
				read = p.read;

			len = p.len;
			location = p.location;
		}

		pDefault& operator=(pDefault&& p) noexcept
		{
			if (this != &p)
			{
				if (readonly())
				{
					if (p.readonly())
						read = p.read;
					else
						new(&write) decltype(write)(std::move(p.write));
				}
				else
				{
					if (p.readonly())
					{
						using namespace std;
						write.~vector();
						read = p.read;
					}
					else
						write = std::move(p.write);
				}

				len = p.len;
				location = p.location;
			}

			return *this;
		}

		~pDefault() { using namespace std; if (!readonly()) write.~vector(); }

		const unsigned char* get() const
		{
			return readonly() ? read : &write[0];
		}

		pTypes type() const
		{
			return static_cast<pTypes>(readonly() ? *read : write[0]);
		}

		unsigned int length() const
		{
			return readonly() ? len : write.size();
		}
};

class PacketFactory
{
	private:
		PacketFactory() = delete;

		template<pTypes type, typename... Args>
		struct Create_ {
			inline static pPacket Create(Args&&... args) {
				static_assert(sizeof(typename pTypesMap<type>::type) == sizeof(pPacket), "Derived types must not contain any additional data");
				return typename pTypesMap<type>::type(std::forward<Args>(args)...);
			}
		};

		template<pTypes type>
		struct Cast_ {
			inline static const typename pTypesMap<type>::type* Cast(const pPacket* packet);
		};

		template<pTypes type, typename... Args>
		struct Access_ {
			inline static void Access(const pPacket* packet, Args&... args) {
				Cast<type>(packet)->access(std::forward<Args&>(args)...);
			}
		};

	public:
		template<pTypes type, typename... Args>
		inline static pPacket Create(Args&&... args) { return Create_<type, Args...>::Create(std::forward<Args>(args)...); }

		template<pTypes type>
		inline static const typename pTypesMap<type>::type* Cast(const pPacket* packet) { return Cast_<type>::Cast(packet); }

		template<pTypes type>
		inline static const typename pTypesMap<type>::type* Cast(const pPacket& packet) { return Cast_<type>::Cast(&packet); }

		template<pTypes type, typename... Args>
		inline static void Access(const pPacket* packet, Args&... args) { Access_<type, Args...>::Access(packet, std::forward<Args&>(args)...); }

		template<pTypes type, typename... Args>
		inline static void Access(const pPacket& packet, Args&... args) { Access_<type, Args...>::Access(&packet, std::forward<Args&>(args)...); }

		template<typename T>
		inline static T Pop(const pPacket* packet);

		template<typename T>
		inline static T Pop(const pPacket& packet) { return Pop<T>(&packet); }

		inline static pPacket Init(const unsigned char* stream, unsigned int len) { return {stream, len}; }
};

template<pTypes type>
inline const typename pTypesMap<type>::type* PacketFactory::Cast_<type>::Cast(const pPacket* packet) {
	return packet->type() == type ? static_cast<const typename pTypesMap<type>::type*>(packet) : nullptr;
}

template<typename T, typename... Args>
void pDefault::construct(const T& arg, const Args&... args)
{
	// is_trivially_copyable not implemented in GCC as of now
	static_assert(std::is_trivial<T>::value, "Type cannot be trivially copied");

	write.insert(write.end(), reinterpret_cast<const unsigned char*>(&arg), reinterpret_cast<const unsigned char*>(&arg) + sizeof(T));

	construct(std::forward<const Args&>(args)...);
}

template<typename... Args>
void pDefault::construct(const pPacket& arg, const Args&... args)
{
	const unsigned char* _data = arg.get();
	unsigned int length = arg.length();

	construct(length);
	write.insert(write.end(), _data, _data + length);

	construct(std::forward<const Args&>(args)...);
}

template<typename... Args>
void pDefault::construct(const std::string& arg, const Args&...args)
{
	size_t length = arg.length();
	const unsigned char* str = reinterpret_cast<const unsigned char*>(arg.c_str());

	write.insert(write.end(), str, str + length + 1);

	construct(std::forward<const Args&>(args)...);
}

template<typename T, typename... Args>
void pDefault::construct(const std::vector<T>& arg, const Args&...args)
{
	construct(arg.size());

	for (const auto& element : arg)
		construct(element);

	construct(std::forward<const Args&>(args)...);
}

template<typename T, typename... Args>
void pDefault::construct(const std::list<T>& arg, const Args&...args)
{
	construct(arg.size());

	for (const auto& element : arg)
		construct(element);

	construct(std::forward<const Args&>(args)...);
}

template<typename K, typename V, typename... Args>
void pDefault::construct(const std::map<K, V>& arg, const Args&...args)
{
	construct(arg.size());

	for (const auto& element : arg)
		construct(element);

	construct(std::forward<const Args&>(args)...);
}

template<typename K, typename V, typename... Args>
void pDefault::construct(const std::unordered_map<K, V>& arg, const Args&...args)
{
	construct(arg.size());

	for (const auto& element : arg)
		construct(element);

	construct(std::forward<const Args&>(args)...);
}

template<typename T1, typename T2, typename... Args>
void pDefault::construct(const std::pair<T1, T2>& arg, const Args&...args)
{
	construct(arg.first);
	construct(arg.second);
	construct(std::forward<const Args&>(args)...);
}

template<typename... T, typename... Args>
void pDefault::construct(const std::tuple<T...>& arg, const Args&...args)
{
	unpack_tuple(arg, tuple_count<sizeof...(T) - 1>());
	construct(std::forward<const Args&>(args)...);
}

template<typename T, size_t N, typename... Args>
void pDefault::construct(const std::array<T, N>& arg, const Args&...args)
{
	unpack_array(arg, tuple_count<N - 1>());
	construct(std::forward<const Args&>(args)...);
}

template<typename... T, size_t N>
void pDefault::unpack_tuple(const std::tuple<T...>& arg, tuple_count<N>)
{
	construct(std::get<N>(arg));
	unpack_tuple(arg, tuple_count<N - 1>());
}

template<typename... T>
void pDefault::unpack_tuple(const std::tuple<T...>& arg, tuple_count<0>)
{
	construct(std::get<0>(arg));
}

template<typename T, size_t N, size_t I>
void pDefault::unpack_array(const std::array<T, N>& arg, tuple_count<I>)
{
	construct(arg[I]);
	unpack_array(arg, tuple_count<I - 1>());
}

template<typename T, size_t N>
void pDefault::unpack_array(const std::array<T, N>& arg, tuple_count<0>)
{
	construct(arg[0]);
}

template<typename T, typename... Args>
void pDefault::deconstruct(T& arg, Args&... args) const
{
	arg = deconstruct_single<T>();
	deconstruct(std::forward<Args&>(args)...);
}

template<typename T>
T pDefault::deconstruct_single() const
{
	// is_trivially_copyable not implemented in GCC as of now
	static_assert(std::is_trivial<T>::value, "Type cannot be trivially copied");

	if (location + sizeof(T) > this->length())
		throw std::runtime_error("Reading past the end of packet");

	location += sizeof(T);

	return *reinterpret_cast<const T*>(&read[location - sizeof(T)]);
}

template<>
inline pPacket pDefault::deconstruct_single() const
{
	unsigned int length = deconstruct_single<unsigned int>();

	if (location + length > this->length())
		throw std::runtime_error("Reading past the end of packet");

	pPacket packet = PacketFactory::Init(&read[location], length);

	location += length;

	return packet;
}

template<typename... Args>
void pDefault::deconstruct(std::string& arg, Args&... args) const
{
	size_t length = std::strlen(reinterpret_cast<const char*>(&read[location]));

	if (location + length + 1 > this->length())
		throw std::runtime_error("Reading past the end of packet");

	arg.assign(reinterpret_cast<const char*>(&read[location]), length);

	location += length + 1;

	deconstruct(std::forward<Args&>(args)...);
}

template<typename T, typename... Args>
void pDefault::deconstruct(std::vector<T>& arg, Args&... args) const
{
	size_t size = deconstruct_single<size_t>();

	arg.resize(size);

	for (auto& element : arg)
		deconstruct(element);

	deconstruct(std::forward<Args&>(args)...);
}

template<typename T, typename... Args>
void pDefault::deconstruct(std::list<T>& arg, Args&... args) const
{
	size_t size = deconstruct_single<size_t>();

	arg.resize(size);

	for (auto& element : arg)
		deconstruct(element);

	deconstruct(std::forward<Args&>(args)...);
}

template<typename K, typename V, typename... Args>
void pDefault::deconstruct(std::map<K, V>& arg, Args&... args) const
{
	size_t size = deconstruct_single<size_t>();

	arg.clear();

	for (size_t i = 0; i < size; ++i)
	{
		std::pair<K, V> data;
		deconstruct(data);
		// arg.emplace_hint(arg.end(), move(data));
		arg.insert(std::move(data));
	}

	deconstruct(std::forward<Args&>(args)...);
}

template<typename K, typename V, typename... Args>
void pDefault::deconstruct(std::unordered_map<K, V>& arg, Args&... args) const
{
	size_t size = deconstruct_single<size_t>();

	arg.clear();

	for (size_t i = 0; i < size; ++i)
	{
		std::pair<K, V> data;
		deconstruct(data);
		arg.emplace(std::move(data));
	}

	deconstruct(std::forward<Args&>(args)...);
}

template<typename T1, typename T2, typename... Args>
void pDefault::deconstruct(std::pair<T1, T2>& arg, Args&... args) const
{
	deconstruct(arg.first);
	deconstruct(arg.second);
	deconstruct(std::forward<Args&>(args)...);
}

template<typename... T, typename... Args>
void pDefault::deconstruct(std::tuple<T...>& arg, Args&... args) const
{
	pack_tuple(arg, tuple_count<sizeof...(T) - 1>());
	deconstruct(std::forward<Args&>(args)...);
}

template<typename T, size_t N, typename... Args>
void pDefault::deconstruct(std::array<T, N>& arg, Args&... args) const
{
	pack_array(arg, tuple_count<N - 1>());
	deconstruct(std::forward<Args&>(args)...);
}

template<typename... T, size_t N>
void pDefault::pack_tuple(std::tuple<T...>& arg, tuple_count<N>) const
{
	deconstruct(std::get<N>(arg));
	pack_tuple(arg, tuple_count<N - 1>());
}

template<typename... T>
void pDefault::pack_tuple(std::tuple<T...>& arg, tuple_count<0>) const
{
	deconstruct(std::get<0>(arg));
}

template<typename T, size_t N, size_t I>
void pDefault::pack_array(std::array<T, N>& arg, tuple_count<I>) const
{
	deconstruct(arg[I]);
	pack_array(arg, tuple_count<I - 1>());
}

template<typename T, size_t N>
void pDefault::pack_array(std::array<T, N>& arg, tuple_count<0>) const
{
	deconstruct(arg[0]);
}

template<typename T>
inline T PacketFactory::Pop(const pDefault* packet) {
	return packet->deconstruct_single<T>();
}

template<pTypes Type, typename... Args>
class pGeneratorDefault : public pDefault
{
		friend class PacketFactory;

	private:
		pGeneratorDefault(const Args&... args) : pDefault(Type)
		{
			construct(std::forward<const Args&>(args)...);
		}
		pGeneratorDefault(const unsigned char* stream, unsigned int len) : pDefault(stream, len)
		{

		}

		void access(Args&... args) const
		{
			deconstruct(std::forward<Args&>(args)...);
		}
};

#define PF_PACKET(ID, generator, ...) \
	template<> struct pTypesMap<pTypes::ID> { typedef generator<pTypes::ID, __VA_ARGS__> type; };

#define PF_PACKET_E(ID, generator) \
	template<> struct pTypesMap<pTypes::ID> { typedef generator<pTypes::ID> type; };

#include "PacketExtensions.hpp"

#endif
