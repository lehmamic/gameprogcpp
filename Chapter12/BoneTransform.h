//
//  BoneTransform.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 26.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef BoneTransform_hpp
#define BoneTransform_hpp

#include "Math.h"

class BoneTransform
{
public:
    // For now, just make this data public
    Quaternion mRotation;
    Vector3 mTranslation;
    
    // Convert to matrix
    Matrix4 ToMatrix() const;
    
    static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};


#endif /* BoneTransform_hpp */
