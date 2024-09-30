 #include <iostream> 
 "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform float _Time;"
"uniform sampler2D backgroundTexture;\n"
"uniform sampler2D characterTexture;"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor *sin(_Time), 1.0f);\n"
"   vec4 backgroundColor = texture(backgroundTexture, TexCoord);\n"
"vec4 characterColor = texture(characterTexture, TexCoord);"
" FragColor = characterColor * characterColor.a + backgroundColor * (1.0 - characterColor.a);\n"
"}\n\0";