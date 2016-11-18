#ifndef MATLAB_GEOMETRY_H
#define MATLAB_GEOMETRY_H

#include <cyclops/cyclops.h>
#include <omega.h>

class MatlabGeometry : public cyclops::ModelGeometry {
    
public:
    
    static MatlabGeometry * create(const omicron::String & name);
    MatlabGeometry(const omicron::String & name);
    
    void setVertex(const float vertex[3]);
    void setFace(const float face[3]);
    void setVertexNormal(const float vertexNormal[3]);
    void setFaceNormal(const float faceNormal[3]);
    void setColor(const float color[4]);
    void setPrimitiveType(const std::string & type);
    
    void setCameraPos(const float cameraPos[3]);
    void setUpVector(const float upVector[3]);
    
    omicron::Vector3f getCameraPos() const;
    omicron::Vector3f getUpVector() const;
    
    void clear();
    bool validate();
    void addPrimitive();
    
    
    
private:
    
    static const string TYPE_POINTS;
    static const string TYPE_TRIANGLES;
    
    omicron::Vector3f m_camPos;
    omicron::Vector3f m_camUp;
    
    /* vertex list for triangles */
    omega::Ref<osg::Vec3Array> m_vertices;
    /* vertex normals for triangles */
    omega::Ref<osg::Vec3Array> m_vertexNormals;
    /* face list for triangles */
    omega::Ref<osg::Vec3iArray> m_faces;
    
    omega::Ref<osg::Vec4Array> m_color;
    
    /* Face normal list added to the geode */
    omega::Ref<osg::Vec3Array> myFaceNormals;
    /* Vertex normal list added to the geode */
    omega::Ref<osg::Vec3Array> myVertexNormals;
    
    osg::PrimitiveSet::Mode m_type;
    
    int m_numVertices;
    int m_startIdx;
    
    enum RECV {RECV_DEF = 0x00, RECV_VERTS = 0x01, RECV_VERTS_NORM = 0x02, RECV_COLOR = 0x04} m_recv;
    
    void addValuesToGeode();
    
    friend MatlabGeometry::RECV operator|(MatlabGeometry::RECV a, MatlabGeometry::RECV b);
    
};



#endif
