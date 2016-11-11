#ifndef MATLAB_GEOMETRY_H
#define MATLAB_GEOMETRY_H

#include <cyclops/cyclops.h>
#include <omega.h>

class MatlabGeometry : public cyclops::ModelGeometry
{
    
public:
    
    static MatlabGeometry * create(const omicron::String & name);
    MatlabGeometry(const omicron::String & name);
    
    void addVertex(const float vertex[3]);
    void addFace(const float face[3]);
    
    void addVertexNormal(const float vertexNormal[3]);
    void addFaceNormal(const float faceNormal[3]);
    
    void addColor(const float color[4]);
    
    void setPrimitiveType(const std::string & type);
    
    void setCameraPos(const float cameraPos[3]);
    void setUpVector(const float upVector[3]);
    
    omicron::Vector3f getCameraPos() const;
    omicron::Vector3f getUpVector() const;
    
    void clear();
    bool checkAndCreate();  
    bool checkAndAdd();
    
    
private:
    
    static const string TYPE_POINTS;
    static const string TYPE_TRIANGLES;
    
    omicron::Vector3f m_campos;
    omicron::Vector3f m_camup;
    
    /* vertex list for triangles */
    omega::Ref<osg::Vec3Array> m_vertices;
    /* vertex normals for triangles */
    omega::Ref<osg::Vec3Array> m_vertexNormals;
    /* face list for triangles */
    omega::Ref<osg::Vec3iArray> m_faces;
    
    /* Face normal list added to the geode */
    omega::Ref<osg::Vec3Array> myFaceNormals;
    /* Vertex normal list added to the geode */
    omega::Ref<osg::Vec3Array> myVertexNormals;
    
    osg::PrimitiveSet::Mode m_type;    
    
    bool m_recvVertices;
    bool m_recvVertexNormals;
    
    int m_numVertices;
    int m_startIdx;
    
};

#endif
