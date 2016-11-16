classdef Omegalib < handle
    
    properties
        
        % geometry data
        model_name;
        type;
        faces;
        colors;
        vertex_normals;
        face_normals;
        vertices;
        
        %socket
        port;
        u; % udp socket
        
        h;
        setVal;
    end
    
    methods (Access = public)
        
        function obj = Omegalib(varargin)
            
            excep = MException('MATLAB:narginchk:notEnoughInputs','Model name and type (POINTS or TRIANGLES) are required');
            
            if nargin < 2
                throw excep
            end
            
            obj.setVal = false;
            obj.model_name = varargin{1};
            obj.type = varargin{2};
            
            if nargin == 2
                obj.port = 30000;
            else
                obj.port = varargin{3};
            end
        end
        
        function setValues(obj, varargin)
            nVarargs = length(varargin);
           
            for i = 1:nVarargs-1
                if strcmp('Faces', varargin{i})
                    obj.faces = varargin{i+1};
                end
                if strcmp('Vertices', varargin{i})
                    obj.vertices = varargin{i+1};
                end 
                if strcmp('Color', varargin{i})
                    obj.colors = varargin{i+1};
                end
                if strcmp('FaceNormals', varargin{i})
                    obj.face_normals = varargin{i+1};
                end
                if strcmp('VertexNormals', varargin{i})
                    obj.vertex_normals = varargin{i+1};
                end
            end
            
            obj.setVal = true;
        end
        
        function plotFigure(obj, figIdx, func, varargin)
            figure(figIdx)
            obj.h=func(varargin{:});
            uicontrol('Style', 'pushbutton', 'String', 'Send Data', 'Position', [50 20 70 20],  'Callback', @obj.sendGeometry);
            uicontrol('Style', 'pushbutton', 'String', 'Next', 'Position', [150 20 70 20],  'Callback', @obj.nextGeometry);
            uicontrol('Style', 'pushbutton', 'String', 'Add', 'Position', [250 20 70 20],  'Callback', @obj.addGeometry);
            uicontrol('Style', 'pushbutton', 'String', 'Clear', 'Position', [350 20 70 20],  'Callback', @obj.clearGeometry);
        end
        
    end
    
    
    methods (Access = private)
        
        function sendGeometry(obj, source, event)
            
            obj.init;

            if obj.setVal ~= true
                obj.faces = get(obj.h,'Faces');
                obj.vertices = get(obj.h, 'Vertices');
                obj.face_normals = get(obj.h,'FaceNormals');
                obj.vertex_normals = get(obj.h,'VertexNormals');
                
                verticesColor = get(obj.h,'FaceVertexCData');
                alpha = get(obj.h,'FaceVertexAlphaData');
                %alphaDataMapping = get(obj.h,'AlphaDataMapping');
                
                obj.colors = [verticesColor, alpha];
            end
            
            count_vertices= size(obj.vertices, 1);
            count_faces= size(obj.faces, 1);
            count_colors= size(obj.colors, 1);
            count_face_normals = size(obj.face_normals, 1);
            count_vertex_normals= size(obj.vertex_normals, 1);
            
            fwrite(obj.u, strcat('H_MODEL_NAME:', obj.model_name), 'char'); % must be the first one
            fwrite(obj.u, strcat('H_TYPE:', obj.type),'char'); % must be the second one
            fwrite(obj.u, strcat('H_CAMPOS:', num2str(campos)),'char');
            fwrite(obj.u, strcat('H_CAMUP:', num2str(camup)),'char');
            
            if count_vertices > 0
                V = num2str(obj.vertices);
                fwrite(obj.u, strcat('D_VERTEX:',  num2str(count_vertices)), 'char');
                for k=1:count_vertices
                    fwrite(obj.u, V(k,:), 'char');
                end
            end
            
            if count_vertex_normals > 0
                F = num2str(obj.vertex_normals);
                fwrite(obj.u, strcat('D_VERTEX_NORMAL:', num2str(count_vertex_normals)), 'char');
                for k=1:count_vertex_normals
                    fwrite(obj.u, F(k,:), 'char');
                end
            end
            
            if count_faces > 0
                E = num2str(obj.faces);
                fwrite(obj.u, strcat('D_FACE:', num2str(count_faces)), 'char');
                for k=1:count_faces
                    fwrite(obj.u, E(k,:), 'char');
                end
            end
            
            if count_colors > 0
                C = num2str(obj.colors);
                fwrite(obj.u, strcat('D_COLOR:', num2str(count_colors)), 'char');
                for k=1:count_colors
                    fwrite(obj.u, C(k,:), 'char');
                end
            end
            
            
            
            if count_face_normals > 0
                E = num2str(obj.face_normals);
                fwrite(obj.u, strcat('D_FACE_NORMAL:', num2str(count_face_normals)), 'char');
                for k=1:count_face_normals
                    fwrite(obj.u, E(k,:), 'char');
                end
            end
            
            obj.close;
            
        end
        
        function clearGeometry(obj, source, event)
            obj.init;
            fwrite(obj.u, 'H_CLEAR:', 'char');
            obj.close;
        end
        
        function nextGeometry(obj, source, event)
            obj.init;
            fwrite(obj.u, 'H_NEXT:', 'char');
            obj.close;
        end
        
        function addGeometry(obj, source, event)
            obj.init;
            fwrite(obj.u, 'H_ADD:', 'char');
            obj.close;
        end
        
        
        function init(obj)
            obj.u=udp('127.0.0.1', obj.port);
            fopen(obj.u);
        end
        
        function close(obj)
            fclose(obj.u);
            delete(obj.u);
            clear obj.u;
        end
        
    end
    
end