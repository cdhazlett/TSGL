#include "Solid3D.h"

namespace tsgl {

Solid3D::Solid3D(int numVertices, const ColorFloat& c, const ColorFloat& outlineC) : Shape3D(numVertices, c) {
  attribMutex.lock();
  if (numVertices < 3) {
    TsglDebug("Cannot have a Solid3D with fewer than 3 vertices."); //TODO
  }
  hasOutline = true;
  outlineColor = outlineC;
  attribMutex.unlock();
}

GLfloat* Solid3D::getPointerToOutlineVerticesArray() {
  attribMutex.lock();
  GLfloat* result = vertices;
  attribMutex.unlock();
  return result;
}

int Solid3D::getOutlineNumberOfVertices() {
  attribMutex.lock();
  int l = length;
  attribMutex.unlock();
  return l;
}

ColorFloat* Solid3D::getOutlineColor() {
  attribMutex.lock();
  ColorFloat* c = &outlineColor;
  attribMutex.unlock();
  return c;
}

void Solid3D::setOutlineColor(const ColorFloat& newColor) {
  attribMutex.lock();
  outlineColor = newColor;
  attribMutex.unlock();
}

void Solid3D::setHasOutline(bool outline) {
  attribMutex.lock();
  hasOutline = outline;
  attribMutex.unlock();
}

}
