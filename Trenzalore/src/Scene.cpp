#include "Scene.h"

void Scene::Add(Object* object)
{
	m_Objects.push_back(object);
}

void Scene::Clear() 
{
	m_Objects.clear();

}