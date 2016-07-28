#include "PongApp.h"

#include <random> 
#include <algorithm>

PongApp::PongApp(std::string appname) : GameApp(appname)
{

}

PongApp::~PongApp()
{

}

bool PongApp::AppInit()
{
	Renderer& renderer = m_Window.GetRenderer();

	// Ball Creation
	if (!m_Ball.CreateTexture(renderer, "..\\gfx\\ball.png"))
		return false;

	m_Ball.SetCollider( std::unique_ptr<Collider>(  new CircleCollider( Vec2D(), m_Ball.GetHeight()/2 ) ) );
	m_Ball.SetAnchorPt(GameObject::CENTRE);
	ResetBall();

	// Dot creation 
	if (!m_TargetDot.CreateTexture(renderer, "..\\gfx\\dot.png"))
		return false;
	m_TargetDot.SetAnchorPt(GameObject::CENTRE);

	int paddle_y = m_Window.GetHeight() / 2;
	int paddle_x = 20;

	// Left paddle creation 
	if (!m_LeftPaddle.CreateTexture(renderer, "..\\gfx\\paddle.png"))
		return false;
	m_LeftPaddle.SetAnchorPt(GameObject::CENTRE);
	m_LeftPaddle.SetPosition(Vec2D(paddle_x, paddle_y));

	// Right paddle creation
	if (!m_RightPaddle.CreateTexture(renderer, "..\\gfx\\paddle.png"))
		return false;
	m_RightPaddle.SetAnchorPt(GameObject::CENTRE);
	m_RightPaddle.SetPosition(Vec2D(m_Window.GetWidth() - paddle_x, paddle_y));

	FontTTF arialFont;
	if (!arialFont.LoadFont("C:\\Windows\\Fonts\\ARIAL.TTF", 24, SDL_Color{ 0xFF, 0xFF, 0xFF, 0xFF }))
		return false;

	if (!m_textInstruct.CreateTextureFromText(renderer, "Press space to reset ball with random direction", arialFont))
		return false;

	return true;
}

void PongApp::AppCleanup()
{
}


void PongApp::AppRender(Renderer& renderer)
{
	m_textInstruct.Render(renderer);
	m_Ball.Render(renderer);

	m_LeftPaddle.Render(renderer);
	m_RightPaddle.Render(renderer);

	if (m_bShowDot)
		m_TargetDot.Render(renderer);
}

void PongApp::AppUpdate(double dt)
{
	m_bShowDot = false;

	if (m_bContCollisionDetect)
	{
		double col_dt = 999.;
		m_pNextCollisonNormal.release();

		TestForWallCollisions(col_dt);
		TestForPaddleCollision(col_dt, m_LeftPaddle.GetPos(), m_LeftPaddle.GetWidth(), m_LeftPaddle.GetHeight());
		TestForPaddleCollision(col_dt, m_RightPaddle.GetPos(), m_RightPaddle.GetWidth(), m_RightPaddle.GetHeight());

		if (m_pNextCollisonNormal && col_dt < dt)
		{
			m_Ball.Update(col_dt);
			m_Ball.GetVel().Reflect(*m_pNextCollisonNormal);
			m_Ball.Update(dt - col_dt);
		}
		else
			m_Ball.Update(dt);
	}
	else
	{
		CheckForCircleAxisCollision(YAXIS, LESSTHAN, 0, m_Ball, m_Ball.GetHeight() / 2);
		CheckForCircleAxisCollision(YAXIS, GRTERTHAN, m_Window.GetHeight(), m_Ball, m_Ball.GetHeight() / 2);

		CheckForCircleAxisCollision(XAXIS, LESSTHAN, 0, m_Ball, m_Ball.GetWidth() / 2);
		CheckForCircleAxisCollision(XAXIS, GRTERTHAN, m_Window.GetWidth(), m_Ball, m_Ball.GetWidth() / 2);
	}
}

void PongApp::TestForWallCollisions(double& col_dt)
{
	int w = m_Window.GetWidth();
	int h = m_Window.GetHeight();

	CircleCollider* pCircle = dynamic_cast<CircleCollider*>(m_Ball.GetCollider());

	if (pCircle)
	{
		CheckForCircleLineCollision(col_dt, LineCollider(Vec2D(0, 0), Vec2D(w, 0), Vec2D(0, 1)), *pCircle);
		CheckForCircleLineCollision(col_dt, LineCollider(Vec2D(w, 0), Vec2D(0, h), Vec2D(-1, 0)), *pCircle);
		CheckForCircleLineCollision(col_dt, LineCollider(Vec2D(w, h), Vec2D(-w, 0), Vec2D(0, -1)), *pCircle);
		CheckForCircleLineCollision(col_dt, LineCollider(Vec2D(0, h), Vec2D(0, -h), Vec2D(1, 0)), *pCircle);
	}
}

void PongApp::TestForPaddleCollision(double& col_dt, Vec2D& paddle_pos, double paddle_width, double paddle_height)
{
	const double& w = paddle_width;
	const double& h = paddle_height;

	CircleCollider* pCircle = dynamic_cast<CircleCollider*>(m_Ball.GetCollider());

	if (pCircle)
	{
		CheckForCircleLineCollision( col_dt, LineCollider(Vec2D(-w/2, -h/2) + paddle_pos, Vec2D(w, 0),	Vec2D(0, -1)), *pCircle );
		CheckForCircleLineCollision( col_dt, LineCollider(Vec2D(w/2, -h/2) + paddle_pos,  Vec2D(0, h),  Vec2D(1, 0)),  *pCircle );
		CheckForCircleLineCollision( col_dt, LineCollider(Vec2D(w/2, h/2) + paddle_pos,	  Vec2D(-w, 0),	Vec2D(0, 1)),  *pCircle );
		CheckForCircleLineCollision( col_dt, LineCollider(Vec2D(-w/2, h/2) + paddle_pos,  Vec2D(0, -h),	Vec2D(-1, 0)), *pCircle );

		CheckForCirclePointCollision( col_dt, Vec2D(-w / 2, -h / 2) + paddle_pos, *pCircle );
		CheckForCirclePointCollision( col_dt, Vec2D(w / 2, -h / 2) + paddle_pos, *pCircle );
		CheckForCirclePointCollision( col_dt, Vec2D(w / 2, h / 2) + paddle_pos, *pCircle );
		CheckForCirclePointCollision( col_dt, Vec2D(-w / 2, h / 2) + paddle_pos, *pCircle );
	}
}


bool PongApp::OnKeyDown(SDL_Scancode scan, SDL_Keycode key)
{
	return true;
}

bool PongApp::OnKeyUp(SDL_Scancode scan, SDL_Keycode key)
{
 	switch (key)
 	{
	case SDLK_SPACE:
		ResetBall();
 		break;
 	case SDLK_ESCAPE:
 		m_Running = false;
 		break;
 	}

	return true;
}

void PongApp::ResetBall()
{
	static const double angl_conv = 2. * 3.14159265 / 360;
	static std::default_random_engine generator;
	static std::uniform_int_distribution<int> distribution(0, 360);

	double angle = distribution(generator);

 	double x = m_Ball_Speed*cos(angle*angl_conv);
 	double y = m_Ball_Speed*sin(angle*angl_conv);
 
 	m_Ball.SetVelocity( Vec2D(x, y) );
 	m_Ball.SetPosition( Vec2D( m_Window.GetWidth() / 2 ,
 						m_Window.GetHeight() / 2  ) );
}

void PongApp::CheckForCircleAxisCollision(AXIS axis, DIRN dirn, int planePos, GameObject& circle_obj, double circle_radius)
{
	double& position = (axis == XAXIS) ? circle_obj.GetPos().x : circle_obj.GetPos().y;
	double& velocity = (axis == XAXIS) ? circle_obj.GetVel().x : circle_obj.GetVel().y;

	if (velocity > 0.0 && dirn == GRTERTHAN) // Going down/right
	{
		double edge = position + circle_radius;

		if (edge > planePos)
		{
			position = position - 2 * (edge - planePos);
			velocity = -velocity;
		}
	}
	else if (velocity < 0.0 && dirn == LESSTHAN)// Going up/left
	{
		double edge = position - circle_radius;

		if (edge < planePos)
		{
			position = position + 2 * (planePos - edge);
			velocity = -velocity;
		}
	}
}

void PongApp::CheckForCircleLineCollision(double& dt, const LineCollider& line, const CircleCollider& circle)
{
	Vec2D B = circle.m_Position- (line.m_Position + line.m_Normal*circle.m_Radius);

	// If moving towards outside of line /edge
	if (circle.m_Velocity.dot(line.m_Normal) < 0.0)
	{
		Vec2D ratio = SolveSimultaneous(line.m_Line.x, -circle.m_Velocity.x*dt, line.m_Line.y, -circle.m_Velocity.y*dt, B);

		if (ratio.x > 0.0 && ratio.x <= 1.0 && ratio.y > 0.0 )
		{
			Vec2D contact_pt = line.m_Position + ratio.x*line.m_Line;
			Vec2D collision_pt = contact_pt + line.m_Normal*circle.m_Radius;
			double time2collision = ratio.y*dt;
			double dist2collision = time2collision*m_Ball_Speed;

			if ( time2collision < dt )
			{
				m_TargetDot.SetPosition(contact_pt);
				m_bShowDot = true;

				dt = time2collision;
				m_pNextCollisonNormal.reset(new Vec2D(line.m_Normal));
			}
		}
	}
}

void PongApp::CheckForCirclePointCollision(double& dt, const Vec2D& point, const CircleCollider& circle)
{
	// We have our ball trying to collide with a corner eg a point.
	// We switch it around so the point becomes a circle of radius equal to the ball and then we can 
	// treat the ball as a point moving along a line.  If the ball hits the corner its line will intersect
	// the circle

	// Eqn of line of trajectory of circle in y = mx +c form
	double m = circle.m_Velocity.y / circle.m_Velocity.x;
	double c = circle.m_Position.y - m*circle.m_Position.x;

	// If C is vector to the corner, P to the balls position and v the ball velocity
	// r the balls radius and t the time to collision. Then collision occurs when ball is a ball
	// radius distance from the corner.
	// | P - C + vt |^2 - r^2 = 0, when solved for t gives us the time to collision
	// Expand to |v|^2 t - 2(P-C).v t + |P-C|^2 - r^2 = 0
	// Convert to form A t^2 + B t + C = 0 to solve t with quadratic formula
	const double& r = circle.m_Radius;

	const Vec2D& v = circle.m_Velocity;
	const Vec2D& C2P = circle.m_Position - point;

	double A = v.dot(v);
	double B = 2*v.dot(C2P);
	double C = C2P.dot(C2P) - r*r;

	double discrim = B*B - 4*A*C;

	double time2collision = -1.0;

	if (discrim > 0.0) // Two points of intersection
	{
		double sqrt_discrim = sqrt(discrim);
		double k1 = (-B + sqrt_discrim) / (2 * A);
		double k2 = (-B - sqrt_discrim) / (2 * A);

		// Find nearest contact point
		if (k1 > 0.0)
		{
			if (k2 > 0.0)
				time2collision = std::min(k1, k2);
			else
				time2collision = k1;
		}
		else if (k2 > 0.0)
			time2collision = k2;
	}
	else if (discrim == 0.0) // One point of intersection
	{
		double k1 = -B / (2 * A);
		if (k1 > 0.0)
			time2collision = k1;
	}
	else // Else line misses circle ie discrim < 0
		return;
	
	Vec2D collision_pt = circle.m_Position + time2collision*v;

	// Can we collide this frame
	if (time2collision >= 0.0 && time2collision < dt)
	{
		m_TargetDot.SetPosition(point);
		m_bShowDot = true;

		dt = time2collision;
		Vec2D collision_norm = (collision_pt - point).Normalize();
		m_pNextCollisonNormal.reset(new Vec2D(collision_norm));
	}
}

