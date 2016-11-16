#include "../MatlabGeometry.h"

const string MatlabGeometry::TYPE_POINTS = "POINTS";
const string MatlabGeometry::TYPE_TRIANGLES = "TRIANGLES";

MatlabGeometry::RECV operator|(MatlabGeometry::RECV a, MatlabGeometry::RECV b) {
    return MatlabGeometry::RECV(static_cast<int>(a)|static_cast<int>(b));
}


MatlabGeometry * MatlabGeometry::create(const omicron::String & name){
    return new MatlabGeometry(name);
}

MatlabGeometry::MatlabGeometry(const omicron::String& name): cyclops::ModelGeometry(name){ 
    
    m_vertices = new osg::Vec3Array();
    m_faces = new osg::Vec3iArray();
    m_vertexNormals = new osg::Vec3Array();
    m_color = new osg::Vec4Array();
    myFaceNormals = new osg::Vec3Array();
    myVertexNormals = new osg::Vec3Array();
    myColors = new osg::Vec4Array();
    
    myGeometry->setNormalArray(myFaceNormals, osg::Array::BIND_PER_VERTEX);
    myGeometry->setNormalArray(myVertexNormals, osg::Array::BIND_PER_VERTEX);
    myGeometry->setColorArray(myColors, osg::Array::BIND_PER_VERTEX);
    
    m_startIdx = 0;
    m_numVertices = 0;
    
    m_recv = RECV_DEF;
    
    m_campos = omicron::Vector3f(0, 0, 0);
    m_camup = omicron::Vector3f(0, 0, 1);
    
}

void MatlabGeometry::addVertex(const float vertex[3]){
    if(osg::PrimitiveSet::POINTS == m_type){   
        myVertices->push_back(osg::Vec3f(vertex[0], vertex[1], vertex[2]));
    } else if ( osg::PrimitiveSet::TRIANGLES == m_type){
        m_vertices->push_back(osg::Vec3f(vertex[0], vertex[1], vertex[2]));
    }
}

void MatlabGeometry::addFace(const float face[3]){   
    
    // matlab indices from 1 to n
    m_faces->push_back(osg::Vec3i(static_cast<int>(face[0]-1), static_cast<int>(face[1]-1), static_cast<int>(face[2]-1)));
    
    // if we have already received the vertices, we can calculate the face vertices and add them to the geode
    if(m_vertices->size()>0){
        myVertices->push_back(m_vertices->at(face[0]-1));
        myVertices->push_back(m_vertices->at(face[1]-1));
        myVertices->push_back(m_vertices->at(face[2]-1));
        m_recv = m_recv | RECV_VERTS;        
    }
    
    // if we have already received the vertex normals, we can calculate the face vertex normals and add them to the geode
    if(m_vertexNormals->size()>0){            
        myVertexNormals->push_back(m_vertexNormals->at(face[0]-1));
        myVertexNormals->push_back(m_vertexNormals->at(face[1]-1));
        myVertexNormals->push_back(m_vertexNormals->at(face[2]-1));
        m_recv = m_recv | RECV_VERTS_NORM;
    }
    
    // if we have already received color
    if(m_color->size()>0){        
        myColors->push_back(m_color->at(face[0]-1));
        myColors->push_back(m_color->at(face[1]-1));
        myColors->push_back(m_color->at(face[2]-1));
        m_recv = m_recv |RECV_COLOR;
    }
    
    
}

void MatlabGeometry::addVertexNormal(const float vertexNormal[3]){    
    
    if (osg::PrimitiveSet::POINTS == m_type){
        myVertexNormals->push_back(osg::Vec3f(vertexNormal[0] , vertexNormal[1], vertexNormal[2]));
    }  else if (osg::PrimitiveSet::TRIANGLES == m_type){
        m_vertexNormals->push_back(osg::Vec3f(vertexNormal[0] , vertexNormal[1], vertexNormal[2]));
    } 
}

void MatlabGeometry::addFaceNormal(const float faceNormal[3]){
    // for each face vertex we have to push the face normal
    myFaceNormals->push_back(osg::Vec3f(faceNormal[0], faceNormal[1], faceNormal[2]));
    myFaceNormals->push_back(osg::Vec3f(faceNormal[0], faceNormal[1], faceNormal[2]));
    myFaceNormals->push_back(osg::Vec3f(faceNormal[0], faceNormal[1], faceNormal[2]));
}

void MatlabGeometry::addColor(const float color[4]){
    
    if(osg::PrimitiveSet::POINTS == m_type){   
        myColors->push_back(osg::Vec4f(color[0], color[1], color[2], color[3]));
    } else if ( osg::PrimitiveSet::TRIANGLES == m_type){
        m_color->push_back(osg::Vec4f(color[0], color[1], color[2], color[3]));
    }
}

void MatlabGeometry::setPrimitiveType(const std::string & type){     
    if( !TYPE_POINTS.compare(type)){        
        m_type = osg::PrimitiveSet::POINTS;        
    } else if ( !TYPE_TRIANGLES.compare(type)){
        m_type = osg::PrimitiveSet::TRIANGLES;
    }
}

void MatlabGeometry::setCameraPos(const float cameraPos[3]){
    m_campos = omicron::Vector3f( cameraPos[0], cameraPos[1], cameraPos[2] );
}

void MatlabGeometry::setUpVector(const float upVector[3]){
    m_camup = omicron::Vector3f( upVector[0], upVector[1], upVector[2] );
}

void MatlabGeometry:: calculateValues(){ 
    
    if(m_faces->size()>0){
        
        if(m_vertices->size()>0){
            
            if(!(m_recv & RECV_VERTS)){
                
                for(int i = 0; i < m_faces->size() ; i++){
                    osg::Vec3i v = m_faces->at(i);   
                    myVertices->push_back(m_vertices->at(v[0]));
                    myVertices->push_back(m_vertices->at(v[1]));
                    myVertices->push_back(m_vertices->at(v[2]));
                }
            }
        }
        
        if(m_vertexNormals->size()>0){
            
            if(!(m_recv & RECV_VERTS_NORM)){
                for(int i = 0; i < m_faces->size(); i++){
                    osg::Vec3i v = m_faces->at(i);   
                    myVertexNormals->push_back(m_vertexNormals->at(v[0]));
                    myVertexNormals->push_back(m_vertexNormals->at(v[1]));
                    myVertexNormals->push_back(m_vertexNormals->at(v[2]));
                }
            }
        }
        
        if(m_color->size() > 0){
            
            if(!(m_recv & RECV_COLOR)){
                for(int i = 0; i < m_faces->size(); i++){
                    osg::Vec3i v = m_faces->at(i);   
                    myColors->push_back(m_color->at(v[0]));
                    myColors->push_back(m_color->at(v[1]));
                    myColors->push_back(m_color->at(v[2]));
                }
            }
        }
        
    }
    
}


bool MatlabGeometry::checkAndAdd() {
    
    calculateValues();
    
    if(myVertices->size() == 0) return false;
    
    if (myFaceNormals->size()>0){
        if(myFaceNormals->size() != myVertices->size()){
            return false;
        }
    }
    
    if (myVertexNormals->size()>0){
        if(myVertexNormals->size() != myVertices->size()){
            return false;
        }
    }
    
    if(myColors->size()>0){
        if(myColors->size() != myVertices->size()){
            return false;
        }
    }
    
    m_numVertices = myVertices->size() - m_startIdx; 
    myGeometry->addPrimitiveSet(new osg::DrawArrays(m_type, m_startIdx, m_numVertices));
    m_startIdx = m_numVertices;
    
    myGeometry->dirtyBound();
    
    m_faces->clear();   
    m_vertices->clear();
    m_vertexNormals->clear();  
    m_color->clear();
    
    m_recv = RECV_DEF;
    
    return true;
}


omicron::Vector3f MatlabGeometry::getCameraPos() const{
    return m_campos;
}

omicron::Vector3f MatlabGeometry::getUpVector() const{
    return m_camup;
}

void MatlabGeometry::clear() {
    
    myVertices->clear();
    myColors->clear();
    m_color->clear();
    m_faces->clear();
    m_vertices->clear();
    m_vertexNormals->clear();
    myFaceNormals->clear();
    myVertexNormals->clear();
    
    myGeometry->removePrimitiveSet(0, myGeometry->getNumPrimitiveSets());
    myGeometry->dirtyBound();
    
    m_recv = RECV_DEF;
    
    m_startIdx = 0;
    m_numVertices = 0;
    
    m_campos = omicron::Vector3f(0, 0, 0);
    m_camup = omicron::Vector3f(0, 0, 1);
}


