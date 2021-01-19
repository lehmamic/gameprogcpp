//
//  FollowCamera.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 18.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef FollowCamera_hpp
#define FollowCamera_hpp

#include "CameraComponent.h"

class FollowCamera : public CameraComponent
{
public:
    FollowCamera(class Actor* owner);

    void Update(float deltaTime) override;
    
    void SnapToIdeal();

    void SetHorzDist(float dist) { mHorzDist = dist; }
    void SetVertDist(float dist) { mVertDist = dist; }
    void SetTargetDist(float dist) { mTargetDist = dist; }
    void SetSpringConstant(float spring) { mSpringConstant = spring; }
private:
    Vector3 ComputeCameraPos() const;

    // Actual position of camera
    Vector3 mActualPos;
    
    // Velocity of actual camera
    Vector3 mVelocity;
    
    // Horizontal follow distance
    float mHorzDist;
    
    // Vertical follow distance
    float mVertDist;
    
    // Target distance
    float mTargetDist;
    
    // Spring constant (higher is more stiff)
    float mSpringConstant;
};

#endif /* FollowCamera_hpp */
