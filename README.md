***********************************
* OMEGALIB - daMatlab module ******
***********************************

Module to transfer MATLAB-based geometry into Omegalib. 

The current version of the daMatlab modules provides a transfer of point clouds and 
surfaces (triangles and quads), determined by vertices, faces, vertex or face normals, and color. 
The transmission of the geometry data is implemented by using UDP sockets 
where MATLAB acts as a client and the daMatlab module as a server.
The server is always listening to incoming data. If a valid data set has arrived 
an ModelGeometry object is created and added to a queue. 
As soon as the function addNewGeometry() is called and the queue is not empty, 
a ModelGeometry is retrieved and added to the SceneManager.

More information: http://dataarena.net/all_tutorials/matlab/

