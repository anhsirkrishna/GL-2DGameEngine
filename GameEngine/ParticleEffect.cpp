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
#include "Events.h"

#include <gtc/random.hpp>

unsigned int particles_spawned_per_frame = 2;


ParticleEffect::ParticleEffect() : Component("PARTICLE_EFFECT"), p_owner_transform(nullptr), \
								   p_texture(nullptr), texture_mode(0), max_particle_count(0), 
								   particle_lifetime(0), maximum_velocity(0.0f),
								   minimum_velocity(0.0f), origin_offset(0.0f), vao_id(0),
								   vertex_buffer_id(0), lastUsedParticle(0) {
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
	
	p_resource_manager->add_shader("particles");
	p_graphics_manager->SetActiveShader("particles");
	p_graphics_manager->BindDefaultAttribLocations();
	p_graphics_manager->BindOutputAttrib(0, "out_Color");
	p_graphics_manager->BindOutputAttrib(1, "post_Buffer");

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


	auto texture_name = json_object["texture_name"].get<std::string>();
	p_resource_manager->add_texture(texture_name);
	SetTexture(p_resource_manager->get_texture(texture_name));

	//Put a vertex consisting of 3 float coordinates x,y,z into 
	//the list of all vertices
	single_particle_vertices = json_object["vertex_list"].get<std::vector<float>>();

	//Put a color consisting of 4 float values rgba into the list of all colors
	auto base_colors = json_object["color_list"].get<std::vector<float>>();
	//Convert colors from 0-255 range to 0-1 range
	ConvertColor(base_colors);

	//Put a texture coordinate cosisting of 2 uv float values 
	auto base_coord = json_object["tex_coord_list"].get<std::vector<float>>();
	//Convert coords from image space to 0..1
	ConvertTextureCoords(base_coord, p_texture->width, p_texture->height);

	std::vector<float> coord;

	for (int i = 0; i < max_particle_count; i++) {
		particles.push_back(Particle(glm::vec4(origin_offset, -1.0f, 0.0f),
			GetRandomParticleVelocity(), particle_lifetime));
		
		//Top left vertex x, y and z
		particle_vertex_list.push_back(single_particle_vertices[0]);
		particle_vertex_list.push_back(single_particle_vertices[1]);
		particle_vertex_list.push_back(single_particle_vertices[2]);

		//Top right vertex x, y and z
		particle_vertex_list.push_back(single_particle_vertices[3]);
		particle_vertex_list.push_back(single_particle_vertices[4]);
		particle_vertex_list.push_back(single_particle_vertices[5]);

		//Bottom left vertex x, y and z
		particle_vertex_list.push_back(single_particle_vertices[6]);
		particle_vertex_list.push_back(single_particle_vertices[7]);
		particle_vertex_list.push_back(single_particle_vertices[8]);

		//Bottom right vertex x, y and z
		particle_vertex_list.push_back(single_particle_vertices[9]);
		particle_vertex_list.push_back(single_particle_vertices[10]);
		particle_vertex_list.push_back(single_particle_vertices[11]);

		//All vertices are going to have the same color
		particle_color_list.push_back(base_colors[0]);
		particle_color_list.push_back(base_colors[1]);
		particle_color_list.push_back(base_colors[2]);
		particle_color_list.push_back(base_colors[3]);

		particle_color_list.push_back(base_colors[4]);
		particle_color_list.push_back(base_colors[5]);
		particle_color_list.push_back(base_colors[6]);
		particle_color_list.push_back(base_colors[7]);

		particle_color_list.push_back(base_colors[8]);
		particle_color_list.push_back(base_colors[9]);
		particle_color_list.push_back(base_colors[10]);
		particle_color_list.push_back(base_colors[11]);

		particle_color_list.push_back(base_colors[12]);
		particle_color_list.push_back(base_colors[13]);
		particle_color_list.push_back(base_colors[14]);
		particle_color_list.push_back(base_colors[15]);


		coord.push_back(base_coord[0]);
		coord.push_back(base_coord[1]);

		coord.push_back(base_coord[2]);
		coord.push_back(base_coord[3]);

		coord.push_back(base_coord[4]);
		coord.push_back(base_coord[5]);

		coord.push_back(base_coord[6]);
		coord.push_back(base_coord[7]);
	}

	vao_id = p_graphics_manager->GenerateDynamicQuadVAO(
		vertex_buffer_id, color_buffer_id, &coord[0], max_particle_count);

	p_graphics_manager->SetActiveShader("final");
}

/*Update each particle in the particle effect.
* Adjust the lifetime.
* Move the particle according to the velocity.
* If a particle has reached it's lifetime, 
* spawn a new one in its place
* Returns : void
*/
void ParticleEffect::Update() {
	Uint32 dt = p_framerate_controller->GetPrevLoopDeltaTime();

	//Respawn a max of 2 new particles per frame
	for (unsigned int j = 0; j < particles_spawned_per_frame; j++) {
		RespawnParticle(GetLastUsedParticle());
	}
	unsigned int vertex_index = 0;
	
	float z = single_particle_vertices[2];
	for (unsigned int i=0; i < particles.size(); i++) {
		particles[i].life_time -= dt;
		particles[i].position += (particles[i].velocity * glm::vec4(dt / 1000.0f));

		vertex_index = i * 12;
		//Update the vertex list to send to the GPU
		//Top left vertex x, y and z
		particle_vertex_list[vertex_index] = (single_particle_vertices[0] + particles[i].position.x);
		particle_vertex_list[vertex_index + 1] = (single_particle_vertices[1] + particles[i].position.y);
		particle_vertex_list[vertex_index + 2] = z;

		//Top right vertex x, y and z
		particle_vertex_list[vertex_index + 3] = (single_particle_vertices[3] + particles[i].position.x);
		particle_vertex_list[vertex_index + 4] = (single_particle_vertices[4] + particles[i].position.y);
		particle_vertex_list[vertex_index + 5] = z;

		//Bottom left vertex x, y and z
		particle_vertex_list[vertex_index + 6] = (single_particle_vertices[6] + particles[i].position.x);
		particle_vertex_list[vertex_index + 7] = (single_particle_vertices[7] + particles[i].position.y);
		particle_vertex_list[vertex_index + 8] = z;

		//Bottom right vertex x, y and z
		particle_vertex_list[vertex_index + 9] = (single_particle_vertices[9] + particles[i].position.x);
		particle_vertex_list[vertex_index + 10] = (single_particle_vertices[10] + particles[i].position.y);
		particle_vertex_list[vertex_index + 11] = z;

		z += 0.01;
	}

	//Debug mode
	if (p_game_manager->GetDegugMode())
		SetTextureMode(0);
	else
		SetTextureMode(1);

}

//Get the index of a dead particle
int ParticleEffect::GetLastUsedParticle() {
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
	p_graphics_manager->SetActiveShader("particles");

	p_graphics_manager->SetProjectionMatrix();
	p_graphics_manager->SetViewMatrix();

	glm::mat4 final_translate_matrix;
	GLuint loc;
	glm::mat4 translate_matrix = p_owner_transform->GetTranslateMatrix();

	p_graphics_manager->SetUniformMatrix4(
		p_owner_transform->GetTranslateMatrix(), "translateMatrix");

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

	p_graphics_manager->SetAlphaBlendingOn();

	p_graphics_manager->SetDynamicBufferData(vao_id, vertex_buffer_id, &particle_vertex_list[0],
		sizeof(float) * particle_vertex_list.size());

	p_graphics_manager->SetDynamicBufferData(vao_id, color_buffer_id, &particle_color_list[0],
		sizeof(float) * particle_vertex_list.size());

	p_graphics_manager->DrawQuad(vao_id, max_particle_count);

	p_graphics_manager->SetAlphaBlendingOff();

	p_graphics_manager->SetActiveShader("final");
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

/*Respawn a set of particles with
* faster velocities and larger lifetime
* to produce a "Burst" effect
* Returns: void
*/
void ParticleEffect::BurstEffect(int burst_size) {
	//Respawn a max of 2 new particles per frame
	unsigned int particle_index;
	glm::vec4 new_velocity;
	for (unsigned int j = 0; j < burst_size; j++) {
		particle_index = GetLastUsedParticle();

		particles[particle_index].life_time = particle_lifetime;
		particles[particle_index].position = glm::vec4(origin_offset, 0.0f, 0.0f);
		new_velocity = GetRandomParticleVelocity();
		particles[particle_index].velocity.x = new_velocity.x * 50;
		particles[particle_index].velocity.y = new_velocity.y * 50;
		particles[particle_index].velocity.z = new_velocity.z * 50;
	}
}

/*Handle an event sent to this component
* Temporary behavior of producing a "Burst"
* effect
* Returns: void
*/
void ParticleEffect::HandleEvent(TimedEvent* p_event) {
	if (p_event->event_id == EventID::disable) {
		ResetParticles();
	}
}

/*Resets all the particles to their starting position
* Returns: void
*/
void ParticleEffect::ResetParticles() {
	for (unsigned int i = 0; i < particles.size(); i++) {
		RespawnParticle(i);
	}
}

/*Change the state of the ParticleEffect
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
void ParticleEffect::ChangeState(json json_object) {

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


	auto texture_name = json_object["texture_name"].get<std::string>();
	p_resource_manager->add_texture(texture_name);
	SetTexture(p_resource_manager->get_texture(texture_name));

	//Put a vertex consisting of 3 float coordinates x,y,z into 
	//the list of all vertices
	single_particle_vertices = json_object["vertex_list"].get<std::vector<float>>();

	//Put a color consisting of 4 float values rgba into the list of all colors
	auto base_colors = json_object["color_list"].get<std::vector<float>>();
	//Convert colors from 0-255 range to 0-1 range
	ConvertColor(base_colors);

	particles.clear();
	particle_vertex_list.clear();
	particle_color_list.clear();

	for (int i = 0; i < max_particle_count; i++) {
		particles.push_back(Particle(glm::vec4(origin_offset, -1.0f, 0.0f),
			GetRandomParticleVelocity(), particle_lifetime));

		//Top left vertex x, y and z
		particle_vertex_list.push_back(single_particle_vertices[0]);
		particle_vertex_list.push_back(single_particle_vertices[1]);
		particle_vertex_list.push_back(single_particle_vertices[2]);

		//Top right vertex x, y and z
		particle_vertex_list.push_back(single_particle_vertices[3]);
		particle_vertex_list.push_back(single_particle_vertices[4]);
		particle_vertex_list.push_back(single_particle_vertices[5]);

		//Bottom left vertex x, y and z
		particle_vertex_list.push_back(single_particle_vertices[6]);
		particle_vertex_list.push_back(single_particle_vertices[7]);
		particle_vertex_list.push_back(single_particle_vertices[8]);

		//Bottom right vertex x, y and z
		particle_vertex_list.push_back(single_particle_vertices[9]);
		particle_vertex_list.push_back(single_particle_vertices[10]);
		particle_vertex_list.push_back(single_particle_vertices[11]);

		//All vertices are going to have the same color
		particle_color_list.push_back(base_colors[0]);
		particle_color_list.push_back(base_colors[1]);
		particle_color_list.push_back(base_colors[2]);
		particle_color_list.push_back(base_colors[3]);

		particle_color_list.push_back(base_colors[4]);
		particle_color_list.push_back(base_colors[5]);
		particle_color_list.push_back(base_colors[6]);
		particle_color_list.push_back(base_colors[7]);

		particle_color_list.push_back(base_colors[8]);
		particle_color_list.push_back(base_colors[9]);
		particle_color_list.push_back(base_colors[10]);
		particle_color_list.push_back(base_colors[11]);

		particle_color_list.push_back(base_colors[12]);
		particle_color_list.push_back(base_colors[13]);
		particle_color_list.push_back(base_colors[14]);
		particle_color_list.push_back(base_colors[15]);
	}
}