#ifndef PongApp_h__
#define PongApp_h__
#pragma once

#include "GameApp.h"
#include "Texture.h"
#include "Font_TTF.h"
#include "Vec2D.h"
#include "GameObject.h"
#include "SoundEffect.h"


//enum DIRN { LESSTHAN, GRTERTHAN};
enum BALL_DIRN { LEFT, RIGHT};

class PongApp : public GameApp
{
public:
	PongApp(std::string appname);
	virtual ~PongApp();

	bool AppInit();

	bool CreateBall(Renderer& renderer);

	bool CreateSounds();

	bool CreatePaddle(GameObject &paddle, Renderer& renderer, int paddle_x, int paddle_y);

	void AppCleanup();

	void AppRender(Renderer& renderer);

	void AppUpdate(double dt);

	bool OnKeyDown(SDL_Scancode scan, SDL_Keycode key);
	bool OnKeyUp(SDL_Scancode scan, SDL_Keycode key);

private:
	void ResetBall(BALL_DIRN dirn);

	bool CheckForBallPaddleCollision(int Norm, GameObject& paddle_obj, GameObject& ball_obj, double circle_radius);

	bool PaddleFaceCollide(GameObject &paddle_obj, GameObject &ball_obj, int Norm, double planePos, double circle_radius);

	bool PaddleCornerCollide(GameObject &ball_obj, GameObject &paddle_obj, int Norm, int dirn, double circle_radius);

	bool BallBounceOffPaddle(int Norm, double dist, GameObject &ball_obj, double relativeYPosition);

	void PaddleClamp(double dt, GameObject& paddle);

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

	double m_Ball_XSpeed = 200.;
	double m_BounceModifier = m_Ball_XSpeed*1.5;

	int m_paddle_min = 50;
	int m_paddle_max = -m_paddle_min; // Redefined in AppInit
	double m_paddle_speed = 200.0;

	int m_LeftPlayerScore = 0;
	int m_RightPlayerScore = 0;
};

#endif // PongApp_h__

