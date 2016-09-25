#ifndef PongApp_h__
#define PongApp_h__
#pragma once

#include "GameApp.h"
#include "Texture.h"
#include "Font_TTF.h"
#include "Vec2D.h"
#include "GameObject.h"

enum AXIS { XAXIS, YAXIS };
enum DIRN { LESSTHAN, GRTERTHAN};

class PongApp : public GameApp
{
public:
	PongApp(std::string appname);
	virtual ~PongApp();

	bool AppInit();
	void AppCleanup();

	void AppRender(Renderer& renderer);

	void AppUpdate(double dt);

	void TestForWallCollisions();

	bool OnKeyDown(SDL_Scancode scan, SDL_Keycode key);
	bool OnKeyUp(SDL_Scancode scan, SDL_Keycode key);

private:
	void ResetBall();
	void CheckForCircleAxisCollision(AXIS axis, DIRN dirn, int planePos, GameObject& circle_obj, double circle_radius);
	void CheckForBallPaddleCollision(DIRN dirn, GameObject& paddle_obj, GameObject& ball_obj, double circle_radius);

	GameObject m_Ball;
	GameObject m_LeftPaddle;
	GameObject m_RightPaddle;
	GameObject m_textInstruct;

	double m_Ball_Speed = 200.;
	double m_BounceModifier = m_Ball_Speed*1.5;
};

#endif // PongApp_h__

