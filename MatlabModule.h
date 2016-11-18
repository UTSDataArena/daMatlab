#include <omega.h>
#include <cyclops/cyclops.h>
#include <omegaToolkit.h>
#include <osg/Geometry>
#include <pthread.h>
#include "MatlabGeometry.h"
#include "Thread.h"
#include "Connection.h"
#include "DataReader.h"
#include "GeometryQueue.h"



class MatlabModule: public omega::EngineModule
{
public:
    
    MatlabModule();
    ~MatlabModule();
    static MatlabModule* createAndInitialize();
    virtual void initialize();
//     void update(const omega::UpdateContext& context);
    omega::Node * getCurrentNode() const;
    cyclops::StaticObject * getStaticObject() const;
    cyclops::SceneManager * getSceneManager() const;
    void addNewGeometry();
    void stopDataReader();
    
    
private:
    
    omega::Ref<cyclops::SceneManager> m_sceneManager;
    omega::Ref<cyclops::StaticObject> m_staticObject;
    omega::Ref<MatlabGeometry> m_matlabGeometry;
    
    GeometryQueue m_queue;
    DataReader m_reader;
    pthread_mutex_t m_mutex;
    bool m_readerIsFin;
    
    unsigned int m_port;
    std::string m_ip_address;
    
    void printSceneNodes(const omega::Node * node);
};


//Python wrapper code.
#ifdef OMEGA_USE_PYTHON
#include "omega/PythonInterpreterWrapper.h"
BOOST_PYTHON_MODULE(daMatlab)
{
    PYAPI_REF_BASE_CLASS(MatlabModule)
    PYAPI_STATIC_REF_GETTER(MatlabModule, createAndInitialize)
    PYAPI_METHOD(MatlabModule, stopDataReader)
    PYAPI_METHOD(MatlabModule, addNewGeometry)
    PYAPI_REF_GETTER(MatlabModule, getCurrentNode)
    PYAPI_REF_GETTER(MatlabModule, getSceneManager)
    PYAPI_REF_GETTER(MatlabModule, getStaticObject)
    ;
}
#endif
