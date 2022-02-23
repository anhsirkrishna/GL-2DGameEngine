#include "GLQuad.h"
#include "ShaderProgram.h"
#include "Transform.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Util.h"
#include "Camera.h"
#include "Texture.h"
#include "GameManager.h"
#include "GraphicsManager.h"

GLQuad::GLQuad() : Component("GLQUAD"), p_texture(NULL), vao_id(0),
					p_owner_transform(NULL), texture_mode(0), vertex_count(0), p_texure_list(),
					dimensions(0.0f) {
	tex_offset[0] = tex_offset[1] = 0;
}


void GLQuad::Draw(ShaderProgram* program) {
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

	p_texture->Bind(2, p_graphics_manager->GetActiveShader()->program_id,
					"texture_map");

	glm::vec2 converted_tex_offset;
	converted_tex_offset[0] = tex_offset[0] / p_texture->width;
	converted_tex_offset[1] = tex_offset[1] / p_texture->height;

	p_graphics_manager->SetUniformVec2(converted_tex_offset, "tex_offset");

	if (p_game_manager->GetDegugMode())
		SetTextureMode(0);
	else
		SetTextureMode(1);

	p_graphics_manager->SetUniformInt(texture_mode, "mode");

	p_graphics_manager->SetUniformInt(0, "particle");

	p_graphics_manager->DrawQuad(vao_id);
}

/*Links the GLQuad component with it's related components
* Related components : Transform
* Returns : void
*/
void GLQuad::Link() {
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}

void GLQuad::SetTexOffset(GLfloat tex_offset_x, GLfloat tex_offset_y) {
	tex_offset[0] = tex_offset_x;
	tex_offset[1] = tex_offset_y;
}

//Sets the render mode to color (int=0) or texture (int=1)
void GLQuad::SetTextureMode(int _mode) {
	texture_mode = _mode;
}

Texture* GLQuad::GetTexture() {
	return p_texture;
}

void GLQuad::AddTexture(Texture* _p_texture) {
	p_texure_list.push_back(_p_texture);
}

void GLQuad::SetTexture(unsigned int index) {
	p_texture = p_texure_list[index];
}

void GLQuad::ClearTextures() {
	p_texure_list.clear();
}

/*Serialize the GLquad object from a json input
* Expects a json dict with the following keys:
* texture_names : list of texture file names present in the resources folder
* vertex_list: A 1D list of 3*4 floats. Usually 4 Vertices.
			   Each group of 3 floats represents 1 vertexs x, y,and z values.
* color_list: A 1D list of 4*4 floats. Each group of 4 floats 
			  represents 1 vertexs r, g, b, and a color values.
* texture_list: A 1D list of 2*4 floats. Each group of 2 floats represents 
				the texture coordinates for each vertex.
* Returns : void
*/
void GLQuad::Serialize(json json_object) {
	auto texture_names = json_object["texture_names"].get<std::vector<std::string>>();
	for (auto texture_name : texture_names) {
		p_resource_manager->add_texture(texture_name);
		AddTexture(p_resource_manager->get_texture(texture_name));
	}
	SetTexture(0);

	//Put a vertex consisting of 3 float coordinates x,y,z into the list of all vertices
	auto vertices = json_object["vertex_list"].get<std::vector<float>>();
	vertex_count = vertices.size() / 3;

	//Get the dimensions of the Quad
	dimensions.x = vertices[6] - vertices[0];
	dimensions.y = vertices[4] - vertices[1];

	//Put a color consisting of 4 float values rgba into the list of all colors 
	auto colors = json_object["color_list"].get<std::vector<float>>();
	//Convert colors from 0-255 range to 0-1 range
	ConvertColor(colors);

	//Put a texture coordinate cosisting of 2 uv float values 
	auto tex_coord = json_object["tex_coord_list"].get<std::vector<float>>();
	//Convert coords from image space to 0..1
	ConvertTextureCoords(tex_coord, p_texture->width, p_texture->height);

	vao_id = p_graphics_manager->GenerateQuadVAO(&vertices[0], &colors[0], &tex_coord[0]);
}


/*Change the state of the GLQuad component
* Involves changing the texture that this quad is pointing to.
* Returns : void
*/
void GLQuad::ChangeState(json json_object) {
	ClearTextures();
	auto texture_names = json_object["texture_names"].get<std::vector<std::string>>();
	for (auto texture_name : texture_names) {
		p_resource_manager->add_texture(texture_name);
		AddTexture(p_resource_manager->get_texture(texture_name));
	}
	SetTexture(0);
}

//Get the quad width and height
glm::vec2 GLQuad::GetDimensions() {
	return dimensions;
}