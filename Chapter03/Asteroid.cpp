//
//  Asteroid.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#include "Asteroid.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Math.h"
#include "Random.h"
#include <vector>

Asteroid::Asteroid(Game* game)
    :Actor(game) {
    // Initialize to random position/orientation
    Vector2 randPos = Random::GetVector(
        Vector2::Zero,
        Vector2(1024.0f, 768.0f));
    SetPosition(randPos);
        
    SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));
    
    // Create a sprite component, and set texture
    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));
    
    // Create a move component, and set a forward speed
    MoveComponent* mc = new MoveComponent(this);
    mc->SetForwardSpeed(150.0f);
        
    // Add to mAsteroids in game
    game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
    GetGame()->RemoveAsteroid(this);
}
