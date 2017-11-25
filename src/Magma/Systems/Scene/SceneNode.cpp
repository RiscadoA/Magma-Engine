#include "SceneNode.hpp"

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
