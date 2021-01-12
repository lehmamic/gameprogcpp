//
//  SpriteComponent.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
    :Component(owner)
    ,mTexture(nullptr)
    ,mDrawOrder(drawOrder)
    ,mTextWidth(0)
    ,mTextHeight(0)
{
    mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
    mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer) {
    if (mTexture) {
        SDL_Rect r;
        
        // Scale the width/height by owner's scale
        r.w = static_cast<int>(mTextWidth * mOwner->GetScale());
        r.h = static_cast<int>(mTextHeight * mOwner->GetScale());
        
        // Center the rectangle around the position of the owner
        r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
        r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);
        
        // Draw (have to convert angle from radians to degrees, and clockwise to counter)
        SDL_RenderCopyEx(
                         renderer,  // Renderer
                         mTexture,  // Texture to draw
                         nullptr,
                         &r,        // Destination rectangle
                         -Math::ToDegrees(mOwner->GetRotation()), // (Convert angle)
                         nullptr,   // Point of rotation
                         SDL_FLIP_NONE); // Flip behavior
    }
}

void SpriteComponent::SetTexture(SDL_Texture *texture) {
    mTexture = texture;
    
    // Get width/height of texture
    SDL_QueryTexture(texture, nullptr, nullptr, &mTextWidth, &mTextHeight);
}
