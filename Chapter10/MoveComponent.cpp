//
//  MoveComponent.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
    :Component(owner, updateOrder)
    ,mAngularSpeed(0.0f)
    ,mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
    if (!Math::NearZero(mAngularSpeed))
    {
        Quaternion rot = mOwner->GetRotation();
        float angle = mAngularSpeed * deltaTime;
        
        // Create quaternion for incremental rotation
        // (Rotate about up axis)
        Quaternion inc(Vector3::UnitZ, angle);
        
        // Concatenate old and new quaternion
        rot = Quaternion::Concatenate(rot, inc);
    
        mOwner->SetRotation(rot);
    }
    
    // Updating position based on forward speed stays the same
    if (!Math::NearZero(mForwardSpeed) || !Math::NearZero(mStrafeSpeed))
    {
        Vector3 pos = mOwner->GetPosition();
        pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
        
        // Update position based on strafe
        pos += mOwner->GetRight() * mStrafeSpeed * deltaTime;
        
        mOwner->SetPosition(pos);
    }
}
