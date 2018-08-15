#pragma once
#include <stdlib.h>
#include <time.h>
#include "Particle.h"

class Swarm
{
public:
	const static int N_PARTICLES = 1000;
private:
	Particle * m_particles;
	int lastTime;
public:
	Swarm();
	void update(int elapsed);
	const Particle * const getParticles() { return m_particles; };
	~Swarm();
};

