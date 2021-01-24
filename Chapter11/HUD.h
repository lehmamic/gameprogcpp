//
//  HUD.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 23.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef HUD_hpp
#define HUD_hpp

#include "UIScreen.h"

class HUD : public UIScreen
{
public:
    // (Lower draw order corresponds with further back)
    HUD(class Game* game);
    ~HUD();
    
    void Update(float deltaTime) override;
    void Draw(class Shader* shader) override;
    
    void AddTargetComponent(class TargetComponent* tc);
    void RemoveTargetComponent(class TargetComponent* tc);
    
protected:
    void UpdateCrosshair(float deltaTime);

    // Textures for crosshair
    class Texture* mCrosshair;
    class Texture* mCrosshairEnemy;
    
    // All the target components in the game
    std::vector<class TargetComponent*> mTargetComps;
    
    // Whether crosshair targets an enemy
    bool mTargetEnemy;
};

#endif /* HUD_hpp */