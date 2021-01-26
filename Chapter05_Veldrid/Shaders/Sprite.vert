#version 450

// Uniforms for world transform and view-proj
layout(set = 0, binding = 0) uniform ViewProjectionBuffer
{
    mat4 ViewProjection;
};

layout(set = 1, binding = 0) uniform WorldTransformBuffer
{
    mat4 WorldTransform;
};

// Attribute 0 is position, 2 is tex coords.
layout(location = 0) in vec3 Position;
layout(location = 2) in vec2 TextureCoordinate;

// Add texture coordinate as output
layout(location = 0) out vec2 fragTexCoord;

void main()
{
    // Convert position to homogeneous coordinates
    vec4 pos = vec4(Position, 1.0);

    // Transform position to world space, then clip space
    gl_Position = ViewProjection * WorldTransform * pos;

    // Pass along the texture coordinate to frag shader
    fragTexCoord = TextureCoordinate;
}
