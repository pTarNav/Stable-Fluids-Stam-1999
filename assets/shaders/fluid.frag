#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D fluidTexture; 

void main() {
    // 1. Read the density float from the texture (e.g., 0.0 to 100.0)
    float density = texture(fluidTexture, TexCoord).r;

    // 2. Map the density to a color! 
    // Let's make the smoke a cool cyan/blue color.
    // If density is 0, it renders black. As it grows, it gets brighter.
    vec3 color = vec3(0.0, 0.8, 1.0) * density;

    FragColor = vec4(color, 1.0);
}