//
//  FollowActor.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 18.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "FollowActor.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "FollowCamera.h"
#include "MoveComponent.h"

FollowActor::FollowActor(Game* game)
    :Actor(game)
{
    mMeshComp = new SkeletalMeshComponent(this);
    mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/CatWarrior.gpmesh"));
    // mMeshComp->SetSkeleton(game->GetSkeleton("Assets/CatWarrior.gpskel"));
    // mMeshComp->PlayAnimation(game->GetAnimation("Assets/CatActionIdle.gpanim"));
    SetPosition(Vector3(0.0f, 0.0f, -100.0f));

    mMoveComp = new MoveComponent(this);
    mCameraComp = new FollowCamera(this);
    mCameraComp->SnapToIdeal();
}

void FollowActor::ActorInput(const uint8_t* keys)
{
    float forwardSpeed = 0.0f;
    float angularSpeed = 0.0f;
    // wasd movement
    if (keys[SDL_SCANCODE_W])
    {
        forwardSpeed += 400.0f;
    }
    if (keys[SDL_SCANCODE_S])
    {
        forwardSpeed -= 400.0f;
    }
    if (keys[SDL_SCANCODE_A])
    {
        angularSpeed -= Math::Pi;
    }
    if (keys[SDL_SCANCODE_D])
    {
        angularSpeed += Math::Pi;
    }

    mMoveComp->SetForwardSpeed(forwardSpeed);
    mMoveComp->SetAngularSpeed(angularSpeed);

    // Adjust horizontal distance of camera based on speed
    if (!Math::NearZero(forwardSpeed))
    {
        mCameraComp->SetHorzDist(500.0f);
    }
    else
    {
        mCameraComp->SetHorzDist(350.0f);
    }
}

void FollowActor::SetVisible(bool visible)
{
    mMeshComp->SetVisible(visible);
}
