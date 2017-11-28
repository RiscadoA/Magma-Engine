#pragma once

#include "..\..\Utils\Serializable.hpp"
#include "..\MessageBus.hpp"
#include "SceneNode.hpp"

namespace Magma
{
	/// <summary>
	///		Contains and manages the scene graph
	/// </summary>
	class Scene : public MessageListener, public Serializable
	{
	public:
		Scene();
		~Scene();

	private:
		SceneNode* m_root;

		// Inherited via Serializable
		virtual void Serialize(std::ostream & stream) const override;
		virtual void Deserialize(std::istream & stream) override;
	};
}