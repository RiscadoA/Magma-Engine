#pragma once

#include <glm\glm.hpp>
#include <memory>
#include <mutex>

namespace Magma
{
	/// <summary>
	///		Represents funcionality that can be attached to a scene node in the scene graph.
	///		If you want to implement custom functionality use a ScriptComponent
	/// </summary>
	class Component
	{
	public:

	private:
		friend class SceneNode;

		std::shared_ptr<SceneNode> m_node; // The node where this component is attached
	};

	/// <summary>
	///		Represents a node in the scene graph.
	///		Contains a transformation and a undefined number of components.
	/// </summary>
	class SceneNode
	{
	public:
		/// <summary>
		///		Gets this node world transform
		/// </summary>
		/// <returns>World transform</returns>
		glm::mat4 GetWorldTransform();

		/// <summary>
		///		Gets this node local transform (relative to parent)
		/// </summary>
		/// <returns>Local transform (relative to parent)</returns>
		glm::mat4 GetLocalTransform();

		/// <summary>
		///		Converts a transformation relative to this node to a world transformation
		/// </summary>
		/// <param name="local">Local transformation</param>
		/// <returns>World transformation</returns>
		glm::mat4 ToWorld(glm::mat4 local);

		/// <summary>
		///		Converts a world transformation to a local transformation relative to this node
		/// </summary>
		/// <param name="world">World transformation</param>
		/// <returns>Local transformation</returns>
		glm::mat4 ToLocal(glm::mat4 world);

	private:
		std::mutex m_mutex;

		std::shared_ptr<SceneNode> m_parent;
		glm::mat4 m_relativeToParent; // Transformation relative to parent node
	};
}