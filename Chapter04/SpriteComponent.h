//
//  SpriteComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef SpriteComponent_hpp
#define SpriteComponent_hpp

#include <SDL/SDL.h>
#include "Component.h"

class SpriteComponent : public Component {
public:
    // (Lower draw order correcsponds with further back)
    SpriteComponent(class Actor* owner, int drawOrder = 100);
    ~SpriteComponent();
    
    virtual void Draw(SDL_Renderer* renderer);
    virtual void SetTexture(SDL_Texture* texture);
    
    int GetDrawOrder() const { return mDrawOrder; }
    int GetTextHeight() const { return mTextHeight; }
    int GetTextWidth() const { return mTextWidth; }
    
protected:
    // Texture to draw
    SDL_Texture* mTexture;
    // Draw order used for painter's algorithm
    int mDrawOrder;
    // Width/height of SDL_Texture
    int mTextWidth;
    int mTextHeight;
};

#endif /* SpriteComponent_hpp */
