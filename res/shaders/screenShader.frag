#version 310 es

precision mediump float;

#define identity float[9]( 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0 )
#define edge0 float[9](1.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0)
#define edge1 float[9](0.0, 1.0, 0.0, 1.0, -4.0, 1.0, 0.0, 1.0, 0.0)
#define edge2 float[9](-1.0, -1.0, -1.0, -1.0, 8.0, -1.0, -1.0, -1.0, -1.0)
#define sharpen float[9](0.0, -1.0, 0.0, -1.0, 5.0, -1.0, 0.0, -1.0, 0.0)
#define box_blur float[9](1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0) * 0.1111
#define gaussian_blur float[9](1.0, 2.0, 1.0, 2.0, 4.0, 2.0, 1.0, 2.0, 1.0) * 0.0625
#define box_blur2 float[9](1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 2.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0)
#define gaussian_blur2 float[9]( 1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0, 2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,  1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0 )
#define emboss float[9](-2.0, -1.0, 0.0, -1.0, 1.0, 1.0, 0.0, 1.0, 2.0)

out vec4 FragColor;
in vec2 TexCoords;

uniform int kernelType;
uniform float offset;
uniform float gamma;
uniform float exposure;
uniform sampler2D screenTexture;

float kernel[9] = float[9]( 
    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0 , //identity for default
    0.0, 0.0, 0.0 );

void main()
{
    //inverted
    //FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

    //grayscale
    //FragColor = texture(screenTexture, TexCoords);
    //float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    //FragColor = vec4(average, average, average, 1.0);

    //better grayscale
    //FragColor = texture(screenTexture, TexCoords);
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    //FragColor = vec4(average, average, average, 1.0);


    if(kernelType == 0){
    kernel = identity; 
    
    } else if(kernelType == 1){
    kernel = box_blur2 ;
    
    } else if(kernelType == 2){
    kernel = gaussian_blur2;
    
    } else if(kernelType == 3){
    kernel = edge0;
    
    } else if(kernelType == 4){
    kernel = edge1;
    
    } else if(kernelType == 5){
    kernel = edge2;
    
    } else if(kernelType == 6){
    kernel = sharpen;
    
    } else if(kernelType == 7){
    kernel = emboss;
    }
    
    vec2 offsets[9] = vec2[9](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    vec3 sampleTex[9];
    
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    
    vec3 col = vec3(0.0);
    
    for(int i = 0; i < 9; i++){
        col += sampleTex[i] * kernel[i];
    }
    
    // reinhard tone mapping
    vec3 mapped = vec3(1.0) - exp(-col * exposure);

    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped,  1.0);
}
