#include "MatlabGeometry.h"

const string MatlabGeometry::TYPE_POINTS = "POINTS";
const string MatlabGeometry::TYPE_TRIANGLES = "TRIANGLES";


MatlabGeometry * MatlabGeometry::create(const omicron::String & name){
    return new MatlabGeometry(name);
}

MatlabGeometry::MatlabGeometry(const omicron::String& name): cyclops::ModelGeometry(name){ 
    
    m_vertices = new osg::Vec3Array();
    m_faces = new osg::Vec3iArray();
    m_vertexNormals = new osg::Vec3Array();
    myVertexNormals = new osg::Vec3Array();
    myFaceNormals = new osg::Vec3Array();
    myColors = new osg::Vec4Array();
    
    myGeometry->setNormalArray(myFaceNormals, osg::Array::BIND_PER_VERTEX);
    myGeometry->setNormalArray(myVertexNormals, osg::Array::BIND_PER_VERTEX);
    myGeometry->setColorArray(myColors, osg::Array::BIND_PER_VERTEX);
    
    m_startIdx = 0;
    m_numVertices = 0;
    
    m_recvVertices =false;
    m_recvVertexNormals =false;
    
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
        m_recvVertices = true;
    }
    
    // if we have already received the vertex normals, we can calculate the face vertex normals and add them to the geode
    if(m_vertexNormals->size()>0){            
        myVertexNormals->push_back(m_vertexNormals->at(face[0]-1));
        myVertexNormals->push_back(m_vertexNormals->at(face[1]-1));
        myVertexNormals->push_back(m_vertexNormals->at(face[2]-1));
        m_recvVertexNormals = true;
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
    myColors->push_back(osg::Vec4f(color[0], color[1], color[2], color[3]));
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

bool MatlabGeometry::checkAndCreate() {
    
    if (osg::PrimitiveSet::TRIANGLES == m_type){
        
        if(m_faces->size()==0 || m_vertices->size()==0){
            std::cerr << "MatlabGeometry: TRIANGLES must have faces and vertices" << std::endl;
            return false;   
        }
        
        if(!m_recvVertices){
            for(int i = 0; i < m_faces->size() ; i++){
                osg::Vec3i v = m_faces->at(i);   
                myVertices->push_back(m_vertices->at(v[0]));
                myVertices->push_back(m_vertices->at(v[1]));
                myVertices->push_back(m_vertices->at(v[2]));
            }
            
        }
        
        if(m_vertexNormals->size()>0){
            
            if(m_vertexNormals->size() != m_vertices->size()){
                std::cerr << "MatlabGeometry: Vertex normals must have same size as vertices" << std::endl;
                return false;
            }
            
            if(!m_recvVertexNormals){
                for(int i = 0; i < m_faces->size(); i++){
                    osg::Vec3i v = m_faces->at(i);   
                    myVertexNormals->push_back(m_vertexNormals->at(v[0]));
                    myVertexNormals->push_back(m_vertexNormals->at(v[1]));
                    myVertexNormals->push_back(m_vertexNormals->at(v[2]));
                }
            }
        }
    }    
    
    m_numVertices = myVertices->size() - m_startIdx; 
    myGeometry->addPrimitiveSet(new osg::DrawArrays(m_type, m_startIdx, m_numVertices));
    m_startIdx = m_numVertices;
    
    myGeometry->dirtyBound();
    
    m_faces->clear();   
    m_vertices->clear();
    m_vertexNormals->clear();    
    
    m_recvVertices =false;
    m_recvVertexNormals =false;
    
    return true;
    
}



bool MatlabGeometry::checkAndAdd() {
    
    if (myFaceNormals->size()>0){
        if(myFaceNormals->size() != myVertices->size()){
            std::cerr << "MatlabGeometry: Face normals must have same size as vertices" << std::endl;
            return false;
        }
    }
    
    if (myVertexNormals->size()>0){
        if(myVertexNormals->size() != myVertices->size()){
            std::cerr << "MatlabGeometry: Vertex normals must have same size as vertices" << std::endl;
            return false;
        }
    }
    
    if(myColors->size()>0){
        if(myColors->size() != myVertices->size()){
            std::cerr << "MatlabGeometry: Colors must have same size as vertices" << std::endl;
            return false;
        }
    }
    
    
    
    m_numVertices = myVertices->size() - m_startIdx; 
    myGeometry->addPrimitiveSet(new osg::DrawArrays(m_type, m_startIdx, m_numVertices));
    m_startIdx = m_numVertices;
    
    myGeometry->dirtyBound();
    
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
    m_faces->clear();
    m_vertices->clear();
    m_vertexNormals->clear();
    myFaceNormals->clear();
    myVertexNormals->clear();
    
    myGeometry->removePrimitiveSet(0, myGeometry->getNumPrimitiveSets());
    myGeometry->dirtyBound();
    
    m_recvVertices =false;
    m_recvVertexNormals =false;
    
    m_startIdx = 0;
    m_numVertices = 0;
    
    m_campos = omicron::Vector3f(0, 0, 0);
    m_camup = omicron::Vector3f(0, 0, 1);
}


