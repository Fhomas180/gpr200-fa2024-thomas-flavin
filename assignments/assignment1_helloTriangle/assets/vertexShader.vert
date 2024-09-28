#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCoord;
in vec3 vp;
out vec3 ourColor;
out vec2 TexCoord;
uniform float _Time;

void main()
{ 
  vec3 Cool = vec3(aPos.x + cos(_Time), aPos.y + sin(_Time) , 0)*0.5; 
  //vp *= sin(_Time);
  vec3 animatedPos = aPos;
  animatedPos.x += sin(_Time) * 0.1;  // Simple animation
  gl_Position = vec4(animatedPos, 1.0);
    
  gl_Position = vec4(aPos + Cool, 1.0);
  gl_Position = vec4(vp, 1.0);
  gl_Position = vec4(aPos, 1.0);
  TexCoord = aTexCoord;
  ourColor = aColor;
  
};
