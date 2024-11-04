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
    //vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
	  // ambient
   // float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	//ambient = 0.05 * color;
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, norm), 0.0) * diffuseStrength;
    vec3 diffuse = diff * lightColor;
    //vec3 normal = normalize(fs_in.Normal);
  
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
   // if (blinn) // Use a uniform to switch between Phong and Blinn-Phong
    //{
       //  vec3 halfwayDir = normalize(lightDir + viewDir);
    //   // Blinn-Phong specular calculation
        //spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
   // }//
    //else
   // {
     //   vec3 reflectDir = reflect(-lightDir, norm);
      //  spec = pow(max(dot( reflectDir, viewDir), 0.0), shininess);
  //  }

    // specular
    //float specularStrength = 0.5;
    
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result =  objectColor *(ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
     
    // ambient
    
    // diffuse
    
    
   // float diff = max(dot(lightDir, normal), 0.0);
   // vec3 diffuse = diff * color;
    // specular
    //vec3 viewDir = normalize(viewPos - fs_in.FragPos);
   // vec3 reflectDir = reflect(-lightDir, normal);
   // float spec = 0.0;
  
    
}
