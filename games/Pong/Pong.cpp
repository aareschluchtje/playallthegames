#include "Pong.h"

#include <blib/SpriteBatch.h>
#include <blib/ResourceManager.h>
#include <blib/Util.h>
#include <blib/Math.h>

#include "../../PlayAllTheGames/Settings.h"
#include "../../PlayAllTheGames/Participant.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>

GameBase* getGame()
{
	return new Pong();
}

std::string Pong::getName()
{
	return "Pong";
}

std::string Pong::getInstructions()
{
	return "Reflect the ball";
}

void Pong::loadResources()
{
	backSprite = resourceManager->getResource<blib::Texture>("assets/games/Pong/back.png");
	playerSprite = resourceManager->getResource<blib::Texture>("assets/games/Pong/rectangle.png");
	ballSprite = resourceManager->getResource<blib::Texture>("assets/games/Pong/Ball.png");
	font = resourceManager->getResource<blib::Font>("menu");
}

void Pong::start(Difficulty difficulty)
{
	positive = false;
	speed = 2;
	turningFactor = 0;
	ball.clear();
	rotation = rand();

	maxPlayerScore = 5;

	for (auto p : players)
	{
		p->position = glm::vec2(1920 / 2, 1080 / 2) + 500.0f * blib::util::fromAngle(p->index / (float)players.size() * 2 * (float)M_PI);
		p->rotation = (float)M_PI / (float)players.size()*2 * turningFactor;
		turningFactor++;
	}
	glm::vec2 v(1920/2, 1080/2);
	ball.push_back(v);
}

void Pong::update(float elapsedTime)
{
	blib::math::Rectangle screenRect(0, 0, 1920, 1080);
	for (auto p : players)
	{
		if (!screenRect.contains(ball[0]))
		{
			p->score += 1;
			ball[0] = glm::vec2(1920 / 2, 1080 / 2);
			speed = 2;
			rotation = rand();
		}
		if (p->joystick.leftStick.y < 0)
		{
			glm::vec2 oldPosition = p->position;
			p->position += 5.0f * blib::util::fromAngle(300) * 60.0f*elapsedTime;
			bool collision = false;
			for (auto pp : players)
			{
				if (p == pp)
					continue;
				if (glm::length(pp->position - p->position) < 100)
				{
					collision = true;
					pp->rotation += (float)M_PI;
				}
			}
			if (!screenRect.contains(p->position))
				collision = true;

			if (collision)
			{
				p->position = oldPosition;
				p->rotation += (float)M_PI;
			}
		}
		if (p->joystick.leftStick.y > 0)
		{
			glm::vec2 oldPosition = p->position;
			p->position += 5.0f * blib::util::fromAngle(900) * 60.0f*elapsedTime;
			bool collision = false;
			for (auto pp : players)
			{
				if (p == pp)
					continue;
				if (glm::length(pp->position - p->position) < 100)
				{
					collision = true;
					pp->rotation += (float)M_PI;
				}
			}
			if (!screenRect.contains(p->position))
				collision = true;

			if (collision)
			{
				p->position = oldPosition;
				p->rotation += (float)M_PI;
			}
		}
		
		glm::vec2 suckPos = p->position + 50.0f*blib::util::fromAngle(p->rotation);
		
		if (blib::linq::any(ball, [p](glm::vec2 t) { return glm::length(t - p->position) < 64; }))
		{
			speed += 0.5;
			rotation -= 1;
		}
		if (glm::length(ball[0] - p->position) < 50)
		{
			ball[0].x -= 20 * elapsedTime;
		}
		else
		{
			ball[0] += speed * 20.0f * blib::util::fromAngle(rotation) * elapsedTime;
		}
	}
}


void Pong::draw()
{
	spriteBatch->begin(settings->scaleMatrix);
	spriteBatch->draw(backSprite, glm::mat4());

	for (auto t : ball) { spriteBatch->draw(ballSprite, blib::math::easyMatrix(t), ballSprite->center); };
	for (auto p : players) { spriteBatch->draw(playerSprite, glm::rotate(glm::translate(glm::mat4(), glm::vec3(p->position, 0)), glm::degrees(p->rotation), glm::vec3(0, 0, 1)), playerSprite->center, blib::math::Rectangle(0, 0, 1, 1), p->participant->color); }
	for (size_t i = 0; i < players.size(); i++)
		spriteBatch->draw(font, blib::util::toString(players[i]->score), blib::math::easyMatrix(glm::vec2(10, 64 * i), 0, 1), players[i]->participant->color);

	spriteBatch->end();
}

blib::Texture* Pong::getTitleImage()
{
	return NULL;
}
