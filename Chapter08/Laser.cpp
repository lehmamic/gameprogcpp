//
//  Laser.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#include "Laser.h"
#include "Game.h"
#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"

Laser::Laser(Game* game)
    :Actor(game)
    ,mDeathTimer(1.0f) {
    // Create a sprite component
    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Laser.png"));

    // Create a move component, and set a forward speed
    MoveComponent* mc = new MoveComponent(this);
    mc->SetForwardSpeed(800.0f);

    // Create a circle component (for collision)
    mCircle = new CircleComponent(this);
    mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime) {
    // If we run out of time, laser is dead
    mDeathTimer -= deltaTime;
    if (mDeathTimer <= 0.0f) {
        SetState(EDead);
    }
    else {
        // Do you intersect with an asteroid?
        for (auto ast : GetGame()->GetAsteroids()) {
            if (Intersect(*mCircle, *(ast->GetCircle()))) {
                // If this laser intersects with an asteroid,
                // set ourselves an the asteroid to dead
                SetState(EDead);
                ast->SetState(EDead);
                break;
            }
        }
    }
}
