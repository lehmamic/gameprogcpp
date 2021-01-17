//
//  InputSystem.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 16.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef InputSystem_hpp
#define InputSystem_hpp

#include <SDL/SDL_scancode.h>
#include <SDL/SDL_gamecontroller.h>
#include <SDL/SDL_mouse.h>
#include "Math.h"

enum ButtonState
{
    ENone,
    EPressed,
    EReleased,
    EHeld,
};

class KeyboardState
{
public:
    // Friend so InputSystem can easily update it
    friend class InputSystem;
    
    // Get just the boolean true/false value of key
    bool GetKeyValue(SDL_Scancode keyCode) const;
    
    // Get a state based on current and previos frame
    ButtonState GetKeyState(SDL_Scancode keyCode) const;
    
private:
    // Current state
    const Uint8* mCurrState;
    
    // State prievious frame
    Uint8 mPrevState[SDL_NUM_SCANCODES];
};

class MouseState
{
public:
    friend class InputSystem;
    
    // For mouse position
    const Vector2& GetPosition() const { return mMousePos; }
    const Vector2& GetScrollWheel() const { return mScrollWheel; }
    bool IsRelative() const { return mIsRelative; }
    
    // For buttons
    bool GetButtonValue(int button) const;
    ButtonState GetButtonState(int button) const;
    
private:
    // Stores mouse position
    Vector2 mMousePos;
    
    // Motion of scroll wheel
    Vector2 mScrollWheel;
    
    // Store button data
    Uint32 mCurrButtons;
    Uint32 mPrevButtons;
    
    // Are we in relative mouse mode
    bool mIsRelative;
};

class ControllerState
{
public:
    friend class InputSystem;
    
    // For buttons
    bool GetButtonValue(SDL_GameControllerButton button) const;
    ButtonState GetButtonState(SDL_GameControllerButton button) const;
    
    const Vector2& GetLeftStick() const { return mLeftStick; }
    const Vector2& GetRightStick() const { return mRightStick; }
    float GetLeftTrigger() const { return mLeftTrigger; }
    float GetRightTrigger() const { return mRightTrigger; }
    
    bool GetIsConnected() const { return mIsConnected; }
private:
    // Current/previous buttons
    Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX];
    Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX];
    
    // Left/right sticks
    Vector2 mLeftStick;
    Vector2 mRightStick;
    
    // Left/right trigger
    float mLeftTrigger;
    float mRightTrigger;
    
    // is this controlled connected
    bool mIsConnected;
};

// Wrapper that contains current state of input
struct InputState
{
    KeyboardState Keyboard;
    MouseState Mouse;
    ControllerState Controller;
};

class InputSystem
{
public:
    bool Initialize();
    void Shutdown();
    
    // Called right before SDL_PollEvents
    void PrepareForUpdate();
    
    // Called after SDL_PollEvents loop
    void Update();
    
    // Called to process an SDL event in input system
    void ProcessEvent(union SDL_Event& event);
    
    const InputState& GetState() const { return mState; }
    
    void SetRelativeMouseMode(bool value);
    
private:
    float Filter1D(int input);
    Vector2 Filter2D(int inputX, int inputY);
    InputState mState;
    SDL_GameController* mController;
};

#endif /* InputSystem_hpp */
