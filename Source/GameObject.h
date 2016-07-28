#ifndef GameObject_h__
#define GameObject_h__
#pragma once

#include "Texture.h"
#include "Vec2D.h"

#include <memory>
#include <utility>

class Renderer;

class Collider
{
public: 
	Collider(Vec2D pos = Vec2D()) : m_Position( pos ) {}
	virtual ~Collider() {}

	void Update(double dt) { m_Position += m_Velocity*dt; }

	Vec2D m_Position;
	Vec2D m_Velocity;
};

class LineCollider : public Collider
{
public:
	LineCollider(Vec2D pos, Vec2D line, Vec2D norm) : Collider(pos), m_Line(line), m_Normal(norm) {}

	Vec2D m_Line;
	Vec2D m_Normal;
};

class CircleCollider : public Collider
{
public:
	CircleCollider(Vec2D pos, double radius) :Collider(pos), m_Radius(radius) {}

	double m_Radius;
};

class GameObject
{
public:
	enum AnchorPt { TOPLEFT, TOPCENTRE, TOPRIGHT, LEFT, CENTRE, RIGHT, LOWLEFT, LOWCENTRE, LOWRIGHT, CUSTOM };

	GameObject();
	virtual ~GameObject();

	void SetCollider(std::unique_ptr<Collider> col) { m_pCollider = std::move(col); }
	Collider* GetCollider() { return m_pCollider.get(); }

	bool CreateTexture(Renderer& renderer, std::string filename);
	bool CreateTextureFromText(Renderer& renderer, std::string text, FontTTF& font);
	void Render(Renderer& renderer) const;

	const Vec2D& GetVel() const { return m_pCollider->m_Velocity; }
	const Vec2D& GetPos() const { return m_pCollider->m_Position; }

	Vec2D& GetVel() { return const_cast<Vec2D&>(static_cast<const GameObject*>(this)->GetVel()); }
	Vec2D& GetPos() { return const_cast<Vec2D&>(static_cast<const GameObject*>(this)->GetPos()); }

	int GetWidth() const { return m_Texture.GetWidth(); }
	int GetHeight() const { return m_Texture.GetHeight(); }

	void SetVelocity(const Vec2D& velocity) { m_pCollider->m_Velocity = velocity; }
	void SetPosition(const Vec2D& position) { m_pCollider->m_Position = position; }

	void SetAnchorPt(AnchorPt anchor);
	void SetAnchorPt(Vec2D anchorpt) { m_AnchorPt = anchorpt; }

	void Update(double dt);

protected:
	Vec2D m_AnchorPt = { 0., 0. };

	std::unique_ptr<Collider> m_pCollider;

	Texture m_Texture;
};

#endif GameObject_h__