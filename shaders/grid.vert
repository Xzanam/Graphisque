#version 330 core 
layout (location = 0) in vec3 aPos; 

uniform mat4 mvp;
uniform float cellSize;
uniform int majorGridInterval;

flat out float gridLine;
void main() 
{
    gl_Position = mvp * vec4(aPos, 1.0f);
    float epsilon = 0.01;

    //Determine if this is major grid line
    float lineIndex = abs(round(aPos.x / cellSize));
    float m = mod(lineIndex, float(majorGridInterval));
    gridLine = abs(m) < epsilon? 1.0 : 0.0;

    //Check for horizontal lines aswell
    if(gridLine == 0.0) { 
        lineIndex = round(aPos.y / cellSize); 
        float m = mod(lineIndex, float(majorGridInterval));
        gridLine = abs(m) < epsilon ? 1.0 : 0.0;
    }

}
