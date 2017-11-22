#pragma once

#include <atomic>
#include <map>
#include <queue>
#include <mutex>
#include <memory>
#include <set>
#include <vector>
#include <iostream>
#include <list>

#include "..\Utils\Utils.hpp"
#include "..\Utils\Serializable.hpp"
#include "..\Utils\Math.hpp"

namespace Magma
{
	/// <summary>
	///		Class used as packet of data to be sent to the message listeners
	/// </summary>
	class Message : public Serializable
	{
	public:
		/// <summary>
		///		Converts this message to a derived message type
		/// </summary>
		template <typename T>
		T& As() { return *static_cast<T*>(this); }

		/// <summary>
		///		Converts a message type ID to type Name
		/// </summary>
		/// <param name="type">Message type ID</param>
		/// <returns>Message type name</returns>
		static std::string TypeIDToTypeName(size_t type);

		/// <summary>
		///		Converts a message type Name to type ID
		/// </summary>
		/// <param name="type">Message type name</param>
		/// <returns>Message type ID</returns>
		static size_t TypeNameToTypeID(const std::string& type);

		/// <summary>
		///		Gets this message type ID
		/// </summary>
		/// <returns>This message type ID</returns>
		inline size_t GetTypeID() { return m_type; }

		/// <summary>
		///		Gets this message type name
		/// </summary>
		/// <returns>This message type name</returns>
		inline std::string GetTypeName() { return TypeIDToTypeName(m_type); }

		/// <summary>
		///		Gets the number of references this message has
		/// </summary>
		/// <returns>Number of references this message has</returns>
		inline size_t GetReferenceCount() { return m_refCount; }

		/// <summary>
		///		Creates a message of a certain type from a stream
		/// </summary>
		/// <param name="typeName">Message type name</param>
		/// <param name="location">Message location</param>
		/// <param name="is">Stream</param>
		/// <returns>New message</returns>
		static Message* Create(const std::string& typeName, void* location, std::istream& is);

	protected:
		Message() = default;
		virtual ~Message() = default;

	private:
		friend class MessageHandle;
		friend class MessageBus;

		std::atomic<size_t> m_refCount;
		size_t m_type;
		static std::map<std::string, size_t> s_types;
		static size_t s_nextTypeID;
	};

	/// <summary>
	///		Handles message references
	/// </summary>
	class MessageHandle
	{
	public:
		inline MessageHandle(Message* msg) : m_message(msg) { if (m_message != nullptr) ++m_message->m_refCount; }
		inline MessageHandle(const MessageHandle& other) : m_message(other.m_message) { if (m_message != nullptr) ++m_message->m_refCount; }
		inline ~MessageHandle() { if (m_message != nullptr) --m_message->m_refCount; }
		inline Message* operator->() { return m_message; }
		inline Message& operator*() { return *m_message; }
		inline bool operator==(MessageHandle& other) { return m_message == other.m_message; }
		inline bool operator==(void* other) { return static_cast<void*>(m_message) == other; }
		inline operator bool() { return m_message != nullptr; }

	private:
		Message* m_message;
	};

	/// <summary>
	///		Listens to messages sent by the message bus
	/// </summary>
	class MessageListener : public std::enable_shared_from_this<MessageListener>
	{
	public:
		virtual ~MessageListener();

		/// <summary>
		///		Inits the message listener
		/// </summary>
		/// <param name="msgBus">Message bus this listener will listen to</param>
		void Init(std::shared_ptr<MessageBus> msgBus);

		/// <summary>
		///		Terminates this message listener
		/// </summary>
		void Terminate();

		/// <summary>
		///		Checks if this listener is subscribed to a type of message
		/// </summary>
		/// <param name="msgType">Type of message t o check</param>
		/// <returns>True if subscribed, otherwsie false</returns>
		inline bool IsSubscribedTo(size_t msgType) { return m_subscriptions.find(msgType) != m_subscriptions.end(); }

		/// <summary>
		///		Checks if this listener is subscribed to a type of message
		/// </summary>
		/// <param name="msgType">Type of message t o check</param>
		/// <returns>True if subscribed, otherwsie false</returns>
		inline bool IsSubscribedTo(const std::string& msgType) { return m_subscriptions.find(Message::TypeNameToTypeID(msgType)) != m_subscriptions.end(); }

	protected:
		virtual void DerivedInit() = 0;
		virtual void DerivedTerminate() = 0;

		/// <summary>
		///		Subscribes this listener to every message type
		/// </summary>
		/// <param name="type">Message type</param>
		void SubscribeToAll();

		/// <summary>
		///		Unsubscribes this listener from every message type
		/// </summary>
		void UnsubscribeFromAll();

		/// <summary>
		///		Subscribes this listener to a message type
		/// </summary>
		/// <param name="type">Message type</param>
		void Subscribe(size_t type);

		/// <summary>
		///		Unsubscribes this listener from a message type
		/// </summary>
		/// <param name="type"></param>
		void Unsubscribe(size_t type);

		/// <summary>
		///		Subscribes this listener to a message type
		/// </summary>
		/// <param name="type">Message type</param>
		size_t Subscribe(const std::string& type);

		/// <summary>
		///		Unsubscribes this listener from a message type
		/// </summary>
		/// <param name="type"></param>
		size_t Unsubscribe(const std::string& type);

		/// <summary>
		///		Gets next message in queue
		/// </summary>
		/// <returns>Next message in queue, nullptr if queue is empty</returns>
		MessageHandle PopMessage();

		std::shared_ptr<MessageBus> m_msgBus; // Message bus this listener is associated with

	private:
		friend class MessageBus;

		void PushMessage(MessageHandle msg);

		std::set<size_t> m_subscriptions;
		std::queue<MessageHandle> m_msgQueue;
		std::mutex m_msgQueueMutex;
	};

	/// <summary>
	///		Sends messages between message listeners
	/// </summary>
	class MessageBus final : public std::enable_shared_from_this<MessageBus>
	{
	public:
		MessageBus(size_t messageMaxSize, size_t messageBufferSize);
		~MessageBus();

		/// <summary>
		///		Sends a message of a certain type to message listeners
		/// </summary>
		template <typename T, typename ... Args>
		void SendMessage(size_t type, Args ... args);

		/// <summary>
		///		Sends a message of a certain type to message listeners
		/// </summary>
		template <typename T, typename ... Args>
		void SendMessage(const std::string& type, Args ... args);

		/// <summary>
		///		Sends a message of a certain type to message listeners, creating it from a stream
		/// </summary>
		/// <param name="type">Message type</param>
		/// <param name="typeName">Message data type</param>
		/// <param name="is">Stream from which the message will be extracted</param>
		void SendMessage(size_t type, const std::string& typeName, std::istream& is);

		/// <summary>
		///		Sends a message of a certain type to message listeners, creating it from a stream
		/// </summary>
		/// <param name="type">Message type</param>
		/// <param name="typeName">Message data type</param>
		/// <param name="is">Stream from which the message will be extracted</param>
		inline void SendMessage(const std::string& type, const std::string& typeName, std::istream& is) { SendMessage(Message::TypeNameToTypeID(type), typeName, is); }

		/// <summary>
		///		Cleans messages without references
		/// </summary>
		void Clean();

	private:
		friend class MessageListener;

		void SendMessage(Message* msg);

		void Subscribe(std::shared_ptr<MessageListener> listener, size_t msgType);
		void Unsubscribe(std::shared_ptr<MessageListener> listener, size_t msgType);

		char* AllocateMessage();

		std::vector<std::vector<std::shared_ptr<MessageListener>>> m_listeners;
		std::recursive_mutex m_listenersMutex;

		std::mutex m_messagesMutex;
		char* m_messageBuffer;
		size_t m_messageBufferSize;
		size_t m_messageMaxSize;
		bool* m_messageBufferState;
		std::list<Message*> m_messages;
	};

	template<typename T, typename ...Args>
	inline void MessageBus::SendMessage(size_t type, Args ...args)
	{
		m_messagesMutex.lock();
		if (sizeof(T) > m_messageMaxSize)
		{
			MAGMA_ERROR("Failed to send message, message size (" + std::to_string(sizeof(T)) + ") exceeds the maximum allowed (" + std::to_string(m_messageMaxSize));
			m_messagesMutex.unlock();
			return;
		}
		char* loc = this->AllocateMessage();
		if (loc == nullptr)
		{
			MAGMA_ERROR("Failed to send message in MessageBus, message allocation returned nullptr");
			m_messagesMutex.unlock();
		}

		Message* msg = new (loc) T(args...);
		msg->m_type = type;
		this->SendMessage(msg);

		m_messagesMutex.unlock();
	}

	template<typename T, typename ...Args>
	inline void MessageBus::SendMessage(const std::string & type, Args ...args)
	{
		this->SendMessage<T, Args...>(Message::TypeNameToTypeID(type), args...);
	}

	namespace Messaging
	{
		namespace Detail
		{
			using CreateRegistrableFunc = Message*(*)(void* loc);
			using RegistrableRegistry = std::map<std::string, CreateRegistrableFunc>;

			inline RegistrableRegistry& GetRegistrableRegistry()
			{
				static RegistrableRegistry reg;
				return reg;
			}

			template <class T>
			Message* CreateRegistrable(void* loc) { return new (loc) T(); }

			template <class T>
			struct RegistryEntry
			{
			public:
				static RegistryEntry<T>& Instance(const std::string& typeName)
				{
					static RegistryEntry<T> inst(typeName);
					return inst;
				}

			private:
				RegistryEntry(const std::string& typeName)
				{
					RegistrableRegistry& reg = GetRegistrableRegistry();
					CreateRegistrableFunc func = CreateRegistrable<T>;

					std::pair<RegistrableRegistry::iterator, bool> ret = reg.insert(RegistrableRegistry::value_type(typeName, func));

					if (ret.second == false)
					{
						// Registrable already registered with this name
						MAGMA_WARNING("Failed to register entry, there is already another entry with the same name (\"" + typeName + "\")");
					}
				}

				RegistryEntry(const RegistryEntry<T>&) = delete;
				RegistryEntry& operator=(const RegistryEntry<T>&) = delete;
			};
		}
	}

	/// <summary>
	///		Registers a message data type with the chosen name, so it can later be created with Message::Create
	/// </summary>
	/// <param name="TYPE">Message data type</param>
	/// <param name="NAME">Message data type name</param>
#define MAGMA_REGISTER_MESSAGE(TYPE, NAME) \
	namespace Messaging {\
	namespace Detail { \
	namespace { \
		template <class T> \
		class RegistrableRegistration; \
		\
		template <> \
		class RegistrableRegistration<TYPE> { \
			static const ::Magma::Messaging::Detail::RegistryEntry<TYPE>& reg; \
		}; \
		\
		const ::Magma::Messaging::Detail::RegistryEntry<TYPE>& \
			RegistrableRegistration<TYPE>::reg = \
				::Magma::Messaging::Detail::RegistryEntry<TYPE>::Instance(NAME); \
	}}}

	// Basic message types
	/// <summary>
	///		Empty message
	/// </summary>
	class EmptyMessage final : public Message
	{
	private:
		// Inherited via Message
		inline virtual void Serialize(std::ostream & stream) const {};
		inline virtual void Deserialize(std::istream & stream) {};
	};
	MAGMA_REGISTER_MESSAGE(EmptyMessage, "empty");
	/// <summary>
	///		Message containing 64bit integer
	/// </summary>
	class IntMessage final : public Message
	{
	public:
		int64_t m_value;
	private:
		// Inherited via Message
		inline virtual void Serialize(std::ostream & stream) const { stream << m_value; };
		inline virtual void Deserialize(std::istream & stream) { stream >> m_value; };
	};
	MAGMA_REGISTER_MESSAGE(IntMessage, "int");
	/// <summary>
	///		Message containing 64bit floating point number
	/// </summary>
	class RealMessage final : public Message
	{
	public:
		double m_value;
	private:
		// Inherited via Message
		inline virtual void Serialize(std::ostream & stream) const { stream << m_value; };
		inline virtual void Deserialize(std::istream & stream) { stream >> m_value; };
	};
	MAGMA_REGISTER_MESSAGE(RealMessage, "real");
	/// <summary>
	///		Message containing a 2D vector
	/// </summary>
	class Vector2Message final : public Message
	{
	public:
		glm::vec2 m_value;
	private:
		// Inherited via Message
		inline virtual void Serialize(std::ostream & stream) const { stream << m_value.x << " " << m_value.y << " "; };
		inline virtual void Deserialize(std::istream & stream) { stream >> m_value.x >> m_value.y; };
	};
	MAGMA_REGISTER_MESSAGE(Vector2Message, "vector2");
	/// <summary>
	///		Message containing a 2D vector
	/// </summary>
	class Vector3Message final : public Message
	{
	public:
		glm::vec3 m_value;
	private:
		// Inherited via Message
		inline virtual void Serialize(std::ostream & stream) const { stream << m_value.x << " " << m_value.y << " " << m_value.z << " "; };
		inline virtual void Deserialize(std::istream & stream) { stream >> m_value.x >> m_value.y >> m_value.z; };
	};
	MAGMA_REGISTER_MESSAGE(Vector3Message, "vector3");
	/// <summary>
	///		Message containing a 2D vector
	/// </summary>
	class Vector4Message final : public Message
	{
	public:
		glm::vec4 m_value;
	private:
		// Inherited via Message
		inline virtual void Serialize(std::ostream & stream) const { stream << m_value.x << " " << m_value.y << " " << m_value.z << " " << m_value.w << " "; };
		inline virtual void Deserialize(std::istream & stream) { stream >> m_value.x >> m_value.y >> m_value.z >> m_value.w; };
	};
	MAGMA_REGISTER_MESSAGE(Vector4Message, "vector4");
	/// <summary>
	///		Message containing string
	/// </summary>
	class StringMessage final : public Message
	{
	public:
		std::string m_value;
	private:
		// Inherited via Message
		virtual void Serialize(std::ostream & stream) const final;
		virtual void Deserialize(std::istream & stream) final;
	};
	MAGMA_REGISTER_MESSAGE(StringMessage, "string");

}