//
//  VertexArray.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 13.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices)
    :mNumVerts(numVerts)
    ,mNumIndices(numIndices) {
    // Create vertex array
    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);
        
    // Create vertex buffer
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,                // The active buffer type to write to
        numVerts * 5 * sizeof(float),   // Number of bytes to copy
        verts,                          // Source to copy from (pointer)
        GL_STATIC_DRAW);                // How will we use this data?
    
    // Create index buffer
    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        
    // Specify the vertex attributes
    // (For now, assume one vertex format)
    // Position is 3 floats starting at offset 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                          0,                    // Attribute index (0 for first one)
                          3,                    // Number of components (3 in this case)
                          GL_FLOAT,             // Type of the components
                          GL_FALSE,             // (Only used for integral types)
                          sizeof(float) * 5,    // Stride (usually size of each vertex)
                          0);                   // Offset from start
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));
}

VertexArray::~VertexArray() {
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive() {
    glBindVertexArray(mVertexArray);
}
