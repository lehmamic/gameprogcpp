//
// Created by Michael Lehmann on 10.01.21.
//

#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder)
        :mOwner(owner)
        ,mUpdateOrder(updateOrder)
{
    // Add to actor's vector of components
    mOwner->AddComponent(this);
}

Component::~Component()
{
    mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{
}