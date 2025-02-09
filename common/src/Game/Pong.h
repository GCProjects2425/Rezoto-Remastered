#pragma once
#include <type_traits>

inline constexpr float GameSizeX = 1600.f;
inline constexpr float GameSizeY = 900.f;
inline constexpr float BallRadius = 5.f;
inline constexpr float PaddleDistFromBorders = 10.f;
inline constexpr float PaddleHeight = 100.f;

enum class GameState
{
	Ongoing = 0,
	LeftWins,
	RightWins
};

enum class PaddlesBehaviour
{
	None = 0,

	LeftUp = 1 << 0,
	LeftDown = 1 << 1,
	RightUp = 1 << 2,
	RightDown = 1 << 3,
};

static PaddlesBehaviour operator|=(PaddlesBehaviour& lhs, PaddlesBehaviour rhs)
{
	using UType = std::underlying_type_t<PaddlesBehaviour>;
	return lhs = static_cast<PaddlesBehaviour>(static_cast<UType>(lhs) | static_cast<UType>(rhs));
}

static PaddlesBehaviour operator&=(PaddlesBehaviour& lhs, PaddlesBehaviour rhs)
{
	using UType = std::underlying_type_t<PaddlesBehaviour>;
	return lhs = static_cast<PaddlesBehaviour>(static_cast<UType>(lhs) & static_cast<UType>(rhs));
}

static PaddlesBehaviour operator~(PaddlesBehaviour rhs)
{
	using UType = std::underlying_type_t<PaddlesBehaviour>;
	return static_cast<PaddlesBehaviour>(~static_cast<UType>(rhs));
}

struct Pong
{
	void Reset();
	void Update(float dt);
	GameState GetGameState() const;
	void OnCollision();
	void OnCollisionMessage();

	float BallX, BallY;
	float BallDx, BallDy;
	float LeftPaddle, RightPaddle;
	PaddlesBehaviour Behaviours;
};
