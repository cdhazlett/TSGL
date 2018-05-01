// #version 330 core
//
// // Input vertex data, different for all executions of this shader.
// layout(location = 0) in vec3 modelVertexPos;
// layout(location = 1) in vec4 colorVec;
// layout(location = 2) in vec3 modelVertexNormal;
//
// // Output data ; will be interpolated for each fragment.
// out vec4 colorData;
// out vec3 pos;  // Position in world space
// out vec3 normal;  // Normal vector to surface
// out vec3 eye_dir;
// out vec3 cameraLightDir;
//
// // Values that stay constant for the whole mesh.
// uniform mat4 MVP;
// uniform mat4 V;
// uniform mat4 M;
// uniform vec3 Light_Coord;
//
// void main(){
//
//   colorData = colorVec;
//
//   // Set the output position of the vertex by multiplying the MVP matrix with
//   // the vertex position
//   gl_Position =  MVP * vec4(modelVertexPos,1);
//
//   // Send the position of the vertex in world-space to the fragment shader
//   pos = (M * vec4(modelVertexPos,1)).xyz;
//
//   // Compute a vector from the surface in world-space to the camera
//   // In the camera space, the camera is at the position (0,0,0)
//   vec3 cameraSpaceVertexPos = ( V * M * vec4(modelVertexPos,1)).xyz;
//   eye_dir = vec3(0,0,0) - cameraSpaceVertexPos;
//
//   // Compute a vector going from the surface to the light source, in camera space
//   vec3 cameraSpaceLightPosition = ( V * vec4(Light_Coord,1)).xyz;
//   cameraLightDir = cameraSpaceLightPosition + eye_dir;
//
//   // Translate the surface normal to camera space
//   normal = ( V * M * vec4(modelVertexNormal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
//
// }




#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
// Input color data
layout(location = 1) in vec4 vertexColor;
// Input normal vector
layout(location = 2) in vec3 normalVector;

out vec4 colorData;
out vec3 normalVec;
out vec3 vertPos;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 Light_Coord;


void main(){
  colorData = vertexColor;
  normalVec = normalize(vec4(normalVec.x, normalVec.y, normalVec.z, 0)).xyz;
  vertPos = vertexPosition;
  // Output position of the vertex, in clip space : MVP * position
  gl_Position =  MVP * vec4(vertexPosition,1);
}
