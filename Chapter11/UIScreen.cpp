//
//  UIScreen.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 23.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "UIScreen.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Font.h"

UIScreen::UIScreen(Game* game)
    :mGame(game)
    ,mTitle(nullptr)
    // ,mBackground(nullptr)
    ,mTitlePos(0.0f, 300.0f)
    // ,mNextButtonPos(0.0f, 200.0f)
    // ,mBGPos(0.0f, 250.0f)
    ,mState(EActive)
{
    // Add to UI Stack
    mGame->PushUI(this);
    mFont = mGame->GetFont("Assets/Carlito-Regular.ttf");
    // mButtonOn = mGame->GetRenderer()->GetTexture("Assets/ButtonYellow.png");
    // mButtonOff = mGame->GetRenderer()->GetTexture("Assets/ButtonBlue.png");
}

UIScreen::~UIScreen()
{
    if (mTitle)
    {
        mTitle->Unload();
        delete mTitle;
    }

    /*for (auto b : mButtons)
    {
        delete b;
    }
    mButtons.clear();*/
}

void UIScreen::Update(float deltaTime)
{    
}

void UIScreen::Draw(Shader* shader)
{
    // Draw title (if exists)
    if (mTitle)
    {
        DrawTexture(shader, mTitle, mTitlePos);
    }
}

void UIScreen::ProcessInput(const uint8_t* keys)
{
}

void UIScreen::HandleKeyPress(int key)
{
}

void UIScreen::Close()
{
    mState = EClosing;
}

void UIScreen::SetTitle(const std::string& text, const Vector3& color, int pointSize)
{
    // Clear out previous title texture if it exists
    if (mTitle)
    {
        mTitle->Unload();
        delete mTitle;
        mTitle = nullptr;
    }
    
    mTitle = mFont->RenderText(text, color, pointSize);
}

void UIScreen::DrawTexture(class Shader* shader, class Texture* texture, const Vector2& offset, float scale)
{
    // Scale the quad by the width/height of texture
    Matrix4 scaleMat = Matrix4::CreateScale(
        static_cast<float>(texture->GetWidth()) * scale,
        static_cast<float>(texture->GetHeight()) * scale,
        1.0f);
    
    // Translate to position on screen
    Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x, offset.y, 0.0f));
    
    // Set world transform
    Matrix4 world = scaleMat * transMat;
    shader->SetMatrixUniform("uWorldTransform", world);
    
    // Set current texture
    texture->SetActive();
    
    // Draw quad
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
