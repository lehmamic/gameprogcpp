// Request GLSL 3.3
#version 330

// Any vertex attribute go here
// For now, just a position.
in vec3 inPosition;

void main()
{
    // Directly pass along inPosition to gl_Position
    gl_Position = vec4(inPosition, 1.0);
}
