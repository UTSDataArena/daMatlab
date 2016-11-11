% %%%%%%%%%% POINT CLOUD %%%%%%%%%%%%%%
% ptCloud = pcread('teapot.ply');
% verts = ptCloud.Location;
% [m,n ] = size(verts);
% verts = double(verts);
% color = zeros(m, 4);
% for i = 1:m
%     color(i, :) = [ 0 0 255 0 ];
% end
% vertex_normal = pcnormals(ptCloud);
% omgpc = Omegalib;
% omgpc.setValues('teapot', 'POINTS', verts, 'Color', color, 'Vertex Normal', vertex_normal);
% omgpc.plotFigure(1, @pcshow, ptCloud);
% 
% %%%%%%%%%% 2D DATA TRIANGLES %%%%%%%%%%%%%%%%%
% 
% verts = gallery('uniformdata',10,2,2);
% faces = delaunay(verts);
% x = verts(:,1);
% y = verts(:,2);
% 
% z1 = zeros(size(x));
% color1 = zeros( size(faces, 1) * 3, 4);
% for i = 1:size(faces, 1) * 3
%     color1(i, :) = [ 0 0 1 0 ];
% end
% omg2d=Omegalib;
% omg2d.setValues('uniform', 'TRIANGLES', [verts z1], 'Face', faces, 'Color', color1);
% omg2d.plotFigure(2, @trisurf, faces, x, y, z1);
% 
% z2 = ones(size(x)) * 2;
% color2 = zeros( size(faces, 1) * 3, 4);
% for i = 1:size(faces, 1) * 3
%     color2(i, :) = [ 1 0 0 0 ];
% end
% omg2d_ = Omegalib;
% omg2d_.setValues('uniform', 'TRIANGLES', [verts z2], 'Face', faces, 'Color', color2);
% omg2d_.plotFigure(3, @trisurf, faces, x, y, z2);
% 
% 
% %%%%%%%%%%%%% SURFACE TRIANGLES %%%%%%%%%%%%%%%%%%%
% 
% [x,y] = meshgrid(1:15,1:15);
% z = peaks(15);
% verts = [x(:) y(:) z(:)];
% faces = delaunay(x,y);
% color = rand(size(faces, 1) * 3, 4);
% omgsu = Omegalib;
% omgsu.setValues('teapot', 'TRIANGLES', verts, 'Face', faces, 'Color', color);
% omgsu.plotFigure(4, @trisurf, faces, verts(:,1) ,verts(:,2),verts(:,3));


%%%%%%%%%%%%% TEAPOT TRIANGLES %%%%%%%%%%%%%%%

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

color = zeros( size(faces, 1) * 3, 4);
for i = 1:size(faces, 1) * 3
    color(i, :) = [ 0 1 0 1 ];
end

omgtea = Omegalib;
omgtea.setValues('teapot', 'TRIANGLES', verts, 'Face', faces, 'Color', color, 'Vertex Normal', vertex_normal);
omgtea.plotFigure(5, @trisurf, faces, verts(:,1) ,verts(:,2),verts(:,3));
