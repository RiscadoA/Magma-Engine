#include "Scene.hpp"

Magma::Scene::Scene()
{
	m_root = std::make_shared<SceneNode>();
}

Magma::Scene::~Scene()
{
	m_root = nullptr;
}

void Magma::Scene::Serialize(std::ostream & stream) const
{
	stream << *m_root;
}

void Magma::Scene::Deserialize(std::istream & stream)
{
	stream >> *m_root;
}

void Magma::Scene::DerivedInit()
{

}

void Magma::Scene::DerivedTerminate()
{

}
