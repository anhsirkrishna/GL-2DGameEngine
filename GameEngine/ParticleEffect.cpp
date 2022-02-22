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
#include "MemoryManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"

#include <gtc/random.hpp>

unsigned int particles_spawned_per_frame = 2;


ParticleEffect::ParticleEffect() : Component("PARTICLE_EFFECT"), p_owner_transform(nullptr), \
								   p_texture(nullptr), texture_mode(0), max_particle_count(0), 
								   particle_lifetime(0), maximum_velocity(0.0f),
								   minimum_velocity(0.0f), origin_offset(0.0f) {
}

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

	auto particle_velocity = 
		json_object["maximum_particle_velocity"].get<std::vector<float>>();
	maximum_velocity = glm::vec4(particle_velocity[0], particle_velocity[1],
								 particle_velocity[2], 0.0f);

	particle_velocity = json_object["minimum_particle_velocity"].get<std::vector<float>>();
	minimum_velocity = glm::vec4(particle_velocity[0], particle_velocity[1],
		particle_velocity[2], 0.0f);

	particle_lifetime = json_object["particle_lifetime"].get<int>();
	max_particle_count = json_object["max_particle_count"].get<int>();

	auto offset = json_object["particle_offset"].get<std::vector<int>>();
	origin_offset.x = offset[0];
	origin_offset.y = offset[1];

	for (int i = 0; i < max_particle_count; i++) {
		particles.push_back(Particle(glm::vec4(origin_offset, glm::vec2(0.0f)),
									 GetRandomParticleVelocity(), particle_lifetime));
	}

	auto texture_name = json_object["texture_name"].get<std::string>();
	p_resource_manager->add_texture(texture_name);
	SetTexture(p_resource_manager->get_texture(texture_name));

	//Put a vertex consisting of 3 float coordinates x,y,z into 
	//the list of all vertices
	auto vertices = json_object["vertex_list"].get<std::vector<float>>();

	//Put a color consisting of 4 float values rgba into the list of all colors
	auto colors = json_object["color_list"].get<std::vector<float>>();
	//Convert colors from 0-255 range to 0-1 range
	ConvertColor(colors);

	//Put a texture coordinate cosisting of 2 uv float values 
	auto coord = json_object["tex_coord_list"].get<std::vector<float>>();
	//Convert coords from image space to 0..1
	ConvertTextureCoords(coord, p_texture->width, p_texture->height);

	vao_id = p_graphics_manager->GenerateQuadVAO(&vertices[0], &colors[0], &coord[0]);
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
	particles[particle_index].position = glm::vec4(origin_offset, 0.0f, 0.0f);
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

	p_graphics_manager->SetUniformMatrix4(
		p_owner_transform->GetRotateMatrix(), "rotateMatrix");

	p_graphics_manager->SetUniformMatrix4(
		p_owner_transform->GetScaleMatrix(), "scaleMatrix");

	p_graphics_manager->SetUniformMatrix4(
		p_owner_transform->GetPreRotateMatrix(), "preRotateMatrix");

	p_graphics_manager->SetUniformMatrix4(
		p_owner_transform->GetPostRotateMatrix(), "postRotateMatrix");

	p_texture->Bind(2, p_graphics_manager->GetActiveShader()->program_id, "texture_map");

	p_graphics_manager->SetUniformInt(texture_mode, "mode");

	p_graphics_manager->SetUniformInt(1, "particle");

	p_graphics_manager->SetUniformVec2(glm::vec2(0, 0), "tex_offset");

	p_graphics_manager->SetAlphaBlendingOn();
	p_graphics_manager->SetDepthTestOff();
	float particle_lifetime_var;
	for (auto &particle : particles) {
		if (particle.life_time > 0)
		{
			final_translate_matrix = translate_matrix;
			final_translate_matrix[3][0] += particle.position.x;
			final_translate_matrix[3][1] += particle.position.y;
			final_translate_matrix[3][2] += particle.position.z;
			particle_lifetime_var = (float)particle.life_time / particle_lifetime;
			p_graphics_manager->SetUniformFloat(particle_lifetime_var, "particle_lifetime");
			p_graphics_manager->SetUniformMatrix4(
				final_translate_matrix, "translateMatrix");
			p_graphics_manager->DrawQuad(vao_id);
		}
	}
	p_graphics_manager->SetDepthTestOn();
	p_graphics_manager->SetAlphaBlendingOff();
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