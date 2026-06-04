#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D fluidTex;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    vec2 velocity = texture(fluidTex, TexCoord).rg;
    
    float speed = length(velocity);
    float angle = atan(velocity.y, velocity.x);
    
    float hue = (angle + 3.14159265) / (2.0 * 3.14159265);
    
    float brightness = clamp(speed, 0.0, 1.0);
    
    vec3 rgb = hsv2rgb(vec3(hue, 1.0, brightness));
    
    FragColor = vec4(rgb, 1.0);
}