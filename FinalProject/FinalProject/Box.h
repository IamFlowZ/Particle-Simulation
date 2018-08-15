#pragma once
#include "stdafx.h"
#include <SDL.h>

using namespace std;

class Box
{
private:
	SDL_Rect m_rect;
	double m_x;
	double m_y;
	double m_speed;
	double m_direction;
public:
	Box();
	void init();
	void update(int interval);
	void drawBox(SDL_Renderer * renderer, SDL_Rect * rect);
	~Box();
};

