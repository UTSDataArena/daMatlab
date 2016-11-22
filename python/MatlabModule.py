from cyclops import *
from daMatlab import *

mm = MatlabModule.createAndInitialize()

scene = mm.getSceneManager()

program = ProgramAsset()
program.name = "MatlabModule"
scene.addProgram(program)

# Update function
def onUpdate(frame, t, dt):
    
    mm.addNewGeometry()

# register the update function
setUpdateFunction(onUpdate)
