//
//  HUD.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 23.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "HUD.h"
#include "Game.h"

HUD::HUD(Game* game)
    :UIScreen(game)
    //,mRadarRange(2000.0f)
    //,mRadarRadius(92.0f)
    //,mTargetEnemy(false)
{
    //Renderer* r = mGame->GetRenderer();
    //mHealthBar = r->GetTexture("Assets/HealthBar.png");
    //mRadar = r->GetTexture("Assets/Radar.png");
    //mCrosshair = r->GetTexture("Assets/Crosshair.png");
    //mCrosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
    //mBlipTex = r->GetTexture("Assets/Blip.png");
    //mRadarArrow = r->GetTexture("Assets/RadarArrow.png");
}

HUD::~HUD()
{
}
