%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% TEAPOT %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% The data for vertices and faces of the teapot object can be downloaded
% here: graphics.stanford.edu/courses/cs148-10-summer/as3/code/as3/teapot.obj

% To read an obj file you can download an external toolbox, e.g.:
% gptoolbox: Geometry Processing Toolbox, Alec Jacobson and others, 2016, http://github.com/alecjacobson/gptoolbox

path = 'external/gptoolbox-master/mesh/'; % path to the toolbox
addpath(path);
[vertices,faces] = readOBJ('teapot.obj');


%%% POINT CLOUD %%%%%%
ptCloud = pointCloud(vertices);

omgpt = Omegalib('teapot', 'POINTS');
omgpt.setNormals('VertexNormals', pcnormals(ptCloud));
omgpt.plotFigure(1, @pcshow, ptCloud);

%%% TRIANGLES %%%%%%%%

TR = triangulation(faces,vertices);
vertex_normals = vertexNormal(TR);
%idx = [1 3 2]; % change to counter clockwise
%vertex_normals = vertex_normals(:, idx);
%face_normal = faceNormal(TR);
%face_normal = face_normal(:, idx);

omgtri = Omegalib('teapot', 'TRIANGLES');
omgtri.setNormals('VertexNormals', vertex_normals);
omgtri.plotFigure(2, @trisurf, faces, vertices(:,1) ,vertices(:,2),vertices(:,3));
