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
    MeshComponent(class Actor* owner, bool isSkeletal = false);
    ~MeshComponent();
    
    // Draw this mesh component with provided shader
    virtual void Draw(class Shader* shader);
    
    // Set the mesh/texture index used by mesh component
    virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
    void SetTextureIndex(size_t index) { mTextureIndex = index; }
    
    void SetVisible(bool visible) { mVisible = visible; }
    bool GetVisible() const { return mVisible; }

    bool GetIsSkeletal() const { return mIsSkeletal; }
    
protected:
    class Mesh* mMesh;
    size_t mTextureIndex;
    bool mVisible;
    bool mIsSkeletal;
};

#endif /* MeshComponent_hpp */
