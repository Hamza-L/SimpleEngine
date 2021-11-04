//
// Created by hlahm on 2021-10-25.
//

#ifndef VULKANENGINE2_SHAPES_H
#define VULKANENGINE2_SHAPES_H

//#define M_PI           3.14159265358979323846  /* pi */

#include <vector>

#include <iostream>
#include <fstream>
#include <string>

using namespace glm;

struct Material{
    vec3 color;
    float specHL;
};

struct PushObj{
    mat4 M = GLM_MAT4_IDENTITY;
    float isTex = 0;
};

struct MeshObject{
    char* name;
    struct PushObj pObj;

    struct Material material;

    int textureID;

    uint32_t pipelineFlag;

    std::vector<MeshObject> children;

    //data
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    //buffers for GPU Access only
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
};

struct Objects{
    std::vector<MeshObject> objects;
};

static void transformVert(struct MeshObject* object, mat4 model)
{
    for(auto & vertex : object->vertices)
    {
        vertex.position = vec3(model * vec4(vertex.position, 1.0f));
        vertex.normal = vec3(model * vec4(vertex.normal, 0.0f));
    }
}

static void getVertandIndx(struct MeshObject* object, std::vector<struct Vertex>* vertices, std::vector<uint32_t>* indices)
{
    if(object->children.size() == 0){
        for(uint32_t indx : object->indices)
        {
            indices->push_back(indx + vertices->size());
        }
        for(Vertex v : object->vertices)
        {
            vertices->push_back(v);
        }

    } else {
        for(struct MeshObject obj : object->children)
        {
            getVertandIndx(&obj, vertices, indices);
        }
    }

}


static struct MeshObject plane( mat4 model, vec3 color){

    struct MeshObject object = {};
    std::vector<Vertex> vertices = {
            {(vec3){0.5f,0.5f,0.0f}  ,{0.0f,0.0f,1.0f}, {color[0],color[1],color[2]}, {1.0f,0.0f}},
            {(vec3){-0.5f,0.5f,0.0f}  ,{0.0f,0.0f,1.0f}, {color[0],color[1],color[2]}, {0.0f,0.0f}},
            {(vec3){-0.5f,-0.5f,0.0f} , {0.0f,0.0f,1.0f},{color[0],color[1],color[2]}, {0.0f,1.0f}},
            {(vec3){0.5f,-0.5f,0.0f} ,  {0.0f,0.0f,1.0f},{color[0],color[1],color[2]}, {1.0f,1.0f}}
    };

    std::vector<uint32_t> indices ={  1,0,2,
                                      2,0,3   };

    object.vertices = vertices;
    object.indices = indices;
    object.pObj.M = GLM_MAT4_IDENTITY;

    transformVert(&object, model);

    struct Material material = { {color[0],color[1],color[2]}, 16.0f};
    object.material = material;

    return object;
}

static struct MeshObject cube( mat4 model, vec3 color)
{
    struct MeshObject cube = {};
    mat4 modelMat;

    cube.pObj.M = GLM_MAT4_IDENTITY;

    float degrees[] = {90.0f,-90.0f,90.0f,-90.0f,0.0f,180.0f};
    float axes[] = {  1.0f,0.0f,0.0f,
                      1.0f,0.0f,0.0f,
                      0.0f,1.0f,0.0f,
                      0.0f,1.0f,0.0f,
                      0.0f,0.0f,1.0f,
                      1.0f,0.0f,0.0f};

    struct MeshObject cubePlane = {};

    for(int i = 0; i  < 6; i++)
    {
        modelMat = rotate(GLM_MAT4_IDENTITY,radians(degrees[i]),(vec3){axes[i*3],axes[i*3+1],axes[i*3+2]});
        modelMat = translate(modelMat,(vec3){0.0f,0.0f,0.5f});

        cubePlane = plane(model * modelMat, color);
        cube.children.push_back(cubePlane);
    }


    std::vector<struct Vertex> vertices;
    std::vector<uint32_t> indices;
    getVertandIndx(&cube, &vertices, &indices);

    for(Vertex v : vertices)
    {
        cube.vertices.push_back(v);
    }
    for(uint32_t i : indices)
    {
        cube.indices.push_back(i);
    }

    return cube;
}

static struct MeshObject checkerPlane(mat4 model,int size, vec3 colour1, vec3 colour2)
{
    struct MeshObject checkerPlane = {};

    mat4 modelMat = model;
    checkerPlane.pObj.M = GLM_MAT4_IDENTITY;

    for(int i = -size; i<size; i++){
        for(int j = -size; j<size; j++){
            vec4 currColour;
            if((i-j)%2 == 0){
                currColour = vec4(colour1,1.0f);
            } else {
                currColour = vec4(colour2,1.0f);
            }

            modelMat = translate(modelMat, (vec3){0.5f+(float)i,0.5f+(float)j,0.0f});

            struct MeshObject p = plane(modelMat, currColour);

            for(uint32_t & ind : p.indices)
            {
                checkerPlane.indices.push_back(ind + checkerPlane.vertices.size());
            }

            for(Vertex & vert : p.vertices)
            {
                checkerPlane.vertices.push_back(vert);
            }

            modelMat = model;
        }
    }

    return  checkerPlane;
}

static vec2 getUVCoord(vec3 normal)
{
    vec2 tex = {};
    tex.x = 0.5f + (atan2(normal.x,normal.z) / (2 * M_PI));
    tex.y = 0.5f - asin(normal.y) / M_PI;

    return tex;
}

static struct MeshObject subdivide_sphere(struct MeshObject* object, int factor)
{
    if(factor == 0)
    {
        return *object;
    }

    struct MeshObject newSphere = {};

    int triOffset = 0;

    std::vector<Vertex> vertList = std::vector<Vertex>();
    std::vector<uint32_t> indList = std::vector<uint32_t>();

    //std::cout<<triangleCount<<std::endl;

    for(int i=0; i<object->indices.size()/3; i++)
    {
        Vertex v0,v1,v2;
        v0 = object->vertices[object->indices[3*i]];
        v1 = object->vertices[object->indices[3*i+1]];
        v2 = object->vertices[object->indices[3*i+2]];

        vec3 u_y = (v1.position-v0.position) * 1.0f/(factor+1.0f);
        vec3 u_x = (v2.position-v0.position) * 1.0f/(factor+1.0f);

        int offset = 0;

        for(int x = 0; x<factor; x++)
        {
            for(int y = 0; y<(factor-x); y++)
            {
                Vertex v;
                v.position = glm::normalize(v0.position + float(y)*u_y + float(x)*u_x);
                v.normal = v.position;
                v.texCoord = getUVCoord(v.normal);
                v.color = v0.color;
                vertList.push_back(v);

                indList.push_back(y+offset + triOffset);
                indList.push_back(y+1+offset + triOffset);
                indList.push_back(y+(factor+2-x)+offset + triOffset);

                indList.push_back(y+(factor+2-x)+offset + triOffset);
                indList.push_back(y+1+offset + triOffset);
                indList.push_back(y+1+(factor+2-x)+offset + triOffset);
            }
            Vertex vEnd0;
            vEnd0.position = glm::normalize(v0.position + float(factor-x)*u_y + float(x)*u_x);
            vEnd0.normal = vEnd0.position;
            vEnd0.texCoord = getUVCoord(vEnd0.normal);
            vEnd0.color = v0.color;
            vertList.push_back(vEnd0);

            Vertex vEnd1;
            vEnd1.position = glm::normalize(v0.position + float(factor+1-x)*u_y + float(x)*u_x);
            vEnd1.normal = vEnd1.position;
            vEnd1.texCoord = getUVCoord(vEnd1.normal);
            vEnd1.color = v0.color;
            vertList.push_back(vEnd1);

            indList.push_back(factor-x + offset + triOffset);
            indList.push_back(factor+1-x + offset + triOffset);
            indList.push_back((factor-x)+(factor+2-x) + offset + triOffset);

            offset += (factor+2)-x;
        }
        Vertex vEnd0;
        vEnd0.position = glm::normalize(v0.position + float(0)*u_y + float(factor)*u_x);
        vEnd0.normal = vEnd0.position;
        vEnd0.texCoord = getUVCoord(vEnd0.normal);
        vEnd0.color = v0.color;
        vertList.push_back(vEnd0);

        Vertex vEnd1;
        vEnd1.position = glm::normalize(v0.position + float(1)*u_y + float(factor)*u_x);
        vEnd1.normal = vEnd1.position;
        vEnd1.texCoord = getUVCoord(vEnd1.normal);
        vEnd1.color = v0.color;
        vertList.push_back(vEnd1);

        Vertex vEndEnd;
        vEndEnd.position = glm::normalize(v0.position + float(0)*u_y + float(factor+1)*u_x);
        vEndEnd.normal = vEndEnd.position;
        vEndEnd.texCoord = getUVCoord(vEnd1.normal);
        vEndEnd.color = v0.color;
        vertList.push_back(vEndEnd);

        //last triangle
        indList.push_back(offset + triOffset);
        indList.push_back(offset+1 + triOffset);
        indList.push_back(offset+2 + triOffset);

        triOffset += ((factor+3)*(factor+2))/2;
    }
    
    newSphere.vertices = vertList;
    newSphere.indices = indList;

    return newSphere;
}

static struct MeshObject icosahedron(mat4 model, vec3 color, int subdivisions)
{
    struct MeshObject icoObj = {};

    float phi = (1 + sqrt(5.0f)) / 2.0f;
    Vertex v0{}, v1{}, v2{}, v3{}, v4{}, v5{}, v6{}, v7{}, v8{}, v9{}, v10{}, v11{};

    v0.position = {-1.0f,  phi, 0.0f}; v0.color = color; v0.normal = v0.position;
    v1.position = { 1.0f,  phi, 0.0f}; v1.color = color; v1.normal = v1.position;
    v2.position = {-1.0f, -phi, 0.0f}; v2.color = color; v2.normal = v2.position;
    v3.position = { 1.0f, -phi, 0.0f}; v3.color = color; v3.normal = v3.position;

    v4.position = {0.0f, -1.0f,  phi}; v4.color = color; v4.normal = v4.position;
    v5.position = {0.0f,  1.0f,  phi}; v5.color = color; v5.normal = v5.position;
    v6.position = {0.0f, -1.0f, -phi}; v6.color = color; v6.normal = v6.position;
    v7.position = {0.0f,  1.0f, -phi}; v7.color = color; v7.normal = v7.position;

    v8.position =  { phi, 0.0f, -1.0f};  v8.color = color; v8.normal = v8.position;
    v9.position =  { phi, 0.0f,  1.0f};  v9.color = color; v9.normal = v9.position;
    v10.position = {-phi, 0.0f, -1.0f}; v10.color = color; v10.normal = v10.position;
    v11.position = {-phi, 0.0f,  1.0f}; v11.color = color; v11.normal = v11.position;

    icoObj.vertices = {v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11};

    icoObj.indices =  {0, 5,  11,
                0,  1,  5,
                0,  7,  1,
                0,  10, 7,
                0, 11, 10,

                1,  9,  5,
                5, 4,  11,
                11, 2,  10,
                10,  6,  7,
                7,  8,  1,

                3,  4,  9,
                3,  2,  4,
                3,  6,  2,
                3,  8,  6,
                3,  9,  8,

                4,  5,  9,
                2,  11, 4,
                6,  10, 2,
                8,  7,  6,
                9,  1,  8};

    if(subdivisions == 0)
    {
        transformVert(&icoObj, scale(model, vec3(1.0f/sqrt(1+phi*phi)))); //scale down the icosahedron
        return icoObj;
    }


    icoObj = subdivide_sphere(&icoObj, subdivisions);

    transformVert(&icoObj, model);

    return icoObj;

    //for(int h = 0; h<vertices.size(); h++){
    //    std::cout<<"["<<vertices[h].position.x<<","<<vertices[h].position.y<<","<<vertices[h].position.z<<"]"<<std::endl;
    //}
}

static struct MeshObject ObjImporter(const std::string& fileName, glm::vec4 colour){
    std::ifstream myfile("objects/"+ fileName + ".obj");
    std::string data;

    struct MeshObject obj{};

    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texture;

    std::vector<int> normInd;

    getline(myfile, data);

    int numslash,numspace;

    int counter = 0;

    while(!data.empty()){
        Vertex v0{};
        if(data[0] == 'v' && data[1] == ' '){
            std::string posNum;
            std::vector<float> posList;
            for(int i=2; i<data.size(); i++){
                posNum.push_back(data[i]);
                if(data[i] == ' ' || i==data.size()-1){
                    posNum.pop_back();
                    posList.push_back(std::stof(posNum));
                    posNum.clear();
                }
            }
            positions.push_back(glm::vec3(posList[0],posList[1]+1.0f,posList[2]));
        }

        if(data[0] == 'v' && data[1] == 'n'){
            std::string normNum;
            std::vector<float> normList;
            for(int i=3; i<data.size(); i++){
                normNum.push_back(data[i]);
                if(data[i] == ' ' || i==data.size()-1){
                    normNum.pop_back();
                    normList.push_back(std::stof(normNum));
                    normNum.clear();
                }
            }
            normals.push_back(glm::vec3(normList[0],normList[1],normList[2]));
        }

        if(data[0] == 'f'){
            int posInd1,posInd2,posInd3;
            int normInd1,normInd2,normInd3;
            std::string num;
            numslash = 0;
            numspace = 0;
            for(int i = 2; i<data.size(); i++){

                num.push_back(data[i]);

                //std::cout<<"data[i]: "<<data[i]<<std::endl;

                if(data[i] == '/'){
                    numslash++;
                    num.pop_back();
                    if(numslash == 1){
                        //std::cout<<num<<std::endl;
                        posInd1 = std::stoi(num);
                    } else if( numslash == 3){
                        //std::cout<<num<<std::endl;
                        posInd2 = std::stoi(num);
                    } else if( numslash == 5){
                        //std::cout<<num<<std::endl;
                        posInd3 = std::stoi(num);
                    }

                    num.clear();
                }

                if(data[i] == ' '){
                    numspace++;
                    num.pop_back();
                    if(numspace == 1){
                        //std::cout<<num<<std::endl;
                        normInd1 = std::stoi(num);
                    } else if( numspace == 2){
                        //std::cout<<num<<std::endl;
                        normInd2 = std::stoi(num);
                    }
                    num.clear();

                }

                if (i == (data.size()-1)){
                    //std::cout<<num<<std::endl;
                    normInd3 = std::stoi(num);
                    num.clear();
                }

            }
            num.clear();

            Vertex v0,v1,v2;

            obj.indices.push_back(counter);
            obj.indices.push_back(counter+1);
            obj.indices.push_back(counter+2);

            counter += 3;

            v0.position = positions[posInd1-1];
            v1.position = positions[posInd2-1];
            v2.position = positions[posInd3-1];

            v0.normal = normals[normInd1-1];
            v1.normal = normals[normInd2-1];
            v2.normal = normals[normInd3-1];

            v0.color = colour;
            v1.color = colour;
            v2.color = colour;

            obj.vertices.push_back(v0);
            obj.vertices.push_back(v1);
            obj.vertices.push_back(v2);

        }

        getline(myfile, data);
    }

    myfile.close();

    return obj;
}


#endif //VULKANENGINE2_SHAPES_H
