//
//  AIState.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 13.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#include "AIState.h"
#include "AIComponent.h"
#include <SDL/SDL.h>

void AIPatrol::Update(float deltaTime) {
    // Do some other updating
    
    bool dead = true; /* Figure out if I'm dead */
    if (dead) {
        // Tell the ai component to change states
        mOwner->ChangeState("Death");
    }
}

void AIPatrol::OnEnter()
{
    SDL_Log("Entering %s state", GetName());
}

void AIPatrol::OnExit()
{
    SDL_Log("Exiting %s state", GetName());
}

void AIDeath::Update(float deltaTime)
{
    SDL_Log("Updating %s state", GetName());
}

void AIDeath::OnEnter()
{
    SDL_Log("Entering %s state", GetName());
}

void AIDeath::OnExit()
{
    SDL_Log("Exiting %s state", GetName());
}

void AIAttack::Update(float deltaTime)
{
    SDL_Log("Updating %s state", GetName());
}

void AIAttack::OnEnter()
{
    SDL_Log("Entering %s state", GetName());
}

void AIAttack::OnExit()
{
    SDL_Log("Exiting %s state", GetName());
}
