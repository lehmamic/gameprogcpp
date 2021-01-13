//
//  Tower.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 13.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef Tower_hpp
#define Tower_hpp

#include "Actor.h"

class Tower : public Actor
{
public:
    Tower(class Game* game);
    
    void UpdateActor(float deltaTime) override;
    
private:
    class MoveComponent* mMove;
    float mNextAttack;
    const float AttackTime = 2.5f;
    const float AttackRange = 100.0f;
};

#endif /* Tower_hpp */
