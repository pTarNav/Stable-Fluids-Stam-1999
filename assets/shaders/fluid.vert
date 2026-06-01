#version 330 core
layout (location = 0) in vec2 aPos;   // Matches glVertexAttribPointer 0
layout (location = 1) in vec2 aTex;   // Matches glVertexAttribPointer 1

out vec2 TexCoord; // Send this to the Fragment Shader

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoord = aTex;
}