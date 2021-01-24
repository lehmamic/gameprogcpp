//
// Created by Michael Lehmann on 10.01.21.
//

#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "AudioSystem.h"
#include "PhysWorld.h"
#include "Actor.h"
#include "UIScreen.h"
#include "HUD.h"
#include "PauseMenu.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "FPSActor.h"
#include "PlaneActor.h"
#include "TargetActor.h"
#include "BallActor.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "Font.h"

Game::Game()
        :mRenderer(nullptr)
        ,mAudioSystem(nullptr)
        ,mPhysWorld(nullptr)
        ,mGameState(EGameplay)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mUpdatingActors(false) { }

bool Game::Initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    
    // Create the renderer
    mRenderer = new Renderer(this);
    if (!mRenderer->Initialize(1024.0f, 768.0f))
    {
        SDL_Log("Failed to initialize renderer");
        delete mRenderer;
        mRenderer = nullptr;
        return false;
    }
    
    // Create the audio system
    mAudioSystem = new AudioSystem(this);
    if (!mAudioSystem->Initialize())
    {
        SDL_Log("Failed to initialize audio system");
        mAudioSystem->Shutdown();
        delete mAudioSystem;
        mAudioSystem = nullptr;
        return false;
    }
    
    // Create the physics world
    mPhysWorld = new PhysWorld(this);
    
    // Initialize SDL_ttf
    if (TTF_Init() != 0)
    {
        SDL_Log("Failed to initialize SDL_ttf");
        return false;
    }

    LoadData();
    
    mTicksCount = SDL_GetTicks();
    
    return true;
}

void Game::RunLoop() {
    while (mGameState != EQuit)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::AddPlane(PlaneActor* plane)
{
    mPlanes.emplace_back(plane);
}

void Game::RemovePlane(PlaneActor* plane)
{
    auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
    mPlanes.erase(iter);
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mGameState = EQuit;
                break;
                
            // This fires when a key's initially pressed
            case SDL_KEYDOWN:
                if (!event.key.repeat)
                {
                    if (mGameState == EGameplay)
                    {
                        HandleKeyPress(event.key.keysym.sym);
                    }
                    else if (!mUIStack.empty())
                    {
                        mUIStack.back()->HandleKeyPress(event.key.keysym.sym);
                    }
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (mGameState == EGameplay)
                {
                    HandleKeyPress(event.button.button);
                }
                else if (!mUIStack.empty())
                {
                    mUIStack.back()->HandleKeyPress(event.button.button);
                }
                break;
                
            default:
                break;
        }
    }

    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    if (mGameState == EGameplay)
    {
        mUpdatingActors = true;
        for (auto actor : mActors)
        {
            if (actor->GetState() == Actor::EActive)
            {
                actor->ProcessInput(keyState);
            }
        }
        mUpdatingActors = false;
    }
    else if(!mUIStack.empty())
    {
        mUIStack.back()->ProcessInput(keyState);
    }
}

void Game::HandleKeyPress(int key)
{
    switch (key)
    {
        case SDLK_ESCAPE:
            // Create pause menu
            new PauseMenu(this);
            break;
            
        case '-':
        {
            // Reduce master volume
            float volume = mAudioSystem->GetBusVolume("bus:/");
            volume = Math::Max(0.0f, volume - 0.1f);
            mAudioSystem->SetBusVolume("bus:/", volume);
            break;
        }
        case '=':
        {
            // Increase master volume
            float volume = mAudioSystem->GetBusVolume("bus:/");
            volume = Math::Min(1.0f, volume + 0.1f);
            mAudioSystem->SetBusVolume("bus:/", volume);
            break;
        }
            
        case SDL_BUTTON_LEFT:
        {
            // Fire weapon
            mFPSActor->Shoot();
            break;
        }

        default:
        {
            break;
        }
    }
}

void Game::UpdateGame() {
    // Compute delta time (as in Chapter 1)
    // Wait until 16ms has elapsed since last frame
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    // Delta time is the difference in ticks from last frame
    // (converted to seconds)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    // Clamp maximum delta time value
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    // Update tick counts (for next frame)
    mTicksCount = SDL_GetTicks();

    if (mGameState == EGameplay)
    {
        // Update all actors
        mUpdatingActors = true;
        for (auto actor : mActors)
        {
            actor->Update(deltaTime);
        }
        mUpdatingActors = false;

        // Move any pending actors to mActors
        for (auto pending : mPendingActors)
        {
            pending->ComputeWorldTransform();
            mActors.emplace_back(pending);
        }
        mPendingActors.clear();

        // Add any dead actors to a temp vector
        std::vector<Actor*> deadActors;
        for (auto actor : mActors)
        {
            if (actor->GetState() == Actor::EDead)
            {
                deadActors.emplace_back(actor);
            }
        }

        // Delete dead actors (which removes them from mActors
        for(auto actor : deadActors)
        {
            delete actor;
        }
    }
    
    // Update audio system
    mAudioSystem->Update(deltaTime);
    
    // Update UI screens
    for (auto ui : mUIStack)
    {
        if (ui->GetState() == UIScreen::EActive)
        {
            ui->Update(deltaTime);
        }
    }
    
    // Delete any UIScreens that are closed
    auto iter = mUIStack.begin();
    while (iter != mUIStack.end())
    {
        if ((*iter)->GetState() == UIScreen::EClosing)
        {
            delete *iter;
            iter = mUIStack.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void Game::GenerateOutput()
{
    mRenderer->Draw();
}

void Game::LoadData()
{
    // Create actors
    Actor* a = nullptr;
    Quaternion q;
    //MeshComponent* mc = nullptr;

    // Setup floor
    const float start = -1250.0f;
    const float size = 250.0f;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            a = new PlaneActor(this);
            a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
        }
    }

    // Left/right walls
    q = Quaternion(Vector3::UnitX, Math::PiOver2);
    for (int i = 0; i < 10; i++)
    {
        a = new PlaneActor(this);
        a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
        a->SetRotation(q);
        
        a = new PlaneActor(this);
        a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
        a->SetRotation(q);
    }

    q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
    // Forward/back walls
    for (int i = 0; i < 10; i++)
    {
        a = new PlaneActor(this);
        a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
        a->SetRotation(q);

        a = new PlaneActor(this);
        a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
        a->SetRotation(q);
    }

    // Setup lights
    mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
    DirectionalLight& dir = mRenderer->GetDirectionalLight();
    dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
    dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
    dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

    // UI elements
    mHUD = new HUD(this);

    // Start music
    mMusicEvent = mAudioSystem->PlayEvent("event:/Music");

    // Enable relative mouse mode for camera look
    SDL_SetRelativeMouseMode(SDL_TRUE);
    // Make an initial call to get relative to clear out
    SDL_GetRelativeMouseState(nullptr, nullptr);

    // Different camera actors
    mFPSActor = new FPSActor(this);

    // Create target actors
    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, 0.0f, 100.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, 0.0f, 400.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, -500.0f, 200.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, 500.0f, 200.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(0.0f, -1450.0f, 200.0f));
    a->SetRotation(Quaternion(Vector3::UnitZ, Math::PiOver2));
    a = new TargetActor(this);
    a->SetPosition(Vector3(0.0f, 1450.0f, 200.0f));
    a->SetRotation(Quaternion(Vector3::UnitZ, -Math::PiOver2));
}

void Game::UnloadData()
{
    // Delete actors
    // Because ~Actor calls RemoveActor, have to use a different style loop
    while (!mActors.empty())
    {
        delete mActors.back();
    }
    
    // Clear the UI stack
    while (!mUIStack.empty())
    {
        delete mUIStack.back();
        mUIStack.pop_back();
    }
    
    if (mRenderer)
    {
        mRenderer->UnloadData();
    }
}

void Game::Shutdown() {
    UnloadData();
    TTF_Quit();
    delete mPhysWorld;
    
    if (mRenderer)
    {
        mRenderer->Shutdown();
    }
    
    if (mAudioSystem)
    {
        mAudioSystem->Shutdown();
    }
    
    SDL_Quit();
}

void Game::AddActor(class Actor *actor)
{
    // If updating actors, need to add to pending
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(class Actor *actor)
{
    // Is it in pending actors?
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    // Is it in actors?
    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::PushUI(UIScreen* screen)
{
    mUIStack.emplace_back(screen);
}

Font* Game::GetFont(const std::string& fileName)
{
    auto iter = mFonts.find(fileName);
    if (iter != mFonts.end())
    {
        return iter->second;
    }
    else
    {
        Font* font = new Font(this);
        if (font->Load(fileName))
        {
            mFonts.emplace(fileName, font);
        }
        else
        {
            font->Unload();
            delete font;
            font = nullptr;
        }
        return font;
    }
}
