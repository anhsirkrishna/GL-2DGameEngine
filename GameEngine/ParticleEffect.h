/******************************************************************************/
/*!
/*File   ParticleEffect.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/13/2022
/*  ParticleEffect header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include <glm.hpp>

#include "Component.h"

typedef unsigned int GLuint;

class Texture;
class Transform;
class ShaderProgram;

struct Particle
{
	glm::vec4 position, velocity;
	long long int life_time;

	Particle(glm::vec4 position_, glm::vec4 velocity_, int life_time_) :
		position(position_), velocity(velocity_), life_time(life_time_) {}
};

class ParticleEffect : public Component {
private:
	Texture* p_texture;
	std::vector<Particle> particles;
	std::vector<float> particle_vertex_list;
	std::vector<float> particle_color_list;
	std::vector<float> single_particle_vertices;
	GLuint vao_id;
	GLuint vertex_buffer_id;
	GLuint color_buffer_id;
	Transform* p_owner_transform;
	unsigned int max_particle_count;
	unsigned int texture_mode;
	int particle_lifetime;
	glm::vec2 origin_offset;
	glm::vec4 maximum_velocity, minimum_velocity;

	//To keep track of the last used particle
	int lastUsedParticle;
	
	//Returns a pointer to the texture used for the particles.
	Texture* GetTexture();

	//Sets a texture as the currently associated texture.
	void SetTexture(Texture* _p_texture);

	//Get the index of a dead particle
	int GetLastUsedParticle();

	//Reset the particle at the given particle_index
	void RespawnParticle(int particle_index);

	//Get a random particle velocity
	glm::vec4 GetRandomParticleVelocity();
public:

	//Default ctor to create a Particle Effect
	ParticleEffect();

	/*Update each particle in the particle effect.
	* Adjust the lifetime.
	* Move the particle according to the velocity.
	* If a particle has reached it's lifetime,
	* spawn a new one in its place
	* Returns : void
	*/
	void Update();

	/*Function to draw all the particles that are part of the particle effect*/
	void Draw(ShaderProgram* program);

	//Function to Link with other dependant components like the Transform component
	virtual void Link();

	//Sets the render mode to color (int=0) or texture (int=1)
	void SetTextureMode(int mode_);

	/*Serialize the ParticleEffect object from a json input
	* Expects a json dict with the following keys:
	* texture_name : The filename of the texture present in the resources folder
	* vertex_list: A 1D list of 3*4 floats. Usually 4 Vertices.
				   Each group of 3 floats represents 1 vertexs x, y,and z values.
	* color_list: A 1D list of 4*4 floats. Each group of 4 floats
				  represents 1 vertexs r, g, b, and a color values.
	* texture_list: A 1D list of 2*4 floats. Each group of 2 floats represents
					the texture coordinates for each vertex.
	* Returns : void
	*/
	void Serialize(json json_object);

	/*Handle an event sent to this component
	* Returns: void
	*/
	virtual void HandleEvent(TimedEvent* p_event);

	/*Respawn a set of particles with
	* faster velocities and larger lifetime
	* to produce a "Burst" effect
	* Returns: void
	*/
	void BurstEffect(int burst_size);

	/*Resets all the particles to their starting position
	* Returns: void
	*/
	void ResetParticles();

	/*Changes the state of the particle effect
	* Expects a json dict with the following keys:
	* texture_name : The filename of the texture present in the resources folder
	* vertex_list: A 1D list of 3*4 floats. Usually 4 Vertices.
				   Each group of 3 floats represents 1 vertexs x, y,and z values.
	* color_list: A 1D list of 4*4 floats. Each group of 4 floats
				  represents 1 vertexs r, g, b, and a color values.
	* texture_list: A 1D list of 2*4 floats. Each group of 2 floats represents
					the texture coordinates for each vertex.
	* Returns : void
	*/
	virtual void ChangeState(json json_object);
};