//
//  OrbiterCamera.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 19.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "OrbitCamera.h"
#include "Actor.h"
#include "Math.h"

OrbitCamera::OrbitCamera(Actor* owner)
    :CameraComponent(owner)
    ,mOffset(-400.0f, 0.0f, 0.0f)
    ,mUp(Vector3::UnitZ)
    ,mPitchSpeed(0.0f)
    ,mYawSpeed(0.0f)
{
}

void OrbitCamera::Update(float deltaTime)
{
    CameraComponent::Update(deltaTime);
    
    // Create a quaternion for yaw about
    Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);
    
    // Transform offset and up by yaw
    mOffset = Vector3::Transform(mOffset, yaw);
    mUp = Vector3::Transform(mUp, yaw);
    
    // Compute camera forward/right from these vectors
    // Forward owner.position - (owner.position + offset) = -offset
    Vector3 forward = -1.0f * mOffset;
    forward.Normalize();
    
    Vector3 right = Vector3::Cross(mUp, forward);
    right.Normalize();
    
    // Create quaternion for pitch about camera right
    Quaternion pitch(right, mPitchSpeed * deltaTime);
    
    // Transform camera offset and up by pitch
    mOffset = Vector3::Transform(mOffset, pitch);
    mUp = Vector3::Transform(mUp, pitch);
    
    // Compute transform matrix
    Vector3 target = mOwner->GetPosition();
    Vector3 cameraPos = target + mOffset;
    
    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, mUp);
    SetViewMatrix(view);
}
