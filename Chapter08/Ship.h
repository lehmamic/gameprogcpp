//
//  Ship.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef Ship_hpp
#define Ship_hpp

#include "Actor.h"

class Ship : public Actor {
public:
    Ship(class Game* game);
    void UpdateActor(float deltaTime) override;
    void ActorInput(const struct InputState& state) override;
    
private:
    float mLaserCooldown;
};

#endif /* Ship_hpp */
