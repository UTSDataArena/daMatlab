#include "../MatlabModule.h"

MatlabModule::~MatlabModule() {
    stopDataReader();
    pthread_mutex_destroy(&m_mutex);
}

MatlabModule* MatlabModule::createAndInitialize() {
    MatlabModule * instance = new MatlabModule();
    omega::ModuleServices::addModule(instance);
    instance->doInitialize(omega::Engine::instance());
    return instance;
}

MatlabModule::MatlabModule(): omega::EngineModule("MatlabModule"), m_sceneManager(NULL) {
}


void MatlabModule::printSceneNodes(const omega::Node * node) {
    if(node == NULL){
        return;
    }
    std::list< omega::Node * > children = node->getChildren();
    for (std::list<omega::Node*>::iterator it=children.begin(); it != children.end(); ++it){
        std::cout << "MatlabModule: Node " << node->getName() << " has " << node->numChildren() << " child(ren)" << std::endl; 
        std::cout << "MatlabModule: Node's child: " << (*it)->getName() << std::endl;
        printSceneNodes(*it);
    }
}

omega::Node * MatlabModule::getCurrentNode() const {
    
    if(m_matlabGeometry != NULL) 
        return omega::Engine::instance()->getScene()->getChild(m_matlabGeometry->getName());    
    
    return omega::Engine::instance()->getScene();
}

cyclops::StaticObject * MatlabModule::getStaticObject() const {
    return m_staticObject.get();
}

cyclops::SceneManager * MatlabModule::getSceneManager() const {
    return m_sceneManager.get();
}

void MatlabModule::stopDataReader(){
    
    if(!m_readerIsFin){
        pthread_mutex_unlock(&m_mutex);
        m_reader.join();
        std::cout << "MatlabModule: DataReader stopped " << std::endl;
        m_readerIsFin = true;
    }
}

void MatlabModule::addNewGeometry() {
    
    static unsigned int round = 0; 
    
    if( !m_queue.isEmpty()) {
        
        m_matlabGeometry = m_queue.pop();
        
        if(round>0){
            //m_matlabGeometry->clear();
            //omega::Engine::instance()->getScene()->removeChild(m_matlabGeometry->getName());
            omega::Engine::instance()->getScene()->removeAllChildren();
            omega::Engine::instance()->getScene()->addChild(omega::Engine::instance()->getDefaultCamera());
            //m_sceneManager->unload();      
        }
        
        m_sceneManager->addModel(m_matlabGeometry);  
        m_staticObject = new cyclops::StaticObject(m_sceneManager, m_matlabGeometry->getName());
        m_staticObject->setName(m_matlabGeometry->getName());
        
        omega::Ref<omega::Camera> camera = omega::Engine::instance()->getDefaultCamera();
        camera->setPosition(m_matlabGeometry->getCameraPos());
        camera->lookAt(m_staticObject->getBoundCenter(), m_matlabGeometry->getUpVector());   
        
        printSceneNodes(omega::Engine::instance()->getScene());
        std::cout << "-------------------------- MatlabModule: Done " << round << " --------------------------" << std::endl;
        round++;
    }
}

void MatlabModule::initialize() {
    
    m_port = 30000;
    m_ip_address = "127.0.0.1";
    
    m_sceneManager = cyclops::SceneManager::createAndInitialize();
    m_queue = GeometryQueue();
    
    // create DataReader thread
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutex_lock(&m_mutex);
    m_reader = DataReader(m_port, m_ip_address, &m_queue, &m_mutex);
    m_readerIsFin = false;
    m_reader.start();
}


// void MatlabModule::update(const omega::UpdateContext& context){
//     EngineModule::update(context);
//     //         if(m_queue.size() > 0){
//     //             addNewGeometry();
//     //         }
//     addNewGeometry();
// }







