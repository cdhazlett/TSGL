/*
* Object3D.h extends the drawable and provides support for loading in arbitrary 3D models
*/

#ifndef OBJECT3D_H_
#define OBJECT3D_H_

// Include the vector template
#include <vector>
using namespace std;

#include "Drawable.h" // For extending our Solid3D object
#include "Mesh.h"     // Import the Mesh class to store the object data

// Import the ASSIMP library, which helps load 3D objects from files
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

namespace tsgl
{

/*! \class Cube
  *  \brief Draw a simple Cube.
  *  \details Cube is a class for holding vertex data for a simple Cube.
  */
class Object3D : public Drawable
{
  private:
    void processNode(aiNode *node, const aiScene *scene)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        vector<Mesh::Vertex> vertices;
        vector<unsigned int> indices;

        // Process vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {

            Mesh::Vertex vertex;

            // process vertex positions, normals and texture coordinates
            glm::vec3 posVector;
            posVector.x = mesh->mVertices[i].x;
            posVector.y = mesh->mVertices[i].y;
            posVector.z = mesh->mVertices[i].z;
            vertex.Position = posVector;

            glm::vec3 normVector;
            normVector.x = mesh->mNormals[i].x;
            normVector.y = mesh->mNormals[i].y;
            normVector.z = mesh->mNormals[i].z;
            vertex.Normal = normVector;

            glm::vec4 colorVector = glm::vec4(1.f, 1.f, 1.f, 1.f);
            // if (i%2 == 0) colorVector = glm::vec4(.5, 0.f, 1.f, 1.f);
            // if (mesh->HasVertexColors(i))
            // {
                const aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
                aiColor4D diffuse;
                if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
                    colorVector = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
            // }

#ifdef RANDOMCOLORS
colorVector = glm::vec4((float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100, 1);
#endif

            vertex.Color = colorVector;

            // Add the vertex to the mesh
            vertices.push_back(vertex);
        }

        // Process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return Mesh(vertices, indices);
    }

  protected:
    // Create an ASSIMP importer object
    Assimp::Importer importer;
    // Create ASSIMP Scene handle
    const aiScene *scene;
    // Vector of meshes
    vector<Mesh> meshes;

    // GL Uint handles
    GLuint objectArrayID;                                             // VAO
    GLuint objectVertexBuffer, objectColorBuffer, objectNormalBuffer; // Buffer objects for the data

  public:
    Object3D(std::string objectPath) : Drawable(3)
    {
        // Open the file and read is with ASSIMP
        scene = importer.ReadFile(objectPath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_GenNormals | aiProcess_PreTransformVertices);

        // Check to see if the import was successful
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            fprintf(stderr, "Error importing a 3D object from the file: %s\nASSIMP says: %s\n\n", objectPath, importer.GetErrorString());
            return;
        }

        processNode(scene->mRootNode, scene);
    }

    void render()
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            meshes[i].draw();
            // printf("rendering %d", i);
        }
    }

    virtual void render(GLuint &vertexbuffer, GLuint &colorbuffer, GLuint &normalbuffer) { render(); }
};
}

#endif /* OBJECT3D_H_ */
