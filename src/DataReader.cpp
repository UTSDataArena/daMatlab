#include "../DataReader.h"

const string DataReader::H_MODEL_NAME = "H_MODEL_NAME";


const string DataReader::H_CAMPOS = "H_CAMPOS";
const string DataReader::H_CAMUP = "H_CAMUP"; 

const string DataReader::H_TYPE = "H_TYPE";



const string DataReader::D_VERTEX = "D_VERTEX";
const string DataReader::D_VERTEX_NORMAL = "D_VERTEX_NORMAL";
const string DataReader::D_FACE_NORMAL = "D_FACE_NORMAL";
const string DataReader::D_FACE = "D_FACE";   
const string DataReader::D_COLOR = "D_COLOR";

const string DataReader::H_CLEAR = "H_CLEAR";
const string DataReader::H_CREATE = "H_CREATE";
const string DataReader::H_ADD = "H_ADD";



DataReader::DataReader() : mThread::Thread(){}

DataReader::DataReader(GeometryQueue * queue, pthread_mutex_t * mutex) : mThread::Thread(){
    m_mutex = mutex;
    m_queue = queue;
    m_connection = Connection();
    m_connection.initializeConnection();
    
    m_state = HEADER;
    m_type = VERTEX;
    m_create = true;
    
    
}

DataReader::~DataReader() {
    //pthread_mutex_destroy(m_mutex);
}

void DataReader::run(){
    std::cout << "DataReader: Start Running" << std::endl;
    
    while(pthread_mutex_trylock(m_mutex) != 0){ //while(mutex_is_locked)
        receiveData();
    }
    pthread_mutex_unlock(m_mutex);
    m_connection.closeConnection();
}


void DataReader::readData(const string & data, const unsigned int & count){
    
    static unsigned int counter = 0;
    float coordinates[3] = {0};  
    float color[4] = {0};   
    
    switch(m_type)
    {     
        
        case VERTEX:
            parseData(data, coordinates);
            m_matlabGeometry->addVertex(coordinates);
            break;
        case FACE:
            parseData(data, coordinates);
            m_matlabGeometry->addFace(coordinates);
            break;
        case COLOR:
            parseData(data, color);
            m_matlabGeometry->addColor(color);
            break;
        case VERTEX_NORMAL:
            parseData(data, coordinates);
            m_matlabGeometry->addVertexNormal(coordinates);
            break;
        case FACE_NORMAL:
            parseData(data, coordinates);
            m_matlabGeometry->addFaceNormal(coordinates);
            break;
    }
    
    counter++;
    
    if(counter == count){
        
        switch(m_type)
        {
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


void DataReader::readHeader(const string & data, unsigned int & count){
    
    size_t i = data.find_first_of(":", 0);
    string flag = data.substr(0, i);
    string value = data.substr(i+1, data.length());
    
    static const string header_flag = "H_";
    static const string data_flag = "D_";
    
    std::cout << "DataReader: flag: " << flag << "    value: " << value << std::endl;
    
    if(!flag.compare(0, header_flag.length(), header_flag)){
        
        if(!H_MODEL_NAME.compare(flag)){
            if(m_create){
                m_matlabGeometry = MatlabGeometry::create(value);
                m_create = false;
            }   
        } else if(!H_TYPE.compare(flag)){
            m_matlabGeometry->setPrimitiveType(value);
        } else if (!H_CAMPOS.compare(flag)){
            static float campos[3] = {0};
            parseData(value, campos);
            m_matlabGeometry->setCameraPos(campos);
        } else if (!H_CAMUP.compare(flag)){
            static float camup[3] = {0};
            parseData( value, camup);
            m_matlabGeometry->setUpVector(camup);
        }  else if (!H_CLEAR.compare(flag)) {
            std::cout << "DataReader: Geometry was cleared -- Please create new geometry" << std::endl; 
            m_matlabGeometry->clear();
        } else if(!H_ADD.compare(flag)){
            
            if(!m_matlabGeometry->checkAndAdd()){ //geometry is invalid
                std::cout << "DataReader: Geometry is invalid -- Please create new geometry" << std::endl; 
                m_matlabGeometry->clear();
            } else {
                
                if(m_queue->push(m_matlabGeometry)){
                    std::cout << "DataReader: Added geometry " << m_queue->back()->getName() << " to queue" << std::endl;
                    m_create =true;
                } else {
                    std::cout << "DataReader: Geometry queue reached max size -- Please remove geometry from queue and send F_ADD again" << std::endl;
                }
            }
            
        } else if(!H_CREATE.compare(flag)){
            
            if(!m_matlabGeometry->checkAndCreate()){ //geometry is invalid
                std::cout << "DataReader: Geometry is invalid -- Please create new geometry" << std::endl; 
                m_matlabGeometry->clear();
            }
        } 
        
        
    } else if (!flag.compare(0, data_flag.length(), data_flag)){
        
        
        count = atof( value.c_str() );
        
        
        if(!D_VERTEX.compare(flag)){
            m_type = VERTEX;
        } else if (!D_FACE.compare(flag)){
            m_type = FACE;
        } else if (!D_COLOR.compare(flag)){
            m_type = COLOR;
        } else if (!D_VERTEX_NORMAL.compare(flag)){
            m_type = VERTEX_NORMAL;
        } else if (!D_FACE_NORMAL.compare(flag)){
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
        string data = string(buffer);
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


void DataReader::parseData(const string & data, float vector[]){
    
    size_t i = data.find_first_not_of(" ", 0);
    size_t found = 0;                
    unsigned int pos = 0;
    while ( (found = data.find_first_of(" ", i)) != string::npos)
    { 
        vector[pos] = atof( data.substr (i, (found-i)).c_str());
        found = data.find_first_not_of(" ", found);
        i = found;
        pos++;
    }
    vector[pos] = atof( data.substr (i, (data.length()-i)).c_str());
}

