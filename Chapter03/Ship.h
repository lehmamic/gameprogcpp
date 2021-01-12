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
    float GetRightSpeed() const { return mRightSpeed; }
    float GetDownSpeed() const { return mDownSpeed; }
    
private:
    float mRightSpeed;
    float mDownSpeed;
};

#endif /* Ship_hpp */
