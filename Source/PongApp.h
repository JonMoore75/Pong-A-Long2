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
//enum PADDLE_MOVE {UP, STOP, DOWN};

class CollisionData
{
public:
	CollisionData() { }
	CollisionData(Vec2D relVelocity, Vec2D norm)
		: m_RelVelocity(relVelocity), m_CollisionNormalA(norm) 
	{
	}

	CollisionData(const CollisionData& rhs)
		: m_RelVelocity(rhs.m_RelVelocity), m_CollisionNormalA(rhs.m_CollisionNormalA)
	{  
	}

	Vec2D		m_RelVelocity;
	Vec2D		m_CollisionNormalA;
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

	void MovePaddle(double dt, GameObject& paddle);

	void CheckForCircleAxisCollision(AXIS axis, DIRN dirn, int planePos, GameObject& circle_obj, double circle_radius);
	void CheckForCircleLineCollision(double& dt, const LineCollider& line, const CircleCollider& circle, const Vec2D lineVelocity);
	void CheckForCirclePointCollision(double& dt, const Vec2D& point, const CircleCollider& circle, const Vec2D pointVelocity);

	void TestForWallCollisions(double& col_dt);
	void TestForPaddleCollision(double& dt, Vec2D& paddle_pos, Vec2D& paddle_vel, double paddle_width, double paddle_height);

	GameObject m_Ball;
	GameObject m_textInstruct;
	GameObject m_TargetDot;
	GameObject m_LeftPaddle;
	GameObject m_RightPaddle;

// 	PADDLE_MOVE m_LeftPaddleMove = STOP;
// 	PADDLE_MOVE m_RightPaddleMove = STOP;

	int m_paddle_min = 50;
	int m_paddle_max = -m_paddle_min;

	double paddle_speed = 200.;

	bool m_bShowDot = false;
	bool m_bContCollisionDetect = true;

	double m_Ball_Speed = 200.;

	std::unique_ptr<CollisionData> m_pNextCollison;
};

#endif // PongApp_h__

