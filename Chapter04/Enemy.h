//
//  Enemy.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 13.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef Enemy_hpp
#define Enemy_hpp

#include "Actor.h"

class Enemy : public Actor
{
public:
    Enemy(class Game* game);
    ~Enemy();
    
    void UpdateActor(float deltaTime) override;
    class CircleComponent* GetCircle() { return mCircle; }
    
private:
    class CircleComponent* mCircle;
};


#endif /* Enemy_hpp */
