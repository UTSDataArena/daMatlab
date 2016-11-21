#include "../DataReader.h"

const std::string DataReader::Flags::H_MODEL_NAME = "H_MODEL_NAME";
const std::string DataReader::Flags::H_CAM_POS = "H_CAM_POS";
const std::string DataReader::Flags::H_CAM_UP = "H_CAM_UP";
const std::string DataReader::Flags::H_TYPE = "H_TYPE";
const std::string DataReader::Flags::D_VERTEX = "D_VERTEX";
const std::string DataReader::Flags::D_VERTEX_NORMAL = "D_VERTEX_NORMAL";
const std::string DataReader::Flags::D_FACE_NORMAL = "D_FACE_NORMAL";
const std::string DataReader::Flags::D_FACE = "D_FACE";   
const std::string DataReader::Flags::D_COLOR = "D_COLOR";
const std::string DataReader::Flags::H_CLEAR = "H_CLEAR";
const std::string DataReader::Flags::H_NEXT = "H_NEXT";
const std::string DataReader::Flags::H_ADD = "H_ADD";

DataReader::DataReader() : mThread::Thread(){}

DataReader::DataReader(const unsigned int port, const std::string & ip_address, GeometryQueue * queue, pthread_mutex_t * mutex) : mThread::Thread(){
    m_mutex = mutex;
    m_queue = queue;
    m_connection = Connection(port, ip_address);
    m_connection.initializeConnection();
    
    m_state = HEADER;
    m_type = VERTEX;
    m_create = true;
}

DataReader::~DataReader() {
    //pthread_mutex_destroy(m_mutex); //destroyed in main
}

void DataReader::run(){
    std::cout << "DataReader: Start Running" << std::endl;
    
    while(pthread_mutex_trylock(m_mutex) != 0){ //while(mutex_is_locked)
        receiveData();
    }
    pthread_mutex_unlock(m_mutex);
    m_connection.closeConnection();
}


void DataReader::readData(const std::string & data, const unsigned int & count){
    
    static unsigned int counter = 0;
    float coordinates[4] = {0};  
    
    parseData(data, coordinates);
    
    switch(m_type) {     
        case VERTEX:
            m_matlabGeometry->setVertex(coordinates);
            break;
        case FACE:
            m_matlabGeometry->setFace(coordinates);
            break;
        case COLOR:
            m_matlabGeometry->setColor(coordinates);
            break;
        case VERTEX_NORMAL:
            m_matlabGeometry->setVertexNormal(coordinates);
            break;
        case FACE_NORMAL:
            m_matlabGeometry->setFaceNormal(coordinates);
            break;
    }
    
    counter++;
    
    if(counter == count){
        
        switch(m_type) {
            case VERTEX:
                std::cout << "DataReader: Received " << counter << " vertices" << std::endl;
                break;
            case FACE:
                std::cout << "DataReader: Received " << counter << " faces" << std::endl;
                break;
            case COLOR:
                std::cout << "DataReader: Received " << counter << " color vectors" << std::endl;
                break;
            case VERTEX_NORMAL:
                std::cout << "DataReader: Received " << counter << " vertex normals" << std::endl;
                break;
            case FACE_NORMAL:
                std::cout << "DataReader: Received " << counter << " face normals" << std::endl;
                break;
        }
        
        m_state = HEADER;
        counter=0;        
    }
    
}


void DataReader::readHeader(const std::string & data, unsigned int & count){
    
    size_t i = data.find_first_of(":", 0);
    std::string flag = data.substr(0, i);
    std::string value = data.substr(i+1, data.length());
    
    static const std::string header_flag = "H_";
    static const std::string data_flag = "D_";
    
    std::cout << "DataReader: flag: " << flag << "    value: " << value << std::endl;
    
    if(!flag.compare(0, header_flag.length(), header_flag)){
        
        if(!Flags::H_MODEL_NAME.compare(flag)){
            if(m_create){
                m_matlabGeometry = MatlabGeometry::create(value);
                m_create = false;
            }   
        } else if(!Flags::H_TYPE.compare(flag)){
            m_matlabGeometry->setPrimitiveType(value);
        } else if (!Flags::H_CAM_POS.compare(flag)){
            static float campos[3] = {0};
            parseData(value, campos);
            m_matlabGeometry->setCameraPos(campos);
        } else if (!Flags::H_CAM_UP.compare(flag)){
            static float camup[3] = {0};
            parseData( value, camup);
            m_matlabGeometry->setUpVector(camup);
        }  else if (!Flags::H_CLEAR.compare(flag)) {
            std::cout << "DataReader: Geometry was cleared -- Please create new geometry" << std::endl; 
            m_matlabGeometry->clear();
        } else if(!Flags::H_ADD.compare(flag)){
            
            if(!m_matlabGeometry->validate()){ //geometry is invalid
                std::cout << "DataReader: Geometry is invalid -- Please create new geometry" << std::endl; 
                m_matlabGeometry->clear();
            } else {
                
                m_matlabGeometry->addPrimitive();
                
                if(m_queue->push(m_matlabGeometry)){
                    std::cout << "DataReader: Added geometry " << m_queue->back()->getName() << " to queue" << std::endl;
                    m_create =true;
                } else {
                    std::cout << "DataReader: Geometry queue reached max size -- Please remove geometry from queue and ADD again" << std::endl;
                }
                
            }
            
        } else if(!Flags::H_NEXT.compare(flag)){
            
            if(!m_matlabGeometry->validate()){ //geometry is invalid
                std::cout << "DataReader: Geometry is invalid -- Please create new geometry" << std::endl; 
                m_matlabGeometry->clear();
            } else {
                m_matlabGeometry->addPrimitive();
            }
            
        } 
        
        
    } else if (!flag.compare(0, data_flag.length(), data_flag)){
        
        
        count = atof( value.c_str() );
        
        
        if(!Flags::D_VERTEX.compare(flag)){
            m_type = VERTEX;
        } else if (!Flags::D_FACE.compare(flag)){
            m_type = FACE;
        } else if (!Flags::D_COLOR.compare(flag)){
            m_type = COLOR;
        } else if (!Flags::D_VERTEX_NORMAL.compare(flag)){
            m_type = VERTEX_NORMAL;
        } else if (!Flags::D_FACE_NORMAL.compare(flag)){
            m_type = FACE_NORMAL;
        }
        
        m_state = DATA;
    }
    
    
}


void DataReader::receiveData(){
    
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));
    
    static unsigned int count = 0;
    
    int recvlen = m_connection.read(buffer, sizeof(buffer));
    
    if (recvlen > 0) {
        std::string data = std::string(buffer);
        switch(m_state){       
            case HEADER:
                readHeader(data, count);
                break;
            case DATA:
                readData(data, count);
                break;
        }
    }
}


void DataReader::parseData(const std::string & data, float vector[]){
    
    size_t i = data.find_first_not_of(" ", 0);
    size_t found = 0;                
    unsigned int pos = 0;
    while ( (found = data.find_first_of(" ", i)) != std::string::npos) { 
        vector[pos] = atof( data.substr (i, (found-i)).c_str());
        found = data.find_first_not_of(" ", found);
        i = found;
        pos++;
    }
    vector[pos] = atof( data.substr (i, (data.length()-i)).c_str());
}

