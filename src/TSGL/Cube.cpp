// #include "Cube.h"
//
// namespace tsgl {
//
//   Cube::Cube(float x, float y, float z, float width, float height, float depth, const ColorFloat &color, const ColorFloat& outlineColor) : Solid3D(24, color, outlineColor) {
//
//     // front
//     addVertex(x, y, z);
//     addVertex(x, y+height, z);
//     addVertex(x+width, y+height, z);
//
//     addVertex(x, y, z);
//     addVertex(x+width, y, z);
//     addVertex(x+width, y+height, z);
//
//     // back
//     addVertex(x, y, z+depth);
//     addVertex(x, y+height, z+depth);
//     addVertex(x+width, y+height, z+depth);
//
//     addVertex(x, y, z+depth);
//     addVertex(x+width, y, z+depth);
//     addVertex(x+width, y+height, z+depth);
//
//     // left side
//     addVertex(x, y, z);
//     addVertex(x, y, z+depth);
//     addVertex(x, y+height, z+depth);
//
//     addVertex(x, y, z);
//     addVertex(x, y+height, z);
//     addVertex(x, y+height, z+depth);
//
//     // right side
//     addVertex(x+width, y, z);
//     addVertex(x+width, y, z+depth);
//     addVertex(x+width, y+height, z+depth);
//
//     addVertex(x+width, y, z);
//     addVertex(x+width, y+height, z);
//     addVertex(x+width, y+height, z+depth);
//
//     // top
//     addVertex(x, y+height, z);
//     addVertex(x+width, y+height, z);
//     addVertex(x+width, y+height, z+depth);
//
//     addVertex(x, y+height, z);
//     addVertex(x, y+height, z+depth);
//     addVertex(x+width, y+height, z+depth);
//
//     // bottom
//     addVertex(x, y, z);
//     addVertex(x+width, y, z);
//     addVertex(x+width, y, z+depth);
//
//     addVertex(x, y, z);
//     addVertex(x, y, z+depth);
//     addVertex(x+width, y, z+depth);
// }
//
//   void Cube::render() {
//
//     glEnableVertexAttribArray(0);
//     glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//     glVertexAttribPointer(
//       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//       3,                  // size
//       GL_FLOAT,           // type
//       GL_FALSE,           // normalized?
//       0,                  // stride
//       (void*)0            // array buffer offset
//     );
//
//     // Draw the triangle !
//     glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
//
//     glDisableVertexAttribArray(0);
//
//
//     // // give opengl some info about the array of point we are about to pass into it
//     // glVertexPointer(
//     //   3,  // how many points per vertex (for us, that's x,y,z)
//     //   GL_FLOAT, // the type of data being passed
//     //   0, // byte offset between vertices
//     //   getPointerToVerticesArray()  // pointer to the array of vertices
//     // );
//     // // set the color of the object
//     // glColor4f(
//     //   getObjectColor()->R,
//     //   getObjectColor()->G,
//     //   getObjectColor()->B,
//     //   getObjectColor()->A
//     // );
//     // // draw the array of vertices
//     // glDrawArrays(
//     //   GL_TRIANGLES,
//     //   0, // The starting index of the array
//     //   getNumberOfVertices() // The number of vertices from the object
//     // );
//
//
//   }
//
// }
