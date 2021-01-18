//
//  MeshComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 15.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef MeshComponent_hpp
#define MeshComponent_hpp

#include "Component.h"
#include <cstddef>

class MeshComponent : public Component
{
public:
    MeshComponent(class Actor* owner);
    ~MeshComponent();
    
    // Draw this mesh component with provided shader
    virtual void Draw(class Shader* shader);
    
    // Set the mesh/texture index used by mesh component
    virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
    void SetTextureIndex(size_t index) { mTextureIndex = index; }
    
protected:
    class Mesh* mMesh;
    size_t mTextureIndex;
};

#endif /* MeshComponent_hpp */
