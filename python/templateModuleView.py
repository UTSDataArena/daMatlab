from cyclops import *
from templateModule import *
import spin_navigation


mm = MatlabModule.createAndInitialize()

scene = mm.getSceneManager()


#### TRIANGLES ###############
program = ProgramAsset()
program.name = "MatlabModule"
scene.addProgram(program)
################################


###### POINT CLOUD ###############
#pointProgram = ProgramAsset()
#pointProgram.name = "points"
#pointProgram.vertexShaderName = "/da/dev/lara/omegalib/modules/pointCloud/shaders/Sphere.vert"
#pointProgram.fragmentShaderName = "/da/dev/lara/omegalib/modules/pointCloud/shaders/Sphere.frag"
#pointProgram.geometryShaderName = "/da/dev/lara/omegalib/modules/pointCloud/shaders/Sphere.geom"
#pointProgram.geometryOutVertices = 4
#pointProgram.geometryInput = PrimitiveType.Points
#pointProgram.geometryOutput = PrimitiveType.TriangleStrip
#scene.addProgram(pointProgram)
#pointScale = Uniform.create('pointScale', UniformType.Float, 1)
##################################



# Update function
def onUpdate(frame, t, dt):
    
    mm.addNewGeometry()
    
    ##### POINT CLOUD ###############
    #pointCloud = mm.getStaticObject()
    #mat = pointCloud.getMaterial()
    #mat.setProgram(pointProgram.name)
    #mat.attachUniform(pointScale)
    #################################
    
    
    spin_navigation.root = mm.getCurrentNode()
    
    
## register the update function
setUpdateFunction(onUpdate)
    
    
