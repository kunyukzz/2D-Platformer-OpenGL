#version 130

in vec2 fragPosition;
in vec4 fragColor;
in vec2 fragUV;

out vec4 color;

uniform sampler2D testSampler;

void main(){

    vec4 txColor = texture(testSampler, fragUV);
    color = fragColor * txColor;
}
