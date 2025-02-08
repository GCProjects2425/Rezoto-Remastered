#pragma once
#include <SFML/Graphics.hpp>
class Scene
{
public:
	virtual ~Scene() = default; 
	virtual void Draw(sf::RenderWindow& window) = 0;
	virtual void Update(float dt) = 0;
	virtual void Start() = 0;
	virtual void HandleInput(sf::RenderWindow& window) = 0;

};

