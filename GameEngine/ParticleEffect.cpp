/******************************************************************************/
/*!
/*File   ParticleEffect.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/13/2022
/*  ParticleEffect implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "ParticleEffect.h"
#include "ResourceManager.h"
#include "Util.h"
#include "Texture.h"
#include "FrameRateController.h"
#include "ShaderProgram.h"
#include "GameManager.h"
#include "Transform.h"
#include "GameObject.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <gtc/random.hpp>

unsigned int particles_spawned_per_frame = 2;


#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line GLQuad.cpp:%d): %s\n", __LINE__, glewGetErrorString(err)); exit(-1);} }

ParticleEffect::ParticleEffect() : Component("PARTICLE_EFFECT"), p_owner_transform(nullptr), \
								   p_texture(nullptr), texture_mode(0), max_particle_count(0), 
								   particle_lifetime(0), maximum_velocity(0.0f) { }

//Returns a pointer to the texture used for the particles.
Texture* ParticleEffect::GetTexture() {
	return p_texture;
}

//Sets a texture as the currently associated texture.
void ParticleEffect::SetTexture(Texture* p_texture_) {
	p_texture = p_texture_;
}

//Get a random particle velocity
glm::vec4 ParticleEffect::GetRandomParticleVelocity() {
	
	return glm::vec4(
		glm::linearRand(minimum_velocity.x, maximum_velocity.x),
		glm::linearRand(minimum_velocity.y, maximum_velocity.y),
		glm::linearRand(minimum_velocity.z, maximum_velocity.z),
		0.0f
	);
}

/*Serialize the ParticleEffect object from a json input
* Expects a json dict with the following keys:
* max_particle_count : The maximum number of particles for this particular 
					   particle effect
* life_time : The lifetime of the particle in milliseconds
* particle_velocity : A list of 3 floats representing the velocity 
					  for a particle in the x, y and z directions.
* texture_name : The filename of the texture present in the resources folder
* vertex_list: A 1D list of 3*4 floats. Usually 4 Vertices.
				Each group of 3 floats represents 1 vertexs x, y,and z values.
* color_list: A 1D list of 4*4 floats. Each group of 4 floats
				represents 1 vertexs r, g, b, and a color values.
* texture_list: A 1D list of 2*4 floats. Each group of 2 floats represents
				the texture coordinates for each vertex.
* Returns : void
*/
void ParticleEffect::Serialize(json json_object) {

	auto particle_velocity = json_object["maximum_particle_velocity"].get<std::vector<float>>();
	maximum_velocity = glm::vec4(particle_velocity[0], particle_velocity[1],
								 particle_velocity[2], 0.0f);

	particle_velocity = json_object["minimum_particle_velocity"].get<std::vector<float>>();
	minimum_velocity = glm::vec4(particle_velocity[0], particle_velocity[1],
		particle_velocity[2], 0.0f);

	particle_lifetime = json_object["particle_lifetime"].get<int>();
	max_particle_count = json_object["max_particle_count"].get<int>();
	for (int i = 0; i < max_particle_count; i++) {
		particles.push_back(Particle(GetRandomParticleVelocity(), particle_lifetime));
	}

	auto texture_name = json_object["texture_name"].get<std::string>();
	p_resource_manager->add_texture(texture_name);
	SetTexture(p_resource_manager->get_texture(texture_name));

	//Create a VAO and put the ID in vao_id
	glGenVertexArrays(1, &vao_id);
	//Use the same VAO for all the following operations
	glBindVertexArray(vao_id);

	//Put a vertex consisting of 3 float coordinates x,y,z into 
	//the list of all vertices
	auto vertices = json_object["vertex_list"].get<std::vector<float>>();

	//Create a continguous buffer for all the vertices/points
	GLuint point_buffer;
	glGenBuffers(1, &point_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, point_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), 
				 &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;

	//Put a color consisting of 4 float values rgba into the list of all colors
	auto colors = json_object["color_list"].get<std::vector<float>>();
	//Convert colors from 0-255 range to 0-1 range
	ConvertColor(colors);
	//Create another continuguous buffer for all the colors for each vertex
	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), &colors[0], 
				 GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;

	//Put a texture coordinate cosisting of 2 uv float values 
	auto coord = json_object["tex_coord_list"].get<std::vector<float>>();
	//Convert coords from image space to 0..1
	ConvertTextureCoords(coord, p_texture->width, p_texture->height);
	//Create another continguous buffer for all the textures for each vertex
	GLuint tex_coord_buffer;
	glGenBuffers(1, &tex_coord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coord.size(), &coord[0],
				 GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECKERROR;
	//IBO data
	GLuint indexData[] = { 0, 1, 2, 3 };
	//Create IBO
	GLuint indeces_buffer;
	glGenBuffers(1, &indeces_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeces_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData,
				 GL_STATIC_DRAW);
	CHECKERROR;
	glBindVertexArray(0);

	SetTextureMode(0);
}

/*Update each particle in the particle effect.
* Adjust the lifetime.
* Move the particle according to the velocity.
* If a particle has reached it's lifetime, 
* spawn a new one in its place
* Returns : void
*/
void ParticleEffect::Update() {
	//Respawn a max of 2 new particles per frame
	for (unsigned int i = 0; i < particles_spawned_per_frame; i++) {
		RespawnParticle(FirstUnusedParticle());
	}

	for (auto &particle : particles) {
		particle.life_time -= p_framerate_controller->GetPrevLoopDeltaTime();
		particle.position += particle.velocity;
	}

	//Debug mode
	if (p_game_manager->GetDegugMode())
		SetTextureMode(0);
	else
		SetTextureMode(1);
}

//Global to keep track of the last used particle
unsigned int lastUsedParticle = 0;

//Get the index of a dead particle
int ParticleEffect::FirstUnusedParticle() {
	for (unsigned int i = lastUsedParticle; i < max_particle_count; ++i) {
		if (particles[i].life_time <= 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].life_time <= 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	//Override the first particle if none are available
	lastUsedParticle = 0;
	return 0;
}

//Get a random particle velocity
void ParticleEffect::RespawnParticle(int particle_index) {
	particles[particle_index].life_time = particle_lifetime;
	particles[particle_index].position = glm::vec4(0.0f);
	particles[particle_index].velocity = GetRandomParticleVelocity();
}

/*Draw all the particles that are part of this particle effect
* Draw them offset at the parent object's transform component's position.
* 
*/
void ParticleEffect::Draw(ShaderProgram* program) {
	glm::mat4 final_translate_matrix;
	GLuint loc;
	glm::mat4 translate_matrix = p_owner_transform->GetTranslateMatrix();
	glm::vec4 owner_position = p_owner_transform->GetPosition();
	translate_matrix[3][0] += owner_position.z/2; //z is the width
	translate_matrix[3][1] += owner_position.w/2; //w is the height

	loc = glGetUniformLocation(program->program_id, "rotateMatrix");
	glm::mat4 rotate_matrix = p_owner_transform->GetRotateMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(rotate_matrix));
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "scaleMatrix");
	glm::mat4 scale_matrix = p_owner_transform->GetScaleMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(scale_matrix));
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "preRotateMatrix");
	glm::mat4 pre_rotate_matrix = p_owner_transform->GetPreRotateMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(pre_rotate_matrix));
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "postRotateMatrix");
	glm::mat4 post_rotate_matrix = p_owner_transform->GetPostRotateMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(post_rotate_matrix));
	CHECKERROR;

	glActiveTexture(GL_TEXTURE2); // Activate texture unit 2
	glBindTexture(GL_TEXTURE_2D, p_texture->texture_id); // Load texture into it
	loc = glGetUniformLocation(program->program_id, "texture_map");
	glUniform1i(loc, 2); // Tell shader texture is in unit 2
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "mode");
	glUniform1i(loc, texture_mode);
	CHECKERROR;

	loc = glGetUniformLocation(program->program_id, "particle");
	glUniform1i(loc, 1);
	CHECKERROR;

	glBindVertexArray(vao_id);
	CHECKERROR;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	for (auto &particle : particles) {
		if (particle.life_time > 0)
		{
			loc = glGetUniformLocation(program->program_id, "translateMatrix");
			final_translate_matrix = translate_matrix;
			final_translate_matrix[3][0] += particle.position.x;
			final_translate_matrix[3][1] += particle.position.y;
			final_translate_matrix[3][2] += particle.position.z;
			glUniformMatrix4fv(loc, 1, GL_FALSE, getMat4Pointer(final_translate_matrix));
			CHECKERROR;
			glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(0);
}

/*Links the Particle Effect component with it's related components
* Related components : Transform
* Returns : void
*/
void ParticleEffect::Link() {
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}

//Sets the render mode to color (int=0) or texture (int=1)
void ParticleEffect::SetTextureMode(int mode_) {
	texture_mode = mode_;
}