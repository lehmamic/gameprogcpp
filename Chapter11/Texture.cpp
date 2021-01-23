//
//  Texture.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 14.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "Texture.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <SOIL/SOIL.h>

Texture::Texture()
:mTextureID(0)
,mWidth(0)
,mHeight(0)
{
}

Texture::~Texture()
{
}

bool Texture::Load(const std::string &fileName)
{
    int channels = 0;
    unsigned char* image = SOIL_load_image(
       fileName.c_str(),    // Name of the file
       &mWidth,             // Stores width
       &mHeight,            // Stores height
       &channels,           // Stores number of channels
       SOIL_LOAD_AUTO);     // Type of image file, or auto form any
    
    if (image == nullptr)
    {
        SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
        return false;
    }
    
    int format = GL_RGB;
    if (channels == 4)
    {
        format = GL_RGBA;
    }
    
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    
    glTexImage2D(
        GL_TEXTURE_2D,      // Texture target
        0,                  // Level of detail (for now, assume 0)
        format,             // Color format OPENGL should use
        mWidth,             // Width of texture
        mHeight,            // Height of texture
        0,                  // Border - "this value must be null"
        format,             // Color format of input data
        GL_UNSIGNED_BYTE,   // Bit depth of input data
        image);             // Pointer to image data
    
    SOIL_free_image_data(image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return true;
}

void Texture::Unload()
{
    glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}
