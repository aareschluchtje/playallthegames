#pragma once

#include "../../AlivePlayer.h"

class SkipRopePlayer : public AlivePlayer
{
public:
	SkipRopePlayer(int index);

	glm::vec2 position;
	float speed;
};