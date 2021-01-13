//
//  Tile.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 13.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef Tile_hpp
#define Tile_hpp

#include "Actor.h"
#include <vector>

class Tile : public Actor {
public:
    friend class Grid;
    enum TileState {
        EDefault,
        EPath,
        EStart,
        EBase
    };
    
    Tile(class Game* game);
    
    void SetTileState(TileState state);
    TileState GetTileState() const { return mTileState; }
    void ToggleSelect();
    const Tile* GetParent() const { return mParent; }
private:
    // For pathfinding
    std::vector<Tile*> mAdjacent;
    Tile* mParent;
    float f;
    float g;
    float h;
    bool mInOpenSet;
    bool mInClosedSet;
    bool mBlocked;
    
    void UpdateTexture();
    class SpriteComponent* mSprite;
    TileState mTileState;
    bool mSelected;
};
#endif /* Tile_hpp */
