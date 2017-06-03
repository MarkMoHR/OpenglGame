#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <gl\freeglut.h>

struct Particle
{
	glm::vec3 position;
	glm::vec3 veclocity;
	glm::vec3 acceleration;
	float color[3];
	float age;
	float life;
	float size;
};

class ParticleSystem
{
public:
	ParticleSystem() {}
	ParticleSystem(int _count, float _gravity) { ptlCount = _count; gravity = _gravity; }
	void init();
	void simulate(float dt);
	void aging(float dt);
	void applyGravity();
	void kinematics(float dt);
	void render();
	~ParticleSystem() {}

private:
	int ptlCount;
	float gravity;
	GLUquadricObj* sphere;
	std::vector<Particle> particles;
};