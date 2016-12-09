/******************************************************************************
 * THE OMEGA LIB PROJECT
 *-----------------------------------------------------------------------------
 * Copyright 2010-2015		Electronic Visualization Laboratory, 
 *							University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-----------------------------------------------------------------------------
 * Copyright (c) 2010-2015, Electronic Visualization Laboratory,  
 * University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this 
 * list of conditions and the following disclaimer. Redistributions in binary 
 * form must reproduce the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or other materials 
 * provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE  GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY,  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

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
    void startDataReader(std::string ip_address, unsigned int port);
    void stopDataReader();
    void addNewGeometry();
    
    
private:
    
    omega::Ref<cyclops::SceneManager> m_sceneManager;
    omega::Ref<cyclops::StaticObject> m_staticObject;
    omega::Ref<MatlabGeometry> m_matlabGeometry;
    
    GeometryQueue m_queue;
    DataReader m_reader;
    pthread_mutex_t m_mutex;
    bool m_readerIsFin;
    
    void printSceneNodes(const omega::Node * node);
};


//Python wrapper code.
#ifdef OMEGA_USE_PYTHON
#include "omega/PythonInterpreterWrapper.h"
BOOST_PYTHON_MODULE(daMatlab)
{
    PYAPI_REF_BASE_CLASS(MatlabModule)
    PYAPI_STATIC_REF_GETTER(MatlabModule, createAndInitialize)
    PYAPI_METHOD(MatlabModule, startDataReader)
    PYAPI_METHOD(MatlabModule, stopDataReader)
    PYAPI_METHOD(MatlabModule, addNewGeometry)
    PYAPI_REF_GETTER(MatlabModule, getCurrentNode)
    PYAPI_REF_GETTER(MatlabModule, getSceneManager)
    PYAPI_REF_GETTER(MatlabModule, getStaticObject)
    ;
}
#endif
