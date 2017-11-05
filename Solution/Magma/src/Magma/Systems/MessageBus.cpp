#include "MessageBus.hpp"

#include <algorithm>

std::map<std::string, size_t> Magma::Message::s_types = {};
size_t Magma::Message::s_nextTypeID = 1;

std::string Magma::Message::TypeIDToTypeName(size_t type)
{
	auto it = std::find_if(s_types.begin(), s_types.end(), [&](std::pair<std::string, size_t> p) { return p.second == type; });
	if (it == s_types.end())
	{
		MAGMA_WARNING("Failed to get message type name from type ID, this message type doesn't exist (" + std::to_string(type) + ")");
		return "";
	}
	return it->first;
}

size_t Magma::Message::TypeNameToTypeID(const std::string & type)
{
	auto it = s_types.find(type);
	if (it == s_types.end())
	{
		s_types.insert(std::make_pair(type, s_nextTypeID));
		return s_nextTypeID++;
	}
	return it->second;
}

Magma::MessageListener::~MessageListener()
{
	if (m_msgBus != nullptr)
		MAGMA_ERROR("Please class MessageListener::Terminate method before destroying one");
}

void Magma::MessageListener::Init(std::shared_ptr<MessageBus> msgBus)
{
	m_msgBus = msgBus;
	this->DerivedInit();
}

void Magma::MessageListener::Terminate()
{
	this->DerivedTerminate();
	this->UnsubscribeFromAll();
	m_msgBus = nullptr;
}

void Magma::MessageListener::SubscribeToAll()
{
	for (auto s : m_subscriptions)
		m_msgBus->Unsubscribe(this->shared_from_this(), s);
	m_subscriptions.clear();
	m_subscriptions.insert(0);
	m_msgBus->Subscribe(this->shared_from_this(), 0);
}

void Magma::MessageListener::UnsubscribeFromAll()
{
	for (auto s : m_subscriptions)
		m_msgBus->Unsubscribe(this->shared_from_this(), s);
	m_subscriptions.clear();
}

void Magma::MessageListener::Subscribe(size_t type)
{
	if (type == 0)
	{
		MAGMA_WARNING("Failed to subscribe MessageListener to messages of type 0, invalid type ID");
		return;
	}
	if (m_subscriptions.find(0) == m_subscriptions.end())
	{
		m_subscriptions.insert(type);
		m_msgBus->Subscribe(this->shared_from_this(), type);
	}
}

void Magma::MessageListener::Unsubscribe(size_t type)
{
	if (type == 0)
	{
		MAGMA_WARNING("Failed to unsubscribe MessageListener from messages of type 0, invalid type ID");
		return;
	}
	if (m_subscriptions.find(0) != m_subscriptions.end())
	{
		MAGMA_WARNING("Failed to unsubscribe MessageListener from messages of type " + std::to_string(type) +", MessageListener is subscribed to every type");
		return;
	}
	m_subscriptions.erase(type);
	m_msgBus->Unsubscribe(this->shared_from_this(), type);
}

size_t Magma::MessageListener::Subscribe(const std::string & type)
{
	auto i = Message::TypeNameToTypeID(type);
	this->Subscribe(i);
	return i;
}

size_t Magma::MessageListener::Unsubscribe(const std::string & type)
{
	auto i = Message::TypeNameToTypeID(type);
	this->Unsubscribe(i);
	return i;
}

Magma::MessageHandle Magma::MessageListener::PopMessage()
{
	std::lock_guard<std::mutex> _lockguard(m_msgQueueMutex);
	if (!m_msgQueue.empty())
	{
		auto msg = m_msgQueue.front();
		m_msgQueue.pop();
		return msg;
	}
	return nullptr;
}

void Magma::MessageListener::PushMessage(MessageHandle msg)
{
	std::lock_guard<std::mutex> _lockguard(m_msgQueueMutex);
	m_msgQueue.push(msg);
}

Magma::MessageBus::MessageBus(size_t messageMaxSize, size_t messageBufferSize)
	: m_messageMaxSize(messageMaxSize), m_messageBufferSize(messageBufferSize)
{
	m_messageBuffer = static_cast<char*>(std::malloc(m_messageMaxSize * m_messageBufferSize));
	std::memset(m_messageBuffer, 0, m_messageMaxSize * m_messageBufferSize);
	m_messageBufferState = new bool[m_messageBufferSize];
	std::memset(m_messageBufferState, 0, m_messageBufferSize);

	m_listeners.resize(1);
}

Magma::MessageBus::~MessageBus()
{
	m_listeners.clear();

	this->Clean();
	delete[] m_messageBufferState;
	free(m_messageBuffer);
}

void Magma::MessageBus::Clean()
{
	std::lock_guard<std::mutex> _lockguard(m_messagesMutex);
	for (auto it = m_messages.begin(); it != m_messages.end();)
	{
		auto i = it;
		++it;
		if ((*i)->m_refCount == 0)
		{
			(*i)->~Message();
			size_t msgIndex = (reinterpret_cast<char*>(*i) - m_messageBuffer) / m_messageMaxSize;
			std::memset(m_messageBuffer + (msgIndex * m_messageMaxSize), 0, m_messageMaxSize);
			m_messageBufferState[msgIndex] = false;
			m_messages.erase(i);
		}
	}
}

void Magma::MessageBus::SendMessage(Message * msg)
{
	std::lock_guard<std::recursive_mutex> _lockguard(m_listenersMutex);
	for (auto& l : m_listeners[0])
		l->PushMessage(msg);
	for (auto it = m_listeners.begin() + 1; it != m_listeners.end(); ++it)
		for (auto& l : *it)
			l->PushMessage(msg);
}

void Magma::MessageBus::Subscribe(std::shared_ptr<MessageListener> listener, size_t msgType)
{
	std::lock_guard<std::recursive_mutex> _lockguard(m_listenersMutex);
	if (m_listeners.size() <= msgType)
		m_listeners.resize(msgType + 1);
	m_listeners[msgType].push_back(listener);
}

void Magma::MessageBus::Unsubscribe(std::shared_ptr<MessageListener> listener, size_t msgType)
{
	std::lock_guard<std::recursive_mutex> _lockguard(m_listenersMutex);
	for (size_t i = 0; i < m_listeners[msgType].size(); ++i)
		if (m_listeners[msgType][i] == listener)
			m_listeners[msgType].erase(m_listeners[msgType].begin() + i);
}

char * Magma::MessageBus::AllocateMessage()
{
	for (size_t i = 0; i < m_messageBufferSize; ++i)
		if (m_messageBufferState[i] == false)
			return &m_messageBuffer[i * m_messageMaxSize];
	MAGMA_ERROR("Failed to allocate message on MessageBus, the message buffer is full");
	return nullptr;
}
