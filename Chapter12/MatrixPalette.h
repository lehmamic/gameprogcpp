//
//  MatrixPalette.h
//  Chapter12-mac
//
//  Created by Michael Lehmann on 26.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef MatrixPalette_h
#define MatrixPalette_h

#include "Math.h"

const size_t MAX_SKELETON_BONES = 96;

struct MatrixPalette
{
    Matrix4 mEntry[MAX_SKELETON_BONES];
};

#endif /* MatrixPalette_h */
