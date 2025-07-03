#version 330 core 
layout (location = 0) in vec3 aPos; 

uniform mat4 mvp;
uniform float cellSize;
uniform int majorGridInterval;

out float gridLine;
void main() 
{
    gl_Position = mvp * vec4(aPos, 1.0f);

    //Determine if this is major grid line
    float lineIndex = round(aPos.x / cellSize);
    gridLine = mod(lineIndex, float(majorGridInterval)) == 0.0 ? 1.0 : 0.0;

    //Check for horizontal lines aswell
    if(gridLine == 0.0) { 
        lineIndex = round(aPos.y / cellSize); 
        gridLine = mod(lineIndex, float(majorGridInterval)) == 0.0 ? 1.0f : 0.0;
    }

}
