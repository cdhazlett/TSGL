#version 330 core

// Interpolated values from the vertex shaders
in vec3 pos;  // Position in world space
in vec3 normal;  // Normal vector to surface
in vec4 colorData;
in vec3 eye_dir;
in vec3 cameraLightDir;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform mat4 M;
uniform mat4 MV;
uniform vec3 Light_Coord;

void main(){

	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 50.0f;

	// Material properties
	vec3 MaterialDiffuseColor = vec3(colorData.x, colorData.y, colorData.z);
	vec3 MaterialAmbientColor = vec3(0.4,0.4,0.4) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.01,0.01,0.01);

	// Distance to the light
	float distance = length( Light_Coord - pos );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( normal );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( cameraLightDir );
	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );

	// Eye vector (towards the camera)
	vec3 E = normalize(eye_dir);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	color =
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);

}
