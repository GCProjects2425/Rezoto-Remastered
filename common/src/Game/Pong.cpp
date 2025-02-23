#include "Pong.h"
#include <numbers>
#include <random>
#include "Enums.h"
#include <nlohmann/json.hpp>

inline constexpr float BallStartMaxAngle = std::numbers::pi_v<float> / 5.f;
inline constexpr float BallBounceMaxAngle = std::numbers::pi_v<float> / 4.f;
inline constexpr float BallStartSpeed = 800.f;
inline constexpr float PaddleSpeed = 400.f;

void Pong::Reset()
{
	BallX = GameSizeX / 2.f;
	BallY = GameSizeY / 2.f;
	LeftPaddle = RightPaddle = (GameSizeY - PaddleHeight) / 2.f;

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution dist(-BallStartMaxAngle, BallStartMaxAngle * 3.f);

	float angle = dist(rng);
	if (angle > BallStartMaxAngle)
	{
		angle = angle - BallStartMaxAngle * 2.f + std::numbers::pi_v<float>;
	}

	BallDx = BallStartSpeed * std::cosf(angle);
	BallDy = BallStartSpeed * std::sinf(angle);
}

static bool operator&(PaddlesBehaviour lhs, PaddlesBehaviour rhs)
{
	using UType = std::underlying_type_t<PaddlesBehaviour>;
	return (static_cast<UType>(lhs) & static_cast<UType>(rhs)) != 0;
}

void Pong::Update(float dt)
{
	// paddles update
	using enum PaddlesBehaviour;
	if (Behaviours & LeftUp)
	{
		LeftPaddle = std::clamp(LeftPaddle - dt * PaddleSpeed, 0.f, GameSizeY - PaddleHeight);
	}
	else if (Behaviours & LeftDown)
	{
		LeftPaddle = std::clamp(LeftPaddle + dt * PaddleSpeed, 0.f, GameSizeY - PaddleHeight);
	}
	if (Behaviours & RightUp)
	{
		RightPaddle = std::clamp(RightPaddle - dt * PaddleSpeed, 0.f, GameSizeY - PaddleHeight);
	}
	else if (Behaviours & RightDown)
	{
		RightPaddle = std::clamp(RightPaddle + dt * PaddleSpeed, 0.f, GameSizeY - PaddleHeight);
	}

	// ball update
	BallX += BallDx * dt;
	BallY += BallDy * dt;

	// bonce top
	if (BallRadius >= BallY)
	{
		BallY += 2.f * (BallRadius - BallY);
		BallDy = -BallDy;
	}

	// bounce bottom
	else if (BallY + BallRadius >= GameSizeY)
	{
		BallY -= 2.f * (BallRadius + BallY - GameSizeY);
		BallDy = -BallDy;
	}

	float distX = std::numeric_limits<float>::max();
	float distY = std::numeric_limits<float>::max();
	float paddleCenter = std::numeric_limits<float>::signaling_NaN();

	// left paddle collision
	if (PaddleDistFromBorders >= BallX - BallRadius)
	{
		float closestY = std::clamp(BallY, LeftPaddle, LeftPaddle + PaddleHeight);
		distX = BallX - PaddleDistFromBorders;
		distY = BallY - closestY;
		paddleCenter = LeftPaddle + PaddleHeight / 2.f;
	}

	// right paddle collision
	else if (BallX + BallRadius >= GameSizeX - PaddleDistFromBorders)
	{
		float closestY = std::clamp(BallY, RightPaddle, RightPaddle + PaddleHeight);
		distX = BallX - GameSizeX + PaddleDistFromBorders;
		distY = BallY - closestY;
		paddleCenter = RightPaddle + PaddleHeight / 2.f;
	}

	// paddle bounce
	if ((distX * distX + distY * distY) < BallRadius * BallRadius)
	{
		float length = std::hypot(distX, distY);

		if (length > 0.0f)
		{
			// collision normal
			float normX = distX / length;
			float normY = distY / length;

			float dotProduct = BallDx * normX + BallDy * normY;
			BallDx -= 2.f * dotProduct * normX;
			BallDy -= 2.f * dotProduct * normY;

			// normalized from -1 to 1
			float impactFactor = (BallY - paddleCenter) / (PaddleHeight / 2.f);
			float newAngle = impactFactor * BallBounceMaxAngle;
			float speed = std::hypot(BallDx, BallDy);

			float paddleSide = std::copysign(1.f, BallDx);
			BallDx = paddleSide * std::cosf(newAngle) * speed;
			BallDy = std::sinf(newAngle) * speed;

			float penetrationDepth = BallRadius - length;
			BallX += normX * penetrationDepth;
			BallY += normY * penetrationDepth;

			//Send ballBounceMessage (BallDx, BallDy)
			//nlohmann::json msg = {
			//				{"type", MessageType::MessageType_BallBounce},
			//				{"data", {
			//					{"ballDx", BallDx },
			//					{"ballDy", BallDy },

			//					{"ballx", BallX },
			//					{"bally", BallY }
			//				}}
			//};
			//UdpS
			//UDPClient::GetInstance()->SendMsg(msg.dump());
		}
	}
}

GameState Pong::GetGameState() const
{
	if (BallRadius > BallX)
		return GameState::RightWins;

	if (BallX + BallRadius > GameSizeX)
		return GameState::LeftWins;

	return GameState::Ongoing;
}

void Pong::OnCollision()
{
}
