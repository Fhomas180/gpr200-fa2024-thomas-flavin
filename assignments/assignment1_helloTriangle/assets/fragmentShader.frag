#version 330 core
out vec4 FragColor;
out vec4 frag_colour;
in vec3 ourColor;
in vec2 TexCoord;
uniform float _Time;
uniform sampler2D backgroundTexture;
uniform sampler2D characterTexture;
void main()
{
  FragColor = vec4(ourColor *sin(_Time), 1.0f);
  FragColor = texture(backgroundTexture, TexCoord * 4.0);
  FragColor = texture(characterTexture, TexCoord);
  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);
    if (FragColor.a < 0.1) { // Alpha clipping
        discard;
        
}
  
};