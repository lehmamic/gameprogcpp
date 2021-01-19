//
//  OrbiterCamera.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 19.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef OrbiterCamera_hpp
#define OrbiterCamera_hpp

#include "CameraComponent.h"

class OrbitCamera : public CameraComponent
{
public:
    OrbitCamera(class Actor* owner);

    void Update(float deltaTime) override;

    float GetPitchSpeed() const { return mPitchSpeed; }
    float GetYawSpeed() const { return mYawSpeed; }

    void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
    void SetYawSpeed(float speed) { mYawSpeed = speed; }
    
private:
    // Offset from target
    Vector3 mOffset;
    
    // Up vector of camera
    Vector3 mUp;
    
    // Rotation/sec speed of pitch
    float mPitchSpeed;
    
    // Rotation/sec speed of yaw
    float mYawSpeed;
};

#endif /* OrbiterCamera_hpp */
