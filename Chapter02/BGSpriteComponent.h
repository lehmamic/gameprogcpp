//
//  BackgroundSpriteComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef BackgroundSpriteComponent_hpp
#define BackgroundSpriteComponent_hpp

#import "SpriteComponent.h"
#import <SDL/SDL.h>
#import <vector>
#import "Math.h"

class BGSpriteComponent : public SpriteComponent {
public:
    // Set draw order to default to lower (so it0s in the background)
    BGSpriteComponent(class Actor* owner, int drawOrder = 10);
    
    // Update/draw overriden from parent
    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer) override;
    
    // Set th etextures used for the background
    void SetBGTextures(const std::vector<SDL_Texture*>& textures);
    
    // Get/set screen size and scroll speed
    void SetScreenSize(const Vector2& size) { mScreenSize = size; }
    void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
    
private:
    // Struct to encapsulate each BG image and its offset
    struct BGTexture {
        SDL_Texture* mTexture;
        Vector2 mOffset;
    };
    
    std::vector<BGTexture> mBGTextures;
    Vector2 mScreenSize;
    float mScrollSpeed;
};

#endif /* BackgroundSpriteComponent_hpp */
