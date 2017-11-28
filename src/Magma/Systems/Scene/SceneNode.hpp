#pragma once

#include <memory>
#include <mutex>
#include <set>

#include "..\..\Utils\Math.hpp"
#include "..\..\Utils\Registrable.hpp"
#include "..\..\Utils\Serializable.hpp"

namespace Magma
{
	class SceneNode;

	/// <summary>
	///		Represents funcionality that can be attached to a scene node in the scene graph.
	///		If you want to implement custom functionality use a ScriptComponent
	/// </summary>
	class Component : public std::enable_shared_from_this<Component>, public Serializable, public Registrable<Component>
	{
	public:
		Component();
		virtual ~Component();

		/// <summary>
		///		Attaches this component to a new scene node.
		/// </summary>
		/// <param name="node">New scene node</param>
		void Attach(std::shared_ptr<SceneNode> node);

		/// <summary>
		///		Dettachs this component from its current scene node.
		/// </summary>
		void Dettach();

		/// <summary>
		///		Checks if this component is attached to a scene node.
		/// </summary>
		/// <param name="node">Node to check for</param>
		/// <returns>True if attached, otherwise false</returns>
		bool IsAttached(std::shared_ptr<SceneNode> node);

		/// <summary>
		///		Checks if this component is attached to a scene node.
		/// </summary>
		/// <returns>True if attached, otherwise false</returns>
		bool IsAttached();

	private:
		std::mutex m_mutex;

		std::shared_ptr<SceneNode> m_node; // The node where this component is attached
	};

	/// <summary>
	///		Serializes a component
	/// </summary>
	void SerializeComponent(std::ostream& ostream, Component* component);

	/// <summary>
	///		Deserializes a component
	/// </summary>
	Component* DeserializeComponent(std::istream& istream);

	/// <summary>
	///		Represents a node in the scene graph.
	///		Contains a transformation and a undefined number of components.
	/// </summary>
	class SceneNode final : public std::enable_shared_from_this<SceneNode>, public Serializable
	{
	public:
		SceneNode();

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

		/// <summary>
		///		Sets this scene node parent. Automatically calls add child on parent.
		/// </summary>
		/// <param name="parent">New parent</param>
		void SetParent(std::shared_ptr<SceneNode> parent);

		/// <summary>
		///		Adds a child to this scene node. Automatically sets the child parent.
		/// </summary>
		/// <param name="child">New child</param>
		void AddChild(std::shared_ptr<SceneNode> child);

		/// <summary>
		///		Removes a child from this scene node. Automatically removes the child parent.
		/// </summary>
		/// <param name="child">Child to be removed</param>
		void RemoveChild(std::shared_ptr<SceneNode> child);

		/// <summary>
		///		Checks if this scene node has a certain node as child
		/// </summary>
		/// <param name="child">Child to test</param>
		/// <returns>True if has child, otherwise false</returns>
		bool HasChild(std::shared_ptr<SceneNode> child);

		/// <summary>
		///		Attaches a component to this scene node
		/// </summary>
		/// <param name="component">Component to be attached</param>
		void Attach(std::shared_ptr<Component> component);

		/// <summary>
		///		Dettaches a component from this scene node
		/// </summary>
		/// <param name="component">Component to be dettached</param>
		void Dettach(std::shared_ptr<Component> component);

		/// <summary>
		///		Checks if a component is attached to this scene node
		/// </summary>
		/// <param name="component">Component to check</param>
		/// <returns>True if component is attached, otherwise false</returns>
		bool IsAttached(std::shared_ptr<Component> component);

	private:
		mutable std::mutex m_mutex;

		std::set<std::shared_ptr<Component>> m_components;
		std::set<std::shared_ptr<SceneNode>> m_children;
		std::shared_ptr<SceneNode> m_parent;
		// Transformation relative to parent node
		glm::mat4 m_relativeToParent;

		// Inherited via Serializable
		virtual void Serialize(std::ostream & stream) const final;
		virtual void Deserialize(std::istream & stream) final;
	};
}