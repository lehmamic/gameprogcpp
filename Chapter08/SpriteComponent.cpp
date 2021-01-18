//
//  SpriteComponent.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
    :Component(owner)
    ,mTexture(nullptr)
    ,mDrawOrder(drawOrder)
    ,mTexWidth(0)
    ,mTexHeight(0)
{
    mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
    mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader) {
    // Scale the quad by the width/height of texture
    Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexWidth), static_cast<float>(mTexHeight), 1.0);
    Matrix4 world = scaleMat * mOwner->GetWorldTransform();
    
    // Since all sprites use the same shader/vertices,
    // the game first sets them active before any sprite draws
    
    // Set world transform
    shader->SetMatrixUniform("uWorldTransform", world);
    
    // Set current texture
    mTexture->SetActive();
    
    // Draw quad
    glDrawElements(
        GL_TRIANGLES,       // Type of polygon / primitiv to draw
        6,                  // Number of indices
        GL_UNSIGNED_INT,    // Type of each index
        nullptr);            // Usually nullptr
}

void SpriteComponent::SetTexture(Texture *texture) {
    mTexture = texture;
    // Set width/height
    mTexWidth = texture->GetWidth();
    mTexHeight = texture->GetHeight();
}