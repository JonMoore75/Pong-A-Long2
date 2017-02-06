#ifndef PongApp_h__
#define PongApp_h__
#pragma once

#include "GameApp.h"
#include "Texture.h"
#include "Font_TTF.h"
#include "Vec2D.h"
#include "GameObject.h"
#include "SoundEffect.h"

enum AXIS { XAXIS, YAXIS };
enum DIRN { LESSTHAN, GRTERTHAN};
enum BALL_DIRN { LEFT, RIGHT};

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
	void ResetBall(BALL_DIRN dirn);
	bool CheckForCircleAxisCollision(AXIS axis, DIRN dirn, int planePos, GameObject& circle_obj, double circle_radius);
	bool CheckForCircleAxisTrigger(AXIS axis, DIRN dirn, int planePos, GameObject& circle_obj, double circle_radius);

	bool CheckForBallPaddleCollision(DIRN dirn, GameObject& paddle_obj, GameObject& ball_obj, double circle_radius);
	void MovePaddle(double dt, GameObject& paddle);

	void CheckForPointWon();

	void UpdateScores();

	void TestForWallCollisions();

	GameObject m_Ball;
	GameObject m_LeftPaddle;
	GameObject m_RightPaddle;

	Texture m_LeftScoreText;
	Texture m_RightScoreText;

	SoundEffect m_BounceSound;
	SoundEffect m_ScoreSound;

	double m_Ball_Speed = 200.;
	double m_BounceModifier = m_Ball_Speed*1.5;

	int m_paddle_min = 50;
	int m_paddle_max = -m_paddle_min;
	double m_paddle_speed = 200.0;

	int m_LeftPlayerScore = 0;
	int m_RightPlayerScore = 0;
};

#endif // PongApp_h__

