//
//  Ship.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#include "Ship.h"
#include "Game.h"
#include "Laser.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "InputSystem.h"
#include "Math.h"
#include <vector>

Ship::Ship(Game* game)
    :Actor(game)
    ,mLaserCooldown(0.0f)
{
    // Create a sprite component
    SpriteComponent* sc = new SpriteComponent(this, 150);
    sc->SetTexture(game->GetTexture("Assets/Ship.png"));
    
    // Create an input component and set keys/speed
    InputComponent* ic = new InputComponent(this);
    ic->SetForwardKey(SDL_SCANCODE_W);
    ic->SetBackKey(SDL_SCANCODE_S);
    ic->SetClockwiseKey(SDL_SCANCODE_A);
    ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
    ic->SetMaxForwardSpeed(300.0f);
    ic->SetMaxAngularSpeed(Math::TwoPi);
}

void Ship::UpdateActor(float deltaTime) {
    mLaserCooldown -= deltaTime;
}

void Ship::ActorInput(const InputState& state)
{
    // NOTE: The original example uses the controller
    if (state.Keyboard.GetKeyValue(SDL_Scancode(SDL_SCANCODE_SPACE)) && mLaserCooldown <= 0.0f)
    {
        // Create a laser and set its position/rotation to mine
        Laser* laser = new Laser(GetGame());
        laser->SetPosition(GetPosition());
        laser->SetRotation(GetRotation());

        // Reset laser cooldown (half second)
        mLaserCooldown = 0.5f;
    }
}
