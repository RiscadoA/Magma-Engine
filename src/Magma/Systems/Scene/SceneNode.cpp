#include "SceneNode.hpp"

Magma::SceneNode::SceneNode()
{

}

glm::mat4 Magma::SceneNode::GetWorldTransform()
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	if (m_parent == nullptr)
		return m_relativeToParent;
	else
		return m_relativeToParent * m_parent->GetWorldTransform();
}

glm::mat4 Magma::SceneNode::GetLocalTransform()
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	return m_relativeToParent;
}

glm::mat4 Magma::SceneNode::ToWorld(glm::mat4 local)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	if (m_parent == nullptr)
		return local * m_relativeToParent;
	else
		return local * m_relativeToParent * m_parent->GetWorldTransform();
}

glm::mat4 Magma::SceneNode::ToLocal(glm::mat4 world)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	if (m_parent == nullptr)
		return glm::inverse(m_relativeToParent) * world;
	else
		return glm::inverse(m_relativeToParent) * glm::inverse(m_parent->GetWorldTransform()) * world;
}

void Magma::SceneNode::SetParent(std::shared_ptr<SceneNode> parent)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	if (m_parent != nullptr)
		m_parent->RemoveChild(shared_from_this());
	parent->AddChild(shared_from_this());
}

void Magma::SceneNode::AddChild(std::shared_ptr<SceneNode> child)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	child->m_parent = shared_from_this();
	m_children.insert(child);
}

void Magma::SceneNode::RemoveChild(std::shared_ptr<SceneNode> child)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	child->m_parent = nullptr;
	m_children.erase(child);
}

bool Magma::SceneNode::HasChild(std::shared_ptr<SceneNode> child)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	return m_children.find(child) != m_children.end();
}

void Magma::SceneNode::Attach(std::shared_ptr<Component> component)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	m_components.insert(component);
	if (!component->IsAttached(shared_from_this()))
		component->Attach(shared_from_this());
}

void Magma::SceneNode::Dettach(std::shared_ptr<Component> component)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	if (component->IsAttached(shared_from_this()))
	{
		m_components.erase(component);
		component->Dettach();
	}
}

bool Magma::SceneNode::IsAttached(std::shared_ptr<Component> component)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	return m_components.find(component) != m_components.end();
}

void Magma::SceneNode::Serialize(std::ostream & stream) const
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	::operator<<(stream, m_relativeToParent) << std::endl;
	stream << m_components.size() << std::endl;
	::operator<<(stream, m_relativeToParent);
	for (auto& c : m_components)
		stream << *c << std::endl;
	stream << m_children.size() << std::endl;
	for (auto& c : m_children)
		stream << *c << std::endl;
}

void Magma::SceneNode::Deserialize(std::istream & stream)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	::operator>>(stream, m_relativeToParent);

	size_t compCount = 0;
	stream >> compCount;
	for (auto it = m_components.begin(); it != m_components.end();)
	{
		auto old = it;
		++it;
		old->get()->Dettach();
	}
	m_components.clear();
	for (size_t i = 0; i < compCount; ++i)
		DeserializeComponent(stream)->Attach(shared_from_this());

	size_t childrenCount = 0;
	stream >> childrenCount;
	for (auto it = m_children.begin(); it != m_children.end();)
	{
		auto old = it;
		++it;
		old->get()->SetParent(nullptr);
	}
	m_children.clear();
	for (size_t i = 0; i < childrenCount; ++i)
	{
		auto node = std::make_shared<SceneNode>();
		node->SetParent(shared_from_this());
		stream >> *node;
	}
}

Magma::Component::Component()
{
	m_node = nullptr;
}

Magma::Component::~Component()
{
	this->Dettach();
}

void Magma::Component::Attach(std::shared_ptr<SceneNode> node)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	if (m_node != nullptr)
		this->Dettach();
	m_node = node;
	m_node->Attach(shared_from_this());
}

void Magma::Component::Dettach()
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	if (m_node->IsAttached(shared_from_this()))
		m_node->Dettach(shared_from_this());
	m_node = nullptr;
}

bool Magma::Component::IsAttached(std::shared_ptr<SceneNode> node)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	return m_node == node;
}

bool Magma::Component::IsAttached()
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	return m_node != nullptr;
}

void Magma::SerializeComponent(std::ostream & ostream, Component * component)
{
	ostream << component->GetTypeName() << std::endl;
	ostream << *component;
}

Magma::Component* Magma::DeserializeComponent(std::istream & istream)
{
	std::string typeName;
	std::getline(istream, typeName);
	Component* ret = Component::Create(typeName);
	if (ret == nullptr)
	{
		MAGMA_ERROR("Failed to deserialize component because the component type \"" + typeName + "\" isn't registered");
		return nullptr;
	}
	else
	{
		istream >> *ret;
		return ret;
	}
}
