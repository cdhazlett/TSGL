// #include "Shape3D.h"
//
// namespace tsgl {
//
//   // Shape3D::Shape3D(int numVertices, const ColorFloat& c) {
//   //   attribMutex.lock();
//   //
//   //   discreteRender = true; // Render 3D shapes discretely to keep 2D performance at top speed
//   //
//   //   if (numVertices < 1) {
//   //     TsglDebug("Cannot have a shape without a vertex."); }
//   //   length = numVertices;
//   //   size = length * 3;
//   //   current = 0;
//   //   vertices = new float[size];
//   //   init = false;
//   //   color = c;
//   //   attribMutex.unlock();
//   // }
//
//   // Shape3D::~Shape3D() {
//   //   attribMutex.lock();
//   //   delete[] vertices;
//   //   attribMutex.unlock();
//   // }
//   //
//   // GLfloat* Shape3D::getPointerToVerticesArray() {
//   //   attribMutex.lock();
//   //   GLfloat* v = vertices;
//   //   attribMutex.unlock();
//   //   return v;
//   // }
//
//   // int Shape3D::getNumberOfVertices() {
//   //   attribMutex.lock();
//   //   int l = length;
//   //   attribMutex.unlock();
//   //   return l;
//   // }
//
//   void Shape3D::addVertex(float x, float y, float z) {
//     attribMutex.lock();
//     if (init) {
//       TsglDebug("Cannot add anymore vertices.");
//       attribMutex.unlock(); //TODO was this commented for some reason?  Seems legit to me
//       return;
//     }
//     vertices[current] = x;
//     vertices[current + 1] = y;
//     vertices[current + 2] = z;
//     current += 3;
//     if (current == size) init = true;
//     attribMutex.unlock();
//   }
//
//   ColorFloat Shape3D::getColor() {
//     attribMutex.lock();
//     ColorFloat c = color;
//     attribMutex.unlock();
//     return c;
//   }
//
//   ColorFloat* Shape3D::getObjectColor() {
//     attribMutex.lock();
//     ColorFloat* c = &color;
//     attribMutex.unlock();
//     return c;
//   }
//
//
//   void Shape3D::setColor(const ColorFloat& c) {
//     attribMutex.lock();
//     color = c;
//     attribMutex.unlock();
//   }
//
//   void Shape3D::setCenter(float x, float y, float z) {
//     float deltaX = x - getX(); //Change for x
//     float deltaY = y - getY(); //Change for y
//     float deltaZ = z - getZ(); //Change for z
//     attribMutex.lock();
//
//     for(int i = 0; i < length; i++) {
//       vertices[i*3]     += deltaX; //Transpose x
//       vertices[(i*3)+1] += deltaY; //Transpose y
//       vertices[(i*3)+2] += deltaZ; //Transpose z
//     }
//     attribMutex.unlock();
//   }
//
//   float Shape3D::getX() { //TODO: decide if this is the best system to protect x and y
//     attribMutex.lock();
//     float minX, maxX;
//     minX = maxX = vertices[0];
//
//     //Find min and max X
//     for(int i = 0; i < length; i++) {
//       if( vertices[i*3] < minX )
//         minX = vertices[i*3];
//       else if( vertices[i*3] > maxX )
//         maxX = vertices[i*3];
//     }
//
//     attribMutex.unlock();
//     return (minX+maxX)/2;
//   }
//
//   float Shape3D::getY() {
//     attribMutex.lock();
//     float minY, maxY;
//     minY = maxY = vertices[1];
//
//     //Find min and max Y
//     for(int i = 0; i < length; i++) {
//       if( vertices[i*3+1] < minY )
//         minY = vertices[i*3+1];
//       else if( vertices[i*3+1] > maxY )
//         maxY = vertices[i*3+1];
//     }
//
//     attribMutex.unlock();
//     return (minY+maxY)/2;
//   }
//
//   float Shape3D::getZ() {
//     attribMutex.lock();
//     float minZ, maxZ;
//     minZ = maxZ = vertices[2];
//
//     //Find min and max Z
//     for(int i = 0; i < length; i++) {
//       if( vertices[i*3+2] < minZ )
//         minZ = vertices[i*3+2];
//       else if( vertices[i*3+2] > maxZ )
//         maxZ = vertices[i*3+2];
//     }
//
//     attribMutex.unlock();
//     return (minZ+maxZ)/2;
//   }
//
//   void Shape3D::centeredRotation(float angle) {
//     setRotation(angle, getX(), getY());
//   }
//
// }
