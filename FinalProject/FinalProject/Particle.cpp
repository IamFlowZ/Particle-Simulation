#include "stdafx.h"
#include "Particle.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Particle::Particle(): m_x(0), m_y(0)
{
	//CHECK THE MOTHER FUCKING DATA TYPES
	init();
}

void Particle::init()
{
	m_direction = (2 * M_PI * rand()) / RAND_MAX;
	m_speed = (0.02 * rand()) / RAND_MAX;

	m_speed *= m_speed;
}

void Particle::update(int interval)
{
	m_direction += interval * 0.003;

	double xspeed = m_speed * cos(m_direction);
	double yspeed = m_speed * sin(m_direction);

	m_x += xspeed * interval;
	m_y += yspeed * interval;

	if ((m_x < -1 || m_x > 1 || m_y < -1 || m_y > 1) || rand() < RAND_MAX / 100)
	{
		init();
	}

}

void Particle::mouseEvent(SDL_MouseMotionEvent mouseEvent)
{
	while (mouseEvent.type == SDL_MOUSEBUTTONDOWN)
	{
		/*m_direction = mouseEvent.;*/
		m_x = mouseEvent.xrel;
		m_x = mouseEvent.yrel;
		//m_direction = ;
	}
}

Particle::~Particle()
{
}
