//
//  SkeletalMeshComponent.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 27.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "SkeletalMeshComponent.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
    :MeshComponent(owner, true)
    // ,mSkeleton(nullptr)
{
}

void SkeletalMeshComponent::Draw(class Shader* shader)
{
    MeshComponent::Draw(shader);
}
