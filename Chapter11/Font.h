//
//  Font.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 23.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef Font_hpp
#define Font_hpp

#include <string>
#include <unordered_map>
#include <SDL/SDL_ttf.h>
#include "Math.h"

class Font
{
public:
    Font(class Game* game);
    ~Font();
    
    // Load/unload from a file
    bool Load(const std::string& fileName);
    void Unload();
    
    // Given string and this font, draw to a texture
    class Texture* RenderText(const std::string& textKey, const Vector3& color = Color::White, int pointSize = 30);
    
private:
    // Map of point size to font data
    std::unordered_map<int, TTF_Font*> mFontData;
    class Game* mGame;
};

#endif /* Font_hpp */
