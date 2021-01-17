//
// Created by Michael Lehmann on 10.01.21.
//

#ifndef CHAPTER02_GAME_H
#define CHAPTER02_GAME_H

#include "SDL/SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Game {
public:
    Game();

    // Initialize the game
    bool Initialize();

    // Runs the game loop until the game is over
    void RunLoop();

    // Shutdown the game
    void Shutdown();

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    
    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);
    
    class Texture* GetTexture(const std::string& fileName);
    
    // Game-specific (add/remove asteroid)
    void AddAsteroid(class Asteroid* ast);
    void RemoveAsteroid(class Asteroid* ast);
    std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }

private:
    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    bool LoadShaders();
    void CreateSpriteVerts();
    void LoadData();
    void UnloadData();

    // Window created by SDL
    SDL_Window* mWindow;
    // OpenGL Context
    SDL_GLContext mContext;
    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;
    
    // Map of textures loaded
    std::unordered_map<std::string, class Texture*> mTextures;

    // All the actors in the game
    std::vector<class Actor*> mActors;
    // Any pending actors
    std::vector<class Actor*> mPendingActors;
    
    class InputSystem* mInputSystem;
    
    // All the sprite components drawn
    std::vector<class SpriteComponent*> mSprites;
    
    // Sprite shader
    class Shader* mSpriteShader;
    // Sprite vertex array
    class VertexArray* mSpriteVerts;

    // Track if we're updating actors right now
    bool mUpdatingActors;
    
    // Game-specific
    class Ship* mShip; // Player's ship
    std::vector<class Asteroid*> mAsteroids;
};


#endif //CHAPTER02_GAME_H
