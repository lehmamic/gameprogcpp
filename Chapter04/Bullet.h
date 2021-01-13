//
//  Bullet.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 13.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include "Actor.h"

class Bullet : public Actor {
public:
    Bullet(class Game* game);
    
    void UpdateActor(float deltaTime) override;
    
private:
    class CircleComponent* mCircle;
    float mLiveTime;
};


#endif /* Bullet_hpp */
