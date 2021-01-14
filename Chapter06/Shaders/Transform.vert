// Request GLSL 3.3
#version 330

// uniform is a global variable which value does not change (often), in and out change every call
// Uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Any vertex attribute go here
// For now, just a position.
in vec3 inPosition;

void main()
{
    
    vec4 pos = vec4(inPosition, 1.0);
    gl_Position = pos * uWorldTransform * uViewProj;
}
