//
// Created by Michael Lehmann on 10.01.21.
//

#include "Game.h"
#include "Renderer.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "Random.h"

Game::Game()
        :mRenderer(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mUpdatingActors(false) { }

bool Game::Initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
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

    mTicksCount = SDL_GetTicks();
    
    LoadData();

    return true;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }
    
    mUpdatingActors = true;
    for (auto actor : mActors) {
        actor->ProcessInput(keyState);
    }
    mUpdatingActors = false;
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

    // Update all actors
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }

    mUpdatingActors = false;

    // Move any pending actors to mActors
    for (auto pending : mPendingActors) {
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

void Game::GenerateOutput()
{
    mRenderer->Draw();
}

void Game::LoadData()
{
}

void Game::UnloadData()
{
    // Delete actors
    // Because ~Actor calls RemoveActor, have to use a different style loop
    while (!mActors.empty())
    {
        delete mActors.back();
    }
}

void Game::Shutdown() {
    UnloadData();
    
    if (mRenderer)
    {
        mRenderer->Shutdown();
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
