#include "ParticleSystem.h"
#include <iostream>
#include <time.h>

#include <glm/gtx/string_cast.hpp>

float pShininess = 16;
float pemit[3][4] = { { 1, 0.62, 0.114, 1 }, { 1, 0.843, 0.114, 1 }, { 1, 0.137, 0.114, 1 } };

void ParticleSystem::init()
{
	int i;
	srand(unsigned(time(0)));
	for (i = 0; i < ptlCount; i++) {
		//std::cout << rand() % 50 - 26 << std::endl;
		int cidx = rand() % 3;
		Particle tmp = {
			glm::vec3(0, 0, 0),	//Position
			glm::vec3(((rand()%40)-10.0f), ((rand()%50)), ((rand()%40-10.0f))),	//velocity
			glm::vec3(0, 0, 0),	//Acceleration
			pemit[cidx][0], pemit[cidx][1], pemit[cidx][2],	//Emitted Color
			0.0f,	//age
			1.5+0.05*(rand()%10),	//Life
			0.1f	//Size
		};
		particles.push_back(tmp);
	}
	sphere = gluNewQuadric();

}

//Simulate the state of all particles
void ParticleSystem::simulate(float dt) {

	aging(dt);
	applyGravity();
	kinematics(dt);
}


//a loop to counting the age of the particles
void ParticleSystem::aging(float dt) {

	for (int i = 0; i < particles.size(); i++) {
		particles[i].age += dt;
		//Age out then die
		if (particles[i].age > particles[i].life) {
			particles.erase(particles.begin() + i);
		}
	}
}

//applying gravity system to the scene
void ParticleSystem::applyGravity() {

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].acceleration = glm::vec3(0, gravity, 0);
	}

}


void ParticleSystem::kinematics(float dt) {

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].position += particles[i].veclocity * dt;
		particles[i].veclocity += particles[i].acceleration * dt;
	}

}

//the render function
void ParticleSystem::render() {

	for (int i = 0; i < particles.size(); i++)
	{
		//std::cout << "drawing!" << std::endl;
		Particle& p = particles[i];
		float alpha = 1 - p.age / p.life;	//The older the dimmer
		//std::cout << p.color.r << " " << p.color.g << " " << p.color.b << std::endl;
		float ambient[4] = { 0.05, 0.05, 0.05, alpha };
		float diffuse[4] = { p.color[0], p.color[1], p.color[2], alpha };
		float specular[4] = { 0.7, 0.7, 0.7, alpha };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pShininess);
		//d::cout << alpha << std::endl;
		glPushMatrix();
		glTranslatef(p.position.x, p.position.y, p.position.z);
		gluSphere(sphere, p.size, 32, 16);
		glPopMatrix();
	}

	//Reset Color for world
	glColor4f(1, 1, 1, 1);
}
