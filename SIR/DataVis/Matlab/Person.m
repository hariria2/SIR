classdef Person < handle
   
    properties
       ID;
       Name;
       Age;
       Gender;
       Home;
       Time;
       x;
       y;
       Location;
       State;
       HasBeenSick;
       SusCells;
       InfCells;
       VirLoads;
       MaxInfLev;
    end
    
    methods
        
        function p = Person(id, name, age, gender, home)
            p.ID = id;
            p.Name = name;
            p.Age = age;
            p.Gender = gender;
            p.Home = home;
        end
        
        function setTime(obj, t)
            obj.Time = t;
        end
        function setx(obj, xx)
            obj.x = xx;
        end
        function sety(obj, yy)
            obj.y = yy;
        end
        function setLocation(obj, loc)
            obj.Location = loc;
        end
        function setState(obj, state)
            obj.State = state;
        end
        function setHasBeenSick(obj, hbs)
            obj.HasBeenSick = hbs;
        end
        function setSusCells(obj, sus)
            obj.SusCells = sus;
        end
        function setInfCells(obj, inf)
            obj.InfCells = inf;
        end
        function setVirLoads(obj, vir)
            obj.VirLoads = vir;
        end
        function setMaxInfLev(obj, mil)
            obj.MaxInfLev = max(mil);
        end

        
    end
    
end