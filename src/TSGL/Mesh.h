/*
* Mesh.h provides a class for storing individual meshes loaded from 3d objects
*/

#ifndef MESH_H_
#define MESH_H_

// Include the vector template
#include <vector>
using namespace std;

// Import the GLM Library for access to vectors
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

// Import the ASSIMP library, which helps load 3D objects from files
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

// Include OpenGL Loader
#include <../glad/glad.h> // Loader for OpenGL function calls

namespace tsgl
{

class Mesh
{
  private:
    bool hasBeenSetup = false;
    GLuint VAO, VBO, EBO;

  public:
    // Create a structure to hold the data for each individual vertex
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec4 Color;
    };

    // Mesh arrays
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    Mesh(vector<Vertex> verts, vector<unsigned int> indices)
    {
        this->vertices = verts;
        this->indices = indices;

        // setupMesh();
    }

    ~Mesh()
    {
        //TODO delete the buffer objects and stuff here
    }

    void setupMesh()
    {
        // note this is not threadsafe at all
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the array object and buffer for this mesh
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        // Copy vertex data to the buffer
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        
        // Setup the GL array for finding vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

        // Setup the GL array for finding vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

        // Setup the GL array for finding vertex colors
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Color));


        // Bind the buffer which contains the face indices -> vertices mapping, then copy the data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     &indices[0], GL_STATIC_DRAW);


        // Unbind the VAO now that it is setup
        glBindVertexArray(0);

        hasBeenSetup = true;
    }

    void draw()
    {
        if (!hasBeenSetup) setupMesh();
        // note this is also nto threadsafe
        // Render the mesh out using opengl
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};
}

#endif /* MESH_H_ */
