//
//  PauseMenu.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 24.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "PauseMenu.h"
#include "Game.h"

PauseMenu::PauseMenu(Game* game)
    :UIScreen(game)
{
    mGame->SetState(Game::EPaused);
    SetRelativeMouseMode(false);
    SetTitle("PauseTitle");
    
    AddButton("ResumeButton", [this]() {
        Close();
    });
    
    AddButton("QuitButton", [this]() {
        /*new DialogBox(mGame, "QuitText",
            [this]() {
                mGame->SetState(Game::EQuit);
        });*/
        mGame->SetState(Game::EQuit);
    });
}

PauseMenu::~PauseMenu()
{
    SetRelativeMouseMode(true);
    mGame->SetState(Game::EGameplay);
}

void PauseMenu::HandleKeyPress(int key)
{
    UIScreen::HandleKeyPress(key);
    
    if (key ==SDLK_ESCAPE)
    {
        Close();
    }
}
