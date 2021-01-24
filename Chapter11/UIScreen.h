//
//  UIScreen.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 23.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef UIScreen_hpp
#define UIScreen_hpp

#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

class UIScreen
{
public:
    UIScreen(class Game* game);
    virtual ~UIScreen();
    
    // UIScreen subclasses can override these
    virtual void Update(float deltaTime);
    virtual void Draw(class Shader* shader);
    virtual void ProcessInput(const uint8_t* keys);
    virtual void HandleKeyPress(int key);
    
    // Tracks if the UI is active or closing
    enum UIState
    {
        EActive,
        EClosing,
    };
    
    // Set state to closing
    void Close();
    
    // Get state of UI screen
    UIState GetState() const { return mState; }
    
    // Change the title text
    void SetTitle(const std::string& text, const Vector3& color = Color::White, int pointSize = 40);
    
protected:
    // Helper to draw a texture
    void DrawTexture(class Shader* shader, class Texture* texture, const Vector2& offset = Vector2::Zero, float scale = 1.0f);
    
    class Game* mGame;
    
    class Font* mFont;
    class Texture* mTitle;
    
    // Configure positions
    Vector2 mTitlePos;
    
    // State
    UIState mState;
};

#endif /* UIScreen_hpp */
