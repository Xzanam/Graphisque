#version 330 core
out vec4 FragColor;

uniform vec4 majorGridColor; // Color for major grid lines
uniform vec4 minorGridColor; // Color for minor grid lines
uniform int majorGridInterval; // Interval for major grid lines

// This uniform can be used to differentiate between major and minor grid lines
uniform bool isMajorGrid;

void main() {
    if (isMajorGrid) {
        FragColor = majorGridColor;
    } else {
        FragColor = minorGridColor;
    }
}
