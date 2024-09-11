#version 130

in vec2 fragPosition;
in vec4 fragColor;
in vec2 fragUV;

out vec4 color;

uniform float time;
uniform sampler2D testSampler;

void main(){

    vec4 txColor = texture(testSampler, fragUV);
    //color = txColor * fragColor;

    color = vec4(fragColor.r * (cos((fragPosition.x * 4.0) + time) + 1.0) * 1.0,
                 fragColor.g * (cos((fragPosition.y * 8.0) + time) + 1.0) * 1.0,
                 fragColor.b * (sin((fragPosition.x * 3.0) + time) + 1.0) * 1.0, fragColor.a) * txColor;
}
