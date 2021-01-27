//
//  SkeletalMeshComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 27.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef SkeletalMeshComponent_hpp
#define SkeletalMeshComponent_hpp

#include "MeshComponent.h"

class SkeletalMeshComponent: public MeshComponent
{
public:
    SkeletalMeshComponent(class Actor* owner);
    
    // Draw this mesh component
    void Draw(class Shader* shader) override;
};

#endif /* SkeletalMeshComponent_hpp */
