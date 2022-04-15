/******************************************************************************/
/*!
/*File   CharacterTextures.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/* Character Texture class implmentation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "CharacterTextures.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <SDL.h>

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

/*Creates a map of characters and their associated textures
* that can be used to render a character using the font type
* specified in the argument
*/
CharacterTextures::CharacterTextures(std::string ttf_filepath) {
    FT_Library ft = nullptr;
    FT_Face face = nullptr;

    SDL_assert(FT_Init_FreeType(&ft) == 0);

    SDL_assert(FT_New_Face(ft, ttf_filepath.c_str(), 0, &face) == 0);

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //Create a texture for the first 128 ASCII characters
    for (unsigned char c = 0; c < 128; c++)
    {
        //Load glyph onto face
        SDL_assert(FT_Load_Char(face, c, FT_LOAD_RENDER) == 0);

        //Generate texture for character and ship to GPU
        GLuint tex_id;
        glGenTextures(1, &tex_id);
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        //Save the tex_id and tex information in the map
        Character character = {
            tex_id,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        character_map.insert(std::pair<char, Character>(c, character));
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}