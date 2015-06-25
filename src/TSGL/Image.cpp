/*
 * Image.cpp
 *
 *  Created on: Jul 8, 2014
 *      Author: pretzel
 */

#include "Image.h"

namespace tsgl {

Image::Image(std::string f, TextureHandler &loader, int x, int y, int w, int h, float a) {
    isTextured = true;  // Let the Canvas know we're a textured object
    myWidth = w; myHeight = h;
    if (myWidth <= 0 || myHeight <= 0) {
      TextureHandler::getDimensions(f,myWidth,myHeight);
    }
    myFile = f;
    myLoader = &loader;
    vertices[0] = x;
    vertices[1] = y;
    vertices[8] = x + myWidth;
    vertices[9] = y;
    vertices[16] = x;
    vertices[17] = y + myHeight;
    vertices[24] = x + myWidth;
    vertices[25] = y + myHeight;
    vertices[2] = vertices[10] = vertices[18] = vertices[26] = 1.0f;  // Texture color of the coords
    vertices[3] = vertices[11] = vertices[19] = vertices[27] = 1.0f;
    vertices[4] = vertices[12] = vertices[20] = vertices[28] = 1.0f;
    vertices[5] = vertices[13] = vertices[21] = vertices[29] = a;
    vertices[6] = vertices[7] = 0.0f;           // Texture coords of top left
    vertices[14] = 1.0f, vertices[15] = 0.0f;   // Texture coords of top right
    vertices[22] = 0.0f, vertices[23] = 1.0f;   // Texture coords of bottom left
    vertices[30] = vertices[31] = 1.0f;         // Texture coords of bottom right
}

void Image::draw() {
    unsigned int w, h;
    myLoader->loadPicture(myFile, w, h, myTexture);

    glBindTexture(GL_TEXTURE_2D, myTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

}
