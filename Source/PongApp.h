#ifndef PongApp_h__
#define PongApp_h__
#pragma once

#include "GameApp.h"
#include "Texture.h"
#include "Font_TTF.h"
#include "Vec2D.h"
#include "GameObject.h"

enum AXIS { XAXIS, YAXIS };
enum DIRN { LESSTHAN, GRTERTHAN };

class LineCollider
{
public:
	LineCollider(Vec2D pos, Vec2D line, Vec2D norm) : m_position(pos), m_Line(line), m_Normal(norm) {}
	Vec2D m_position;
	Vec2D m_Line;
	Vec2D m_Normal;
};

class PongApp : public GameApp
{
public:
	PongApp(std::string appname);
	virtual ~PongApp();

	bool AppInit();
	void AppCleanup();

	void AppRender(Renderer& renderer);

	void AppUpdate(double dt);

	bool OnKeyDown(SDL_Scancode scan, SDL_Keycode key);
	bool OnKeyUp(SDL_Scancode scan, SDL_Keycode key);

private:
	void ResetBall();

	void CheckForCircleAxisCollision(AXIS axis, DIRN dirn, int planePos, GameObject& circle_obj, double circle_radius);
	void CheckForCircleLineCollision(double& dt, LineCollider& line, GameObject& circle_obj, double circle_radius);

	GameObject m_Ball;
	GameObject m_textInstruct;
	GameObject m_TargetDot;

	bool m_bShowDot = false;

	double m_Ball_Speed = 200.;
};

#endif // PongApp_h__

