//
//  CameraComponent.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 18.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "CameraComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "AudioSystem.h"

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
    :Component(owner, updateOrder)
{
}

void CameraComponent::SetViewMatrix(const Matrix4 &view)
{
    // Pass view matrix to render and audio system
    Game* game = mOwner->GetGame();
    game->GetRenderer()->SetViewMatrix(view);
    game->GetAudioSystem()->SetListener(view);
}
