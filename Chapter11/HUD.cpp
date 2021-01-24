//
//  HUD.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 23.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "HUD.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"
#include "PhysWorld.h"
#include "Collision.h"
#include "TargetComponent.h"

HUD::HUD(Game* game)
    :UIScreen(game)
    //,mRadarRange(2000.0f)
    //,mRadarRadius(92.0f)
    ,mTargetEnemy(false)
{
    Renderer* r = mGame->GetRenderer();
    //mHealthBar = r->GetTexture("Assets/HealthBar.png");
    //mRadar = r->GetTexture("Assets/Radar.png");
    mCrosshair = r->GetTexture("Assets/Crosshair.png");
    mCrosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
    //mBlipTex = r->GetTexture("Assets/Blip.png");
    //mRadarArrow = r->GetTexture("Assets/RadarArrow.png");
}

HUD::~HUD()
{
}

void HUD::Update(float deltaTime)
{
    UIScreen::Update(deltaTime);
    
    UpdateCrosshair(deltaTime);
}

void HUD::Draw(class Shader* shader)
{
    // Crosshair
    Texture* cross = mTargetEnemy ? mCrosshairEnemy : mCrosshair;
    DrawTexture(shader, cross, Vector2::Zero, 2.0f);
}

void HUD::AddTargetComponent(TargetComponent* tc)
{
    mTargetComps.emplace_back(tc);
}

void HUD::RemoveTargetComponent(TargetComponent* tc)
{
    auto iter = std::find(mTargetComps.begin(), mTargetComps.end(), tc);
    mTargetComps.erase(iter);
}

void HUD::UpdateCrosshair(float deltaTime)
{
    // Reset to regular cursor
    mTargetEnemy = false;
    
    // Make a line segment
    const float cAimDist = 5000.0f;
    Vector3 start, dir;
    mGame->GetRenderer()->GetScreenDirection(start, dir);
    LineSegment l(start, start + dir * cAimDist);
    
    // Segment cast
    PhysWorld::CollisionInfo info;
    if (mGame->GetPhysWorld()->SegmentCast(l, info))
    {
        // Is this a target?
        for (auto tc : mTargetComps)
        {
            if (tc->GetOwner() == info.mActor)
            {
                mTargetEnemy = true;
                break;
            }
        }
    }
}
