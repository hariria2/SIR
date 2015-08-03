classdef SQLVisualization < handle
    
    properties
        Connection;
        People = {};
        HistoryData;
        T;
        S;
        I;
        P;
        R;
        D;
    end
    
    
    methods
        function sv = SQLVisualization(db, usrnm, psswd, vndr, hst)
            sv.Connection = database(db, usrnm, psswd, 'vendor', vndr, 'server', hst);
        end
        
        function readHistoryData(obj)
            sqlquery = 'SELECT * FROM HistoryData';
            curs = exec(obj.Connection,sqlquery);
            curs = fetch(curs);
            data = cell2mat(curs.data);
            
            obj.T = data(:,2);
            obj.S = data(:,3);
            obj.I = data(:,4);
            obj.P = data(:,5);
            obj.R = data(:,6);
            obj.D = data(:,7);
            
        end
        
        function getPeople(obj)
            sqlquery = 'SELECT * FROM People';
            curs = exec(obj.Connection,sqlquery);
            curs = fetch(curs);
            
            l = length(curs.data);
            
            for ii = 1:l
                
                
            end
        end
    end
    
end
