%%%%%%%%%% POINT CLOUD %%%%%%%%%%%%%%

ptCloud = pcread('teapot.ply');
omgpt = Omegalib('teapot', 'POINTS');
omgpt.setNormals('VertexNormals', pcnormals(ptCloud));
omgpt.plotFigure(1, @pcshow, ptCloud);

%%%%%%%%% 2D DATA TRIANGLES %%%%%%%%%%%%%%%%%

verts = gallery('uniformdata',10,2,2);
faces = delaunay(verts);
x = verts(:,1);
y = verts(:,2);

z1 = zeros(size(x));
omg2d=Omegalib('uniform', 'TRIANGLES');
omg2d.plotFigure(2, @trisurf, faces, x, y, z1);

z2 = ones(size(x)) * 2;
omg2d_ = Omegalib('uniform', 'TRIANGLES');
omg2d_.plotFigure(3, @trisurf, faces, x, y, z2);

%%%%%%%%%%%% SURFACE QUADS %%%%%%%%%%%%%%%%%%%

[x,y] = meshgrid(1:15,1:15);
z = peaks(15);

omg=Omegalib('mesh', 'QUADS');
omg.plotFigure(4, @surf, x, y, z);

%%%%%%%%%%% TEAPOT TRIANGLES %%%%%%%%%%%%%%%

% teapot.obj: graphics.stanford.edu/courses/cs148-10-summer/as3/code/as3/teapot.obj
% gptoolbox: Geometry Processing Toolbox, Alec Jacobson and others, 2016, http://github.com/alecjacobson/gptoolbox

path = '/da/dev/lara/omegalib/modules/templateModule/matlab/external/gptoolbox-master/mesh/';
addpath(path);
[verts,faces] = readOBJ('/da/dev/lara/omegalib/modules/templateModule/matlab/teapot.obj');

TR = triangulation(faces,verts);
vertex_normal = vertexNormal(TR);
idx = [1 3 2]; % change to counter clockwise
vertex_normal = vertex_normal(:, idx);
%face_normal = faceNormal(TR);
%face_normal = face_normal(:, idx);

omgtea = Omegalib('teapot', 'TRIANGLES');
omgtea.plotFigure(5, @trisurf, faces, verts(:,1) ,verts(:,2),verts(:,3), 'VertexNormals', vertex_normal);
