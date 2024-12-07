#version 330 core

in vec2 TexCoord;

out vec4 FragColor;
in vec3 Normal;  
in vec3 FragPos;  
uniform sampler2D floorTexture;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform bool blinn;
uniform float shininess;
uniform float ambientStrength;
uniform float specularStrength; 
uniform float diffuseStrength;
uniform sampler2D tex;

void main() {
	FragColor = texture(tex, TexCoord);
    FragColor = vec4(1.0);
    
	  // ambient
    vec3 ambient = ambientStrength * lightColor;
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, norm), 0.0) * diffuseStrength;
    vec3 diffuse = diff * lightColor;
  
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
   
    vec3 specular = specularStrength  * lightColor * spec;  
        
    vec3 result =  objectColor *(ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
     
    
}
