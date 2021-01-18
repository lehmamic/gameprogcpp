//
// Created by Michael Lehmann on 10.01.21.
//

#include "Game.h"
#include <GL/glew.h>
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Random.h"
#include "InputSystem.h"

Game::Game()
        : mWindow(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mSpriteShader(nullptr)
        ,mUpdatingActors(false) { }

bool Game::Initialize() {
    // Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    
    // Set OpenGL window's attribute (use prior to creating the window)
    // Returns 0 if successful, othewise a negative value
    // use the core OpenGL profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    // Specify version 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    // Request a color buffer with 8-bits per RGBA channel
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    
    // Enable double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // Force OpenGL to use hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // Create an SDL Window
    // See: https://discourse.libsdl.org/t/sdl2-cant-see-the-window/25835
    mWindow = SDL_CreateWindow(
            "Game Programming in C++ (Chapter 5)", // Window title
            100,    // Top left x-coordinate of window
            100,    // Top left y-coordinate of window
            1024,   // Width of window
            768,    // Height of window
            SDL_WINDOW_OPENGL);   // Flags (0 for no flags set)

    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    
    // Initialize input system
    mInputSystem = new InputSystem();
    if (!mInputSystem->Initialize())
    {
        SDL_Log("Failed to initialize input system");
        return false;
    }
    
    // Create an OpenGL context
    mContext = SDL_GL_CreateContext(mWindow);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        SDL_Log("Failed to initialize GLEW.");
        return false;
    }
    
    // On some platforms, GLEW will emit a benign error code,
    // so clear it
    glGetError();
    
    // Make sure we can create/compile shaders
    if (!LoadShaders())
    {
        SDL_Log("Failed to load shaders.");
        return false;
    }
    
    // Create quad for drawing sprites
    CreateSpriteVerts();
    
    LoadData();

    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::Shutdown() {
    UnloadData();
    
    mInputSystem->Shutdown();
    delete mInputSystem;
    
    delete mSpriteVerts;
    mSpriteShader->Unload();
    delete mSpriteShader;
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::AddActor(class Actor *actor) {
    // If updating actors, need to add to pending
    if (mUpdatingActors) {
        mPendingActors.emplace_back(actor);
    } else {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(class Actor *actor) {
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

void Game::AddSprite(class SpriteComponent *sprite) {
    // Find the insertion point in the sorted vector
    // (The first element with a higher draw order than me)
    int myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
    for( ; iter != mSprites.end(); ++iter) {
        if (myDrawOrder < (*iter)->GetDrawOrder()) {
            break;
        }
    }
    
    // inserts element before position iterator
    mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
    // (We can't swap because it ruins ordering)
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

Texture* Game::GetTexture(const std::string& fileName)
{
    Texture* tex = nullptr;
    
    // Is the texture already in the map?
    auto iter = mTextures.find(fileName);
    if (iter != mTextures.end())
    {
        tex = iter->second;
    }
    else
    {
        // Load from file
        tex = new Texture();
        if (tex->Load(fileName))
        {
            mTextures.emplace(fileName, tex);
        }
        else
        {
            delete tex;
            tex = nullptr;
        }
    }
    
    return tex;
}

void Game::AddAsteroid(Asteroid* ast)
{
    mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
    auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
    if (iter != mAsteroids.end())
    {
        mAsteroids.erase(iter);
    }
}

void Game::ProcessInput()
{
    mInputSystem->PrepareForUpdate();
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mIsRunning = false;
                break;
                
            case SDL_MOUSEWHEEL:
                mInputSystem->ProcessEvent(event);
                break;
        }
    }
    
    mInputSystem->Update();
    const InputState& state = mInputSystem->GetState();
    
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == EReleased)
    {
        mIsRunning = false;
    }
    
    mUpdatingActors = true;
    for (auto actor : mActors) {
        actor->ProcessInput(state);
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
    // Set the clear color to gray
    glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
    
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw all sprite components
    // Enable alpha blending on the color buffer
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set sprite shader and vertex array objects active
    mSpriteShader->SetActive();
    mSpriteVerts->SetActive();
    
    // Draw all sprites
    for (auto sprite : mSprites) {
        sprite->Draw(mSpriteShader);
    }
    
    // Swap the buffers, which also displays the scene
    SDL_GL_SwapWindow(mWindow);
}

bool Game::LoadShaders() {
    mSpriteShader = new Shader();
    
    if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) {
        return false;
    }
    
    mSpriteShader->SetActive();
    
    // Set the view-projection matrix
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
    mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
    
    return true;
}

void Game::CreateSpriteVerts()
{
    float vertices[] = {
        -0.5f,  0.5f, 0.f, 0.f, 0.f, // top left
         0.5f,  0.5f, 0.f, 1.f, 0.f, // top right
         0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right
        -0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Game::LoadData()
{
    // Create player's ship
    mShip = new Ship(this);
    mShip->SetRotation(Math::PiOver2);

    // Create asteroids
    const int numAsteriods = 20;
    for (int i = 0; i < numAsteriods; i++)
    {
        new Asteroid(this);
    }
}

void Game::UnloadData()
{
    // Delete actors
    // Because ~Actor calls RemoveActor, have to use a different style loop
    while (!mActors.empty())
    {
        delete mActors.back();
    }

    // Destroy textures
    for (auto i : mTextures)
    {
        i.second->Unload();
        delete i.second;
    }
    mTextures.clear();
}
