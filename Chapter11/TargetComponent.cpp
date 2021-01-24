//
//  TargetComponent.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 24.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "TargetComponent.h"
#include "Actor.h"
#include "Game.h"
#include "HUD.h"

TargetComponent::TargetComponent(Actor * owner)
    :Component(owner)
{
    mOwner->GetGame()->GetHUD()->AddTargetComponent(this);
}

TargetComponent::~TargetComponent()
{
    mOwner->GetGame()->GetHUD()->RemoveTargetComponent(this);
}
