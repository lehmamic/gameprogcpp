//
//  Texture.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 14.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <string>

class Texture {
public:
    Texture();
    ~Texture();
    
    bool Load(const std::string& fileName);
    void Unload();
    
    void SetActive();
    
    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
    
private:
    // OpenGL ID of this texture
    unsigned int mTextureID;
    
    // Width/height of the Texture
    int mWidth;
    int mHeight;
};

#endif /* Texture_hpp */
