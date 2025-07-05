#version 330 core

flat in float gridLine;

uniform vec4 majorGridColor; // Color for major grid lines
uniform vec4 minorGridColor; // Color for minor grid lines

uniform float zoomLevel;

out vec4 FragColor;

void main() {
    float alpha = 1.0;
    if(gridLine == 0.0){ 
        alpha = smoothstep(0.1, 0.5, zoomLevel);
    }
    vec4 color = mix(minorGridColor, majorGridColor, gridLine);
    color.a *= alpha;

    FragColor = color;
}
