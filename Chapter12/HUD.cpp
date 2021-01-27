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
#include "FollowActor.h"

HUD::HUD(Game* game)
    :UIScreen(game)
    ,mRadarRange(2000.0f)
    ,mRadarRadius(92.0f)
    ,mTargetEnemy(false)
{
    Renderer* r = mGame->GetRenderer();
    mHealthBar = r->GetTexture("Assets/HealthBar.png");
    mRadar = r->GetTexture("Assets/Radar.png");
    mCrosshair = r->GetTexture("Assets/Crosshair.png");
    mCrosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
    mBlipTex = r->GetTexture("Assets/Blip.png");
    mRadarArrow = r->GetTexture("Assets/RadarArrow.png");
}

HUD::~HUD()
{
}

void HUD::Update(float deltaTime)
{
    UIScreen::Update(deltaTime);
    
    UpdateCrosshair(deltaTime);
    UpdateRadar(deltaTime);
}

void HUD::Draw(class Shader* shader)
{
    // Crosshair
    Texture* cross = mTargetEnemy ? mCrosshairEnemy : mCrosshair;
    DrawTexture(shader, cross, Vector2::Zero, 2.0f);
    
    // Radar
    const Vector2 cRadarPos(-390.0f, 275.0f);
    DrawTexture(shader, mRadar, cRadarPos, 1.0f);
    
    // Blips
    for (Vector2& blip : mBlips)
    {
        DrawTexture(shader, mBlipTex, cRadarPos + blip, 1.0f);
    }
    
    // Radar arrow
    DrawTexture(shader, mRadarArrow, cRadarPos);
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

void HUD::UpdateRadar(float deltaTime)
{
    // Clear blip positions from last frame
    mBlips.clear();
    
    // Convert player position to radar coordinates (x forward, z up)
    Vector3 playerPos = mGame->GetPlayer()->GetPosition();
    Vector2 playerPos2D(playerPos.y, playerPos.x);
    
    // Ditto for player forward
    Vector3 playerForward = mGame->GetPlayer()->GetForward();
    Vector2 playerForward2D(playerForward.x, playerForward.y);
    
    // Use atan2 to get rotation of radar
    float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
    
    // Make a 2D rotation matrix
    Matrix3 rotMat = Matrix3::CreateRotation(angle);
    
    // Get position of blips
    for (auto tc : mTargetComps)
    {
        Vector3 targetPos = tc->GetOwner()->GetPosition();
        Vector2 actorPos2D(targetPos.y, targetPos.x);
        
        // Calculate vector between player and target
        Vector2 playerTotarget = actorPos2D - playerPos2D;
        
        // See if within range
        if (playerTotarget.LengthSq() <= (mRadarRange * mRadarRange))
        {
            // Convert playertotarget into an offset from the center of the on-screen
            Vector2 blipPos = playerTotarget;
            blipPos *= mRadarRadius / mRadarRange;
            
            // Rotate blipPos
            blipPos = Vector2::Transform(blipPos, rotMat);
            mBlips.emplace_back(blipPos);
        }
    }
}
