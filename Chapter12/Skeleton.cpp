//
//  Skeleton.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 26.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "Skeleton.h"

void Skeleton::ComputeGlobalInvBindPose()
{
    // Resize to number of bones, which automatically fills identity
    mGlobalInvBindPoses.resize(GetNumBones());
    
    // Step 1: Compute global bind pose for each bone
    
    // The global bind pose for root is just the local bind pose
    mGlobalInvBindPoses[0] = mBones[0].mLocalBindPose.ToMatrix();

    // Each remaining bone's global bind pose is its local pose
    // multiplied by the parent's global bind pose
    for (size_t i = 1; i < mGlobalInvBindPoses.size(); i++)
    {
        Matrix4 localMat = mBones[i].mLocalBindPose.ToMatrix();
        mGlobalInvBindPoses[i] = localMat * mGlobalInvBindPoses[mBones[i].mParent];
    }

    // Step 2: Invert
    for (size_t i = 0; i < mGlobalInvBindPoses.size(); i++)
    {
        mGlobalInvBindPoses[i].Invert();
    }
}
