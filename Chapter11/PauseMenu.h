//
//  PauseMenu.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 24.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef PauseMenu_hpp
#define PauseMenu_hpp

#include "UIScreen.h"

class PauseMenu : public UIScreen
{
public:
    PauseMenu(class Game* game);
    ~PauseMenu();

    void HandleKeyPress(int key) override;
};

#endif /* PauseMenu_hpp */
