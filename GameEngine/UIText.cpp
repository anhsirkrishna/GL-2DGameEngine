#include "UIText.h"
#include "ResourceManager.h"
#include "GraphicsManager.h"
#include "CharacterTextures.h"
#include "Transform.h"
#include "GameObject.h"
#include "GameManager.h"
#include "ShaderProgram.h"
#include "Util.h"

UIText::UIText() : Component("UITEXT"), text(), pixel_position(), 
					scale(), vertex_coords(), color_coords(),
					tex_coords(), vertex_buffer_id(0), 
					color_buffer_id(0), texture_buffer_id(0), 
					vao_id(), visible(true), texture_mode(1), 
					p_owner_transform(nullptr), p_font_textures(nullptr) {
	
}

/*Serialize the UIText from a json object
* Expects a dict with the following key:values
* pixel_position : [float, float]
* scale : [float, float]
* font_name : string
* text_color: [float, float, float, float]
* text : string
* Returns: void
*/
void UIText::Serialize(json json_object) {
	auto font_name = json_object["font_name"].get<std::string>();
	p_resource_manager->add_font_textures(font_name);
	p_font_textures = p_resource_manager->get_font_textures(font_name);

	pixel_position[0] = json_object["pixel_position"].get<std::vector<int>>()[0];
	pixel_position[1] = json_object["pixel_position"].get<std::vector<int>>()[1];

	scale.x = json_object["scale"].get<std::vector<float>>()[0];
	scale.y = json_object["scale"].get<std::vector<float>>()[1];

	text = json_object["text"].get<std::string>();

	GenerateTilemapVertices();

	auto text_color = json_object["text_color"].get<std::vector<int>>();
	//Bottom left vertex
	for (unsigned int i = 0; i < 4; ++i) {
		color_coords.push_back(text_color[0]);
		color_coords.push_back(text_color[1]);
		color_coords.push_back(text_color[2]);
		color_coords.push_back(text_color[3]);
	}

	//Bottom left vertex
	tex_coords.push_back(0.0f);
	tex_coords.push_back(0.0f);

	//Top left vertex
	tex_coords.push_back(0.0f);
	tex_coords.push_back(1.0f);

	//Top right vertex
	tex_coords.push_back(1.0f);
	tex_coords.push_back(1.0f);

	//Bottom right vertex
	tex_coords.push_back(1.0f);
	tex_coords.push_back(0.0f);

	ConvertColor(color_coords);

	vao_id = p_graphics_manager->GenerateDynamicQuadVAO(vertex_buffer_id,
		color_buffer_id, texture_buffer_id);
}

void UIText::Link() {
	p_owner_transform =
		static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
	p_owner_transform->SetPosition(glm::vec4(pixel_position.x, pixel_position.y, 0, 1));
	p_owner_transform->SetScale(scale.x, scale.y);
}

/*Populates the vertex array object for
* vertex coords
* Returns: void
*/
void UIText::GenerateTilemapVertices() {
	std::string::const_iterator c;
	float char_x_pos = 0;
	float char_y_pos = 0;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = p_font_textures->character_map[*c];

		float xpos = char_x_pos + ch.bearing.x;
		float ypos = char_y_pos - (ch.size.y - ch.bearing.y);

		float w = ch.size.x;
		float h = ch.size.y;
		std::vector<GLfloat> temp_vec;
		temp_vec.push_back(xpos);
		temp_vec.push_back(ypos - h);
		temp_vec.push_back(0.0f);

		temp_vec.push_back(xpos);
		temp_vec.push_back(ypos);
		temp_vec.push_back(0.0f);

		temp_vec.push_back(xpos + w);
		temp_vec.push_back(ypos);
		temp_vec.push_back(0.0f);

		temp_vec.push_back(xpos + w);
		temp_vec.push_back(ypos - h);
		temp_vec.push_back(0.0f);

		vertex_coords.push_back(temp_vec);

		//(advance is number of 1/64 pixels)
		char_x_pos += (ch.advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
	}
}

/*Draw the UIText on the screen with an
* orthographic projection
* Returns: void
*/
void UIText::Draw(ShaderProgram* p_program) {
	p_graphics_manager->SetActiveShader("ui");
	p_graphics_manager->SetDepthTestOff();
	p_graphics_manager->SetOrthographicMatrix();

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

	p_graphics_manager->SetDynamicBufferData(vao_id, color_buffer_id, &color_coords[0],
		sizeof(float) * color_coords.size());

	p_graphics_manager->SetDynamicBufferData(vao_id, texture_buffer_id, &tex_coords[0],
		sizeof(float) * tex_coords.size());

	if (p_game_manager->GetDegugMode())
		texture_mode = 0;
	else
		texture_mode = 1;

	p_graphics_manager->SetUniformInt(texture_mode, "mode");

	p_graphics_manager->SetUniformInt(1, "text");

	//Draw a separate quad for each text character
	std::string::const_iterator c;
	int indx = 0;
	for (c = text.begin(); c != text.end(); c++) {

		p_graphics_manager->SetDynamicBufferData(vao_id, vertex_buffer_id, &vertex_coords[indx][0],
			sizeof(float) * vertex_coords[indx].size());

		p_graphics_manager->BindTexture(2, p_font_textures->character_map[*c].texture_id, "texture_map");

		p_graphics_manager->DrawQuad(vao_id, 1, false);

		indx++;
	}

	p_graphics_manager->SetDepthTestOn();
	p_graphics_manager->SetActiveShader("final");
}

void UIText::Update() {

}
