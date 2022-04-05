/******************************************************************************/
/*!
/*File: FBO.h
/*Author: Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   2/24/2022
/*  Header file for the Frame buffer object abstraction
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

typedef unsigned int GLuint;

class FBO {

private:
    int width, height;  // Size of the texture.
    GLuint color_attachment_count;
public:
    GLuint fboID = 0;
    GLuint depthBufferID = 0;
    GLuint textureID[4] = { 0, 0, 0, 0 };
    /*Constructor for the FBO object
    * Creates an FBO with a specified width, height and 
    * number of color_attachments
    */
    FBO(const int w, const int h, const int _color_attachment_count = 1);
    /*Deletes an FBO by deleting the associated texture 
    * and frame buffer
    */
    ~FBO();
    /* Bind this FBO so it's used as the render target for the next draw call
    * Returns: void
    */
    void Bind();
    /* UnBind this FBO so that the screen is the render target for the next 
    *  draw call
    * Returns: void
    */
    void Unbind();
    /* Bind this FBOs texture to a texture unit so it can be accessed by 
    * the shader.
    * Returns: void
    */
    void BindTexture(const int program_id, const int texture_unit, const char* var_name, const int color_attachment = 0);
    /* Unbind a texture from a texture
    * Returns: void
    */
    void UnbindTexture(const int texture_unit);
};
