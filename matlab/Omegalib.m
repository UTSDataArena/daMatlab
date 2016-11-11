classdef Omegalib < handle
    
    properties
        
        m_model_name;
        m_type;
        m_face;
        m_color;
        m_vertex_normal;
        m_face_normal;
        m_vertex;
        
        m_port;
        m_u; % udp socket
    end
    
    methods (Access = public)
        
        function obj = Omegalib(varargin)
            if nargin == 0
                obj.m_port = 30000;
            else
                obj.m_port = varargin{1};
            end
        end
        
        function setValues(obj, model_name, type, vertex, varargin)
            nVarargs = length(varargin);
            obj.m_vertex = vertex;
            obj.m_model_name = model_name;
            obj.m_type = type;
            for i = 1:nVarargs-1
                if strcmp('Face', varargin{i})
                    obj.m_face = varargin{i+1};
                end
                if strcmp('Color', varargin{i})
                    obj.m_color = varargin{i+1};
                end
                if strcmp('Face Normal', varargin{i})
                    obj.m_face_normal = varargin{i+1};
                end
                if strcmp('Vertex Normal', varargin{i})
                    obj.m_vertex_normal = varargin{i+1};
                end
            end
        end
        
        function plotFigure(obj, figIdx, func, varargin)
            figure(figIdx)
            func(varargin{:});
            uicontrol('Style', 'pushbutton', 'String', 'Data', 'Position', [50 20 70 20],  'Callback', @obj.sendGeometry);
            uicontrol('Style', 'pushbutton', 'String', 'Create/Check', 'Position', [150 20 70 20],  'Callback', @obj.createGeometry);
            uicontrol('Style', 'pushbutton', 'String', 'Add', 'Position', [250 20 70 20],  'Callback', @obj.addGeometry);
            uicontrol('Style', 'pushbutton', 'String', 'Clear', 'Position', [350 20 70 20],  'Callback', @obj.clearGeometry);
        end
        
    end
    
    
    methods (Access = private)
        
        function sendGeometry(obj, source, event)
            
            obj.init;
            
            count_vertex= size(obj.m_vertex, 1);
            count_face= size(obj.m_face, 1);
            count_color= size(obj.m_color, 1);
            count_face_normal = size(obj.m_face_normal, 1);
            count_vertex_normal= size(obj.m_vertex_normal, 1);
            
            fwrite(obj.m_u, strcat('H_MODEL_NAME:', obj.m_model_name), 'char'); % must be the first one
            fwrite(obj.m_u, strcat('H_TYPE:', obj.m_type),'char'); % must be the second one
            fwrite(obj.m_u, strcat('H_CAMPOS:', num2str(campos)),'char');
            fwrite(obj.m_u, strcat('H_CAMUP:', num2str(camup)),'char');
            
            if count_vertex > 0
                V = num2str(obj.m_vertex);
                fwrite(obj.m_u, strcat('D_VERTEX:',  num2str(count_vertex)), 'char');
                for k=1:count_vertex
                    fwrite(obj.m_u, V(k,:), 'char');
                end
            end
            
            if count_vertex_normal > 0
                F = num2str(obj.m_vertex_normal);
                fwrite(obj.m_u, strcat('D_VERTEX_NORMAL:', num2str(count_vertex_normal)), 'char');
                for k=1:count_vertex_normal
                    fwrite(obj.m_u, F(k,:), 'char');
                end
            end
            
            if count_face > 0
                E = num2str(obj.m_face);
                fwrite(obj.m_u, strcat('D_FACE:', num2str(count_face)), 'char');
                for k=1:count_face
                    fwrite(obj.m_u, E(k,:), 'char');
                end
            end
            
            
            
            
            if count_face_normal > 0
                E = num2str(obj.m_face_normal);
                fwrite(obj.m_u, strcat('D_FACE_NORMAL:', num2str(count_face_normal)), 'char');
                for k=1:count_face_normal
                    fwrite(obj.m_u, E(k,:), 'char');
                end
            end
            
            if count_color > 0
                C = num2str(obj.m_color);
                fwrite(obj.m_u, strcat('D_COLOR:', num2str(count_color)), 'char');
                for k=1:count_color
                    fwrite(obj.m_u, C(k,:), 'char');
                end
            end
            
            obj.close;
            
        end
        
        function clearGeometry(obj, source, event)
            obj.init;
            fwrite(obj.m_u, 'H_CLEAR:', 'char');
            obj.close;
        end
        
        function createGeometry(obj, source, event)
            obj.init;
            fwrite(obj.m_u, 'H_CREATE:', 'char');
            obj.close;
        end
        
        function addGeometry(obj, source, event)
            obj.init;
            fwrite(obj.m_u, 'H_ADD:', 'char');
            obj.close;
        end
        
        
        function init(obj)
            obj.m_u=udp('127.0.0.1', obj.m_port);
            fopen(obj.m_u);
        end
        
        function close(obj)
            fclose(obj.m_u);
            delete(obj.m_u);
            clear obj.m_u;
        end
        
    end
    
end