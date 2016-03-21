classdef Location < handle

    properties
    ID;
    Name;
    Type;
    Xmin;
    Xmax;
    Ymin;
    Ymax;
    Perimeter
    end
    methods
        function l = Location(id, name, type, xmin, xmax, ymin, ymax)
            l.ID = cell2mat(id);
            l.Name = cell2mat(name);
            l.Type = cell2mat(type);
            l.Xmin = cell2mat(xmin);
            l.Xmax = cell2mat(xmax);
            l.Ymin = cell2mat(ymin);
            l.Ymax = cell2mat(ymax);
            l.Perimeter = [l.Xmin, l.Xmax; l.Ymin, l.Ymax];
        end
    end
    
end
