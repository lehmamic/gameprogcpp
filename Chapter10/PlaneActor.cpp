//
//  PlaneActor.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 15.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "PlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

PlaneActor::PlaneActor(Game* game)
    :Actor(game)
{
    SetScale(10.0f);
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh");
    mc->SetMesh(mesh);
    
    // Add collision box
    BoxComponent* bc = new BoxComponent(this);
    bc->SetObjectBox(mesh->GetBox());
    
    game->AddPlane(this);
}

PlaneActor::~PlaneActor()
{
    GetGame()->RemovePlane(this);
}
