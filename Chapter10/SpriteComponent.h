//
//  SpriteComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef SpriteComponent_hpp
#define SpriteComponent_hpp

#include "Component.h"
#include <SDL/SDL.h>

class SpriteComponent : public Component {
public:
    // (Lower draw order correcsponds with further back)
    SpriteComponent(class Actor* owner, int drawOrder = 100);
    ~SpriteComponent();
    
    virtual void Draw(class Shader* shader);
    virtual void SetTexture(class Texture* texture);
    
    int GetDrawOrder() const { return mDrawOrder; }
    int GetTexHeight() const { return mTexHeight; }
    int GetTexWidth() const { return mTexWidth; }
    
    void SetVisible(bool visible) { mVisible = visible; }
    bool GetVisible() const { return mVisible; }
    
protected:
    // Texture to draw
    class Texture* mTexture;
    
    // Draw order used for painter's algorithm
    int mDrawOrder;
    
    // Width/height of SDL_Texture
    int mTexWidth;
    int mTexHeight;
    
    bool mVisible;
};

#endif /* SpriteComponent_hpp */
