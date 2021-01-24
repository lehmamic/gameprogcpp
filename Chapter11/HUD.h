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
};

#endif /* HUD_hpp */
