#pragma once

#include <atomic>
#include <map>
#include <queue>
#include <mutex>
#include <memory>
#include <set>
#include <vector>

#include "..\Utils\Utils.hpp"

namespace Magma
{
	/// <summary>
	///		Class used as packet of data to be sent to the message listeners
	/// </summary>
	struct Message
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

	protected:
		inline Message() = default;
		inline virtual ~Message() = default;

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
}