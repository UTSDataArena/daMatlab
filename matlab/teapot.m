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
setNormals(omgpt, 'VertexNormals', pcnormals(ptCloud));
plotFigure(omgpt, 1, @pcshow, ptCloud );

%%% TRIANGLES %%%%%%%%
TR = triangulation(faces,vertices);
vertex_normals = vertexNormal(TR);
omgtri = Omegalib('teapot', 'TRIANGLES');
setNormals(teaTri, 'VertexNormals', vertex_normals);
%plotFigure(omgtri, 2, @trisurf, faces, vertices(:,1) ,vertices(:,2),vertices(:,3), 'FaceVertexCData', [1 0 0], 'EdgeAlpha', 'interp', 'FaceVertexAlphaData', bsxfun(@times, ones(size(vertices, 1), 1), 0.6));
plotFigure(omgtri, 2, @trisurf, faces, vertices(:,1) ,vertices(:,2),vertices(:,3));