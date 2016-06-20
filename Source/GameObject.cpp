#include "GameObject.h"

#include "Renderer.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	m_Texture.Release();
}

bool GameObject::CreateTexture(Renderer& renderer, std::string filename)
{
	return m_Texture.CreateFromFile(renderer, filename);
}

void GameObject::Render(Renderer& renderer) const
{
	m_Texture.Render(renderer, m_Position.intX(), m_Position.intY());
}

void GameObject::Update(double dt)
{
	m_Position += m_Velocity*dt;
}

