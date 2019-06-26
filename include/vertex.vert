#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix; 
out float diffuse;        
out vec2 TexCoords;

// the simplest function to calculate lighting 
float doColor(){
    vec3 norm  = normalize( normalMatrix * normalize(aNormal) );
    vec3 light = normalize( vec3(1.0, 1.0, 1.0) );
    diffuse = max(dot(norm,light),0.0 );
    return diffuse;
  }
  
void main()
{
    diffuse = doColor();
	TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}