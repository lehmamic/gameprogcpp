//
//  MeshComponent.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 15.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

MeshComponent::MeshComponent(Actor* owner)
    :Component(owner)
    ,mMesh(nullptr)
    ,mTextureIndex(0)
    ,mVisible(true)
{
    mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
    mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(class Shader *shader)
{
    if (mMesh)
    {
        // Set the world transform
        shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
        
        // Set specular power
        shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
        
        // Set the active texture
        Texture* t = mMesh->GetTexture(mTextureIndex);
        if (t)
        {
            t->SetActive();
        }
        
        // Set mesh's vertex array as active
        VertexArray* va = mMesh->GetVertexArray();
        va->SetActive();
        
        // Draw
        glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
    }
}
