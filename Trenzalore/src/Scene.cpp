#include "Scene.h"

void Scene::Add(Sphere object)
{
	m_Objects.emplace_back(object);
}

void Scene::Clear() 
{
	m_Objects.clear();

}