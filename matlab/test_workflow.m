%%%%%%%%%% POINT CLOUD %%%%%%%%%%%%%%

ptCloud = pcread('teapot.ply');
verts = double(ptCloud.Location);
[m,n ] = size(verts);
pointscolor=uint8(zeros(m,3)) ;
pointscolor(:,1)=255;
pointscolor(:,2)=255;
pointscolor(:,3)=51;
color = double([ pointscolor/255 , ones(m,1) * 1 ]);
ptCloud.Color=pointscolor;
vertex_normal = pcnormals(ptCloud);
omgpc = Omegalib('teapot', 'POINTS');
omgpc.setValues('Vertices', verts, 'Color', color, 'VertexNormals', vertex_normal);
omgpc.plotFigure(1, @pcshow, ptCloud);

%%%%%%%%%% 2D DATA TRIANGLES %%%%%%%%%%%%%%%%%

verts = gallery('uniformdata',10,2,2);
faces = delaunay(verts);
x = verts(:,1);
y = verts(:,2);

z1 = zeros(size(x));

color1 = zeros(size(verts,1), 4);
for i = 1:size(verts,1)
    color1(i, :) = [ 0 0 1 1 ];
end
omg2d=Omegalib('uniform', 'TRIANGLES');
omg2d.setValues('Vertices', [verts z1], 'Faces', faces, 'Color', color1);
omg2d.plotFigure(2, @trisurf, faces, x, y, z1);

z2 = ones(size(x)) * 2;

color2 = zeros(size(verts,1), 4);
for i = 1:size(verts,1)
    color2(i, :) = [ 1 0 0 1 ];
end

omg2d_ = Omegalib('uniform', 'TRIANGLES');
omg2d_.setValues('Vertices', [verts z2], 'Faces', faces, 'Color', color2);
omg2d_.plotFigure(3, @trisurf, faces, x, y, z2);


%%%%%%%%%%%% SURFACE TRIANGLES %%%%%%%%%%%%%%%%%%%

[x,y] = meshgrid(1:15,1:15);
z = peaks(15);
verts = [x(:) y(:) z(:)];
faces = delaunay(x,y);
omgsu = Omegalib('teapot', 'TRIANGLES');
omgsu.plotFigure(4, @trisurf, faces, verts(:,1) ,verts(:,2),verts(:,3));


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

s = size(verts, 1);
color = zeros( s, 3);
for i = 1:s
    color(i, :) = [ 0 1 0];
end
alpha = ones(s, 1) * 0.5;

omgtea = Omegalib('teapot', 'TRIANGLES');
omgtea.plotFigure(5, @trisurf, faces, verts(:,1) ,verts(:,2),verts(:,3), 'VertexNormals', vertex_normal, 'FaceVertexCData' , color, 'FaceAlpha', 'interp' , 'FaceVertexAlphaData', alpha, 'AlphaDataMapping', 'none');
%omgtea.plotFigure(5, @trisurf, faces, verts(:,1) ,verts(:,2),verts(:,3), 'VertexNormals', vertex_normal);
