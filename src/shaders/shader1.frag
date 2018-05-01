// #version 330 core
//
// // Interpolated values from the vertex shaders
// in vec4 colorData;
// in vec3 pos;  // Position in world space
// in vec3 normal;  // Normal vector to surface
// in vec3 eye_dir;
// in vec3 cameraLightDir;
//
// // Ouput data
// out vec3 color;
//
// // Values that stay constant for the whole mesh.
// uniform mat4 MV;
// uniform vec3 Light_Coord;
//
// void main(){
//
// 	// Light emission properties
// 	// You probably want to put them as uniforms
// 	vec3 LightColor = vec3(1,1,1);
// 	float LightPower = 50.0f;
//
// 	// Material properties
// 	vec3 MaterialDiffuseColor = vec3(colorData.x, colorData.y, colorData.z);
// 	vec3 MaterialAmbientColor = vec3(0.4,0.4,0.4) * MaterialDiffuseColor;
// 	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);
//
// 	// Distance to the light
// 	float distance = length( Light_Coord - pos );
//
// 	// Normal of the computed fragment, in camera space
// 	vec3 n = normalize( normal );
// 	// Direction of the light (from the fragment to the light)
// 	vec3 l = normalize( cameraLightDir );
// 	// Cosine of the angle between the normal and the light direction,
// 	// clamped above 0
// 	//  - light is at the vertical of the triangle -> 1
// 	//  - light is perpendicular to the triangle -> 0
// 	//  - light is behind the triangle -> 0
// 	float cosTheta = clamp( dot( n,l ), 0,1 );
//
// 	// Eye vector (towards the camera)
// 	vec3 E = normalize(eye_dir);
// 	// Direction in which the triangle reflects the light
// 	vec3 R = reflect(-l,n);
// 	// Cosine of the angle between the Eye vector and the Reflect vector,
// 	// clamped to 0
// 	//  - Looking into the reflection -> 1
// 	//  - Looking elsewhere -> < 1
// 	float cosAlpha = clamp( dot( E,R ), 0,1 );
//
// 	color =
// 		// Ambient : simulates indirect lighting
// 		MaterialAmbientColor +
// 		// Diffuse : "color" of the object
// 		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
// 		// Specular : reflective highlight, like a mirror
// 		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
//
// }



#version 330 core
//
// // Data
// in vec4 colorData;
// in vec3 normalVec;
// out vec3 color;
//
// void main()
// {
//
// 	// Output color = red
// 	color = vec3(normalVec.x, normalVec.y, normalVec.z);
//
// }


precision mediump float;

in vec3 normalVec;  // Surface normal
in vec3 vertPos;       // Vertex position
in vec4 colorData;

int mode = 0;   // Rendering mode
float Ka =.2;   // Ambient reflection coefficient
float Kd = .4;   // Diffuse reflection coefficient
float Ks = .2;   // Specular reflection coefficient
float shininessVal = .2; // Shininess

// Material color
vec3 ambientColor = colorData.xyz;
vec3 diffuseColor = colorData.xyz;
vec3 specularColor = colorData.xyz;

uniform vec3 Light_Coord; // Light position

out vec4 color;

void main() {
  vec3 N = normalize(normalVec);
  vec3 L = normalize(Light_Coord - vertPos);

  // Lambert's cosine law
  float lambertian = max(dot(N, L), 0.0);

  float specular = 0.0;

  if(lambertian > 0.0) {
    vec3 R = reflect(-L, N);      // Reflected light vector
    vec3 V = normalize(-vertPos); // Vector to viewer

    // Compute the specular term
    float specAngle = max(dot(R, V), 0.0);
    specular = pow(specAngle, shininessVal);
  }
  color = vec4(Ka * ambientColor +
                      Kd * lambertian * diffuseColor +
                      Ks * specular * specularColor, 1.0);

  // only ambient
  if(mode == 2) color = vec4(Ka * ambientColor, 1.0);
  // only diffuse
  if(mode == 3) color = vec4(Kd * lambertian * diffuseColor, 1.0);
  // only specular
  if(mode == 4) color = vec4(Ks * specular * specularColor, 1.0);
}
