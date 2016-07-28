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
	void CheckForCircleLineCollision(double& dt, const LineCollider& line, const CircleCollider& circle);
	void CheckForCirclePointCollision(double& dt, const Vec2D& point, const CircleCollider& circle);

	void TestForWallCollisions(double& col_dt);
	void TestForPaddleCollision(double& dt, Vec2D& paddle_pos, double paddle_width, double paddle_height);

	GameObject m_Ball;
	GameObject m_textInstruct;
	GameObject m_TargetDot;
	GameObject m_LeftPaddle;
	GameObject m_RightPaddle;

	bool m_bShowDot = false;
	bool m_bContCollisionDetect = true;

	double m_Ball_Speed = 200.;

	std::unique_ptr<Vec2D> m_pNextCollisonNormal;
};

#endif // PongApp_h__

