#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL.h>

using namespace std;

struct Particle
{
	double m_x;
	double m_y;
private:
	double m_speed;
	double m_direction;
private:
	void init();
public:
	Particle();
	void update(int interval);
	void mouseEvent(SDL_MouseMotionEvent mouseEvent);
	//void callBack(int userdata, Uint8* stream, int len);
	~Particle();
};

