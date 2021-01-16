//
//  Laser.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef Laser_hpp
#define Laser_hpp

#include "Actor.h"
class Laser : public Actor
{
public:
    Laser(class Game* game);

    void UpdateActor(float deltaTime) override;
private:
    class CircleComponent* mCircle;
    float mDeathTimer;
};


#endif /* Laser_hpp */
