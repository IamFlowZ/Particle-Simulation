#include "stdafx.h"
#include "Box.h"


Box::Box()
{
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 50;
	m_rect.h = 50;
}

void Box::init()
{
	m_speed = 2;
	m_direction = -5;
}

void Box::update(int interval)
{
	m_x += m_speed;
	m_y += m_speed;

	if ((m_x < -1 || m_x > 1 || m_y < -1 || m_y > 1))
	{
		init();
	}
}

void Box::drawBox(SDL_Renderer * renderer, SDL_Rect * rect)
{
	SDL_RenderDrawRect(renderer, rect);
}

Box::~Box()
{
}
