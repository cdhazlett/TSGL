#include "Shape.h"

namespace tsgl
{

Shape::Shape(int numVertices, const ColorFloat &c) : Drawable(numVertices)
{
  attribMutex.lock();
  if (numVertices < 1)
  {
    TsglDebug("Cannot have a shape without a vertex.");
  }

  init = false;
  color = c;

  attribMutex.unlock();
}

Shape::~Shape()
{
  // Delete the memory from the graphics card
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
}

GLfloat *Shape::getPointerToVerticesArray()
{
  return getPointerToVerticesArray();
}

void Shape::addVertex(float x, float y)
{
  // if (init)
  // {
  //   TsglDebug("Cannot add anymore vertices.");
  //   // attribMutex.unlock();
  //   return;
  // }

  addVertexWithRGBA(x, y, 0.f, color.R, color.G, color.B, color.A);
}

ColorFloat Shape::getColor()
{
  attribMutex.lock();
  ColorFloat c = color;
  attribMutex.unlock();
  return c;
}

ColorFloat *Shape::getObjectColor()
{
  attribMutex.lock();
  ColorFloat *c = &color;
  attribMutex.unlock();
  return c;
}

void Shape::setColor(const ColorFloat &c)
{
  setColorForAllVertices(c);
}

void Shape::setCenter(float x, float y)
{
  // float deltaX = x - getX(); //Change for x
  // float deltaY = y - getY(); //Change for y
  // attribMutex.lock();

  // for (int i = 0; i < length; i++)
  // {
  //   vertices[i * 2] += deltaX;       //Transpose x
  //   vertices[(i * 2) + 1] += deltaY; //Transpose y
  // }
  // attribMutex.unlock();

  resetTranslation();
  translate(x, y, 0.f);
}

float Shape::getX()
{
  //TODO: decide if this is the best system to protect x and y
  //   attribMutex.lock();
  //   float minX, maxX;
  //   minX = maxX = vertices[0];

  //   //Find min and max X
  //   for (int i = 0; i < length; i++)
  //   {
  //     if (vertices[i * 2] < minX)
  //       minX = vertices[i * 2];
  //     else if (vertices[i * 2] > maxX)
  //       maxX = vertices[i * 2];
  //   }

  //   attribMutex.unlock();
  //   return (minX + maxX) / 2;
  return getTranslation().x;
}

float Shape::getY()
{
  // attribMutex.lock();
  // float minY, maxY;
  // minY = maxY = vertices[1];

  // //Find min and max X
  // for (int i = 0; i < length; i++)
  // {
  //   if (vertices[i * 2 + 1] < minY)
  //     minY = vertices[i * 2 + 1];
  //   else if (vertices[i * 2 + 1] > maxY)
  //     maxY = vertices[i * 2 + 1];
  // }

  // attribMutex.unlock();
  // return (minY + maxY) / 2;

  return getTranslation().y;
}

void Shape::centeredRotation(float angle)
{
  resetRotation();
  rotate(angle, 0, 0, 1); // For a 2D object, rotate around the Z axis, the one pointing out toward the screen.
}

void Shape::setupShape()
{
  // Create VAOs and VBOs for the object
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  int numVerts = getNumberOfVertices();
  attribMutex.lock();
  // Bind the array object and buffer for this mesh
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Create a temporary array to pack all the data in
  GLfloat packingArr[numVerts * 10];
  memset(&packingArr, 0, sizeof(GLfloat) * numVerts * 10);
  
  int i;
  for (i=0; i<numVerts; i++) {
    // Vertex position
    packingArr[10*i + 0] = (GLfloat)vertexArray[3*i + 0]; //x
    packingArr[10*i + 1] = (GLfloat)vertexArray[3*i + 1]; //y
    packingArr[10*i + 2] = (GLfloat)vertexArray[3*i + 2]; //z

    // Vertex normals
    packingArr[10*i + 3] = (GLfloat)vertexNormalArray[3*i + 0]; //x
    packingArr[10*i + 4] = (GLfloat)vertexNormalArray[3*i + 1]; //y
    packingArr[10*i + 5] = (GLfloat)vertexNormalArray[3*i + 2]; //z

    // Colors
    packingArr[10*i + 6] = (GLfloat)vertexColorArray[4*i + 0]; //R
    packingArr[10*i + 7] = (GLfloat)vertexColorArray[4*i + 1]; //G
    packingArr[10*i + 8] = (GLfloat)vertexColorArray[4*i + 2]; //B
    packingArr[10*i + 9] = (GLfloat)vertexColorArray[4*i + 3]; //A
  }

  // Copy vertex data to the buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVerts * 10, &packingArr, GL_STATIC_DRAW);

  // Setup the GL array for finding vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*10, (void *)0);

  // Setup the GL array for finding vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*10, (void *)(sizeof(GLfloat)*3));

  // Setup the GL array for finding vertex colors
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*10, (void *)(sizeof(GLfloat)*6));

  // Unbind the VAO now that it is setup
  glBindVertexArray(0);

  hasBeenSetup = true;
  attribMutex.unlock();
}

void Shape::render()
{
  if (!hasBeenSetup)
    setupShape();
  // note this is also not threadsafe
  // Render the shape out using opengl
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, getNumberOfVertices() * 10);
  glBindVertexArray(0);
}

} // namespace tsgl
