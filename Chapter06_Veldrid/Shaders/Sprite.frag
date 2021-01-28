#version 450

// Tex coord input from vertex shader
layout(location = 0) in vec2 fragTexCoord;

// This corresponds to the output color to the color buffer
layout(location = 0) out vec4 outColor;

// This is used for the texture sampling
layout(set = 1, binding = 1) uniform texture2D SurfaceTexture;
layout(set = 1, binding = 2) uniform sampler SurfaceSampler;

void main()
{
    // Sample color from texture
    outColor = texture(sampler2D(SurfaceTexture, SurfaceSampler), fragTexCoord);
}
