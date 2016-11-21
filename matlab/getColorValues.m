function vertex_color = getColorValues(graphics_obj)

vertex_color = [];


if isa(graphics_obj, 'matlab.graphics.axis.Axes')
    child_obj = get(graphics_obj,'Children');
    graphics_obj = child_obj;
end


if isa(graphics_obj, 'matlab.graphics.chart.primitive.Scatter')
    
    axes_obj = get(graphics_obj,'Parent');
    colorLimits = get(axes_obj,'CLim');
    
    figure_obj = get(axes_obj,'Parent');
    cmap = get(figure_obj,'colormap');
    
    XData = get(graphics_obj,'XData').';
    
    vColor = get(graphics_obj,'CData');
    if (size(vColor, 1) == 1) && (size(vColor, 2) == 3) % RGB triplet
        vertex_color = ones(size(XData,1), 3);
        for i=1:size(XData,1)
            vertex_color = vertex_color(i,:) * vColor;
        end
    elseif (size(vColor, 1) == size(XData,1)) && (size(vColor, 2) == 3) %true colors
        vertex_color = vColor;
    elseif (size(vColor, 1) == size(XData,1)) && (size(vColor, 2) == 1) %colormap colors
        vertex_color = getMapping(vColor, cmap, colorLimits);
    end
    
    
    aValues = get(graphics_obj,'MarkerFaceAlpha');
    alpha_values = ones(size(XData,1), 1) * aValues;
    
    vertex_color = [vertex_color alpha_values];
    
elseif isa(graphics_obj, 'matlab.graphics.chart.primitive.Surface')
    
    axes_obj = get(graphics_obj,'Parent');
    colorLimits = get(axes_obj,'CLim');
    alphaLimits = get(axes_obj,'ALim');
    
    figure_obj = get(axes_obj,'Parent');
    cmap = get(figure_obj,'colormap');
    amap = get(figure_obj,'alphamap').';
    
    vColor = get(graphics_obj,'CData');
    if size(vColor,3) == 1 %colormap colors
        vColor = reshape(vColor, [], 1);
        vertex_color = getMapping(vColor, cmap, colorLimits);
    elseif size(vColor,3) == 0 %true colors
        vColor = reshape(vColor, [], 3);
        vertex_color = vColor;
    end
    
    aValues = get(graphics_obj,'AlphaData');
    alphaMapping = get(graphics_obj,'AlphaDataMapping');
    
    if aValues == 1
        aValues = ones(size(vColor, 1), 1);
    end
    
    if strcmp(alphaMapping, 'scaled')
        alpha_values = getMapping(aValues, amap, alphaLimits);
    elseif strcmp(alphaMapping, 'none')
        alpha_values = aValues;
    end
    
    vertex_color = [vertex_color alpha_values];
    
elseif isa(graphics_obj, 'matlab.graphics.primitive.Patch')
    
    axes_obj = get(graphics_obj,'Parent');
    colorLimits = get(axes_obj,'CLim');
    alphaLimits = get(axes_obj,'ALim');
    
    figure_obj = get(axes_obj,'Parent');
    cmap = get(figure_obj,'colormap');
    amap = get(figure_obj,'alphamap').';
    
    vColor = get(graphics_obj,'FaceVertexCData');
    if size(vColor,2) == 1 %colormap colors
        vertex_color = getMapping(vColor, cmap, colorLimits);
    elseif size(vColor,2) == 3 %true colors
        vertex_color = vColor;
    end
    
    aValues = get(graphics_obj,'FaceVertexAlphaData');
    alphaMapping = get(graphics_obj,'AlphaDataMapping');
    
    if size(aValues,1) == 0
        aValues = ones(size(vertex_color, 1), 1);
    end
    
    if strcmp(alphaMapping, 'scaled')
        alpha_values = getMapping(aValues, amap, alphaLimits);
    elseif strcmp(alphaMapping, 'none')
        alpha_values = aValues;
    end
    
    vertex_color = [vertex_color alpha_values];
    
end

    function mapping = getMapping(color, map, limits)
        cmin = limits(1);
        cmax = limits(2);
        m = size(map, 1);
        
        index =  fix((color-cmin)/(cmax-cmin)*m)+1;
        index(index<1) = 1;
        index(index>m) = m;
        
        mapping = map(index, :);
    end


end



