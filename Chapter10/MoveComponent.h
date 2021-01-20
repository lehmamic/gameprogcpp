//
//  MoveComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef MoveComponent_hpp
#define MoveComponent_hpp

#include "Component.h"

class MoveComponent : public Component {
public:
    // Lower update order to update first
    MoveComponent(class Actor* owner, int updateOrder = 10);
    
    void Update(float deltaTime) override;
    
    float GetAngularSpeed() const { return mAngularSpeed; }
    float GetForwardSpeed() const { return mForwardSpeed; }
    void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
    void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
    void SetStrafeSpeed(float speed) { mStrafeSpeed = speed; }
    
private:
    // Controls rotation (radians/second)
    float mAngularSpeed;
    
    // Controls forward movement
    float mForwardSpeed;
    
    // Controls strafe movement
    float mStrafeSpeed;
    
};

#endif /* MoveComponent_hpp */
