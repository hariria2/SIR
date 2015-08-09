classdef InHostFlu < handle
    properties 
        tend;
        dt;
        ic;
        t;
        dim;
        beta  = @(t) 1;
        delta = @(r) 1;
        p     = @(r) 1;
        c     = @(r) 1;
        dDdr  = @(r) 0;
        result;
        lam;
        Roots;
        V;
        Frames;
        Population;
    end
    methods
        function ihf = InHostFlu(tend, dt, ic, pop)
            ihf.tend  = tend;
            ihf.dt    = dt;
            ihf.t     = 0:dt:ihf.tend;
            ihf.ic    = ic;
            ihf.dim   = length(ic);
            ihf.Population = pop;
        end
        function Simulate(obj, isPeriodic)
%             if isPeriodic
%                 [~,y] = ode45(@obj.flowPeriodic,obj.t,obj. ic);
%             else
%                 [~,y] = ode45(@obj.flow,obj.t,obj.ic);
%             end
            y = zeros(length(obj.t),3);
            cy = obj.ic;
            for ii = 1:length(obj.t)
                y(ii,:) = cy; 
%                 if obj.t(ii) == 2
%                     y(ii,2) = 0.1;
%                 end
                cy = obj.Update(y(ii,:));
            end
        obj.result = y;
        end
        function dy = Update(obj,y)
           
            dy = y + obj.flow(obj.t,y)*obj.dt;
        end
        function dy = flow(obj,t, y)  
            dy    =  zeros(size(y));   
            dy(1) = -obj.beta(t)*y(1)*y(3);
            dy(2) =  obj.beta(t)*y(1)*y(3) - obj.delta(t)*y(2);
            dy(3) =  obj.p(t)*y(2)-obj.c(t)*y(3);
        end
        
            
        function J = Jacobian(obj, t0)
            id = find(t0-obj.dt/2 <= obj.t & obj.t < t0+obj.dt/2); 
            S  = obj.result(id,1);
            I  = obj.result(id,2);
            R  = obj.result(id,3);
            a  = obj.beta(t0);
            b  = obj.gamma(t0);
            
            J = [-a*I*obj.delta(R)*S^(obj.delta(R)-1), ... 
                 -a*S^obj.delta(R),...
                 -a*I*log(S)*S^obj.delta(R)*obj.dDdr(R);...
                 
                 a*I*obj.delta(R)*S^(obj.delta(R)-1),...
                 a*S^obj.delta(R) - b,...
                 a*I*S^obj.delta(R)*log(S)*obj.dDdr(R);...
                 
                 0,...
                 b,...
                 0];
        end 
        function getRoots(obj)
            obj.Roots = zeros(3,length(obj.t));
            
            for ii = 1:length(obj.t)
                t0 = obj.t(ii);
                id = find(t0-obj.dt/2 <= obj.t & obj.t < t0+obj.dt/2); 
                S  = obj.result(id,1);
                I  = obj.result(id,2);
                R  = obj.result(id,3);
            
                p = [1, -obj.beta*(I + S), ...
                obj.gamma^2 + 2*obj.beta^2*I*S, -obj.beta*obj.gamma^2*I];
                obj.Roots(:,ii) = roots(p);
                obj.Roots(abs(obj.Roots)<1e-5)=0;
            end
        end
            
        function getEigen(obj)
            obj.lam = zeros(obj.dim,length(obj.t));
            obj.V = zeros(obj.dim,obj.dim,length(obj.t));
            for ii = 1:length(obj.t)
                J = obj.Jacobian(obj.t(ii));
                [v,l] = eig(J);
                obj.lam(:,ii) = diag(l);
                obj.V(:,:,ii) = v;
                obj.lam(abs(obj.lam)<1e-5)=0;
            end
        end
        function plot(obj,fignum,leg,varargin)
            dleg = {'Susceptible Cells','Infected Cells','Free Virions','Dead'};
            dc   = {'b','r','g'};
            p = zeros(1,obj.dim);
            figure(fignum)
            for ii = 1:obj.dim
                p(ii) = plot(obj.t, obj.result(:,ii)*obj.Population,...
                    dc{ii},'LineWidth',3, 'DisplayName', dleg{ii}); hold on
            end
            hold off
           
            if strcmpi(leg,'LegendOn')
                l = legend(p);
                set(l,'FontSize',18)
            end
            %ti = title(sprintf('\\beta = %3.2e, \\gamma = %3.2e',obj.beta, obj.gamma));
            %set(ti, 'FontSize',20)
            xlabel('Time','FontSize',18)
            ylabel('S, I, V','FontSize',18)
        end    
        function timePlotEigen(obj,fignum) 
            
            figure(fignum)
            for ii = 1:obj.dim
                subplot(obj.dim, 2, 2*ii-1) 
                plot(obj.t,real(obj.lam(ii,:)),'k','LineWidth',3); hold on
                xlabel('Time','FontSize',18)
                ylabel(['\lambda_',sprintf('%d',ii)],'FontSize',18)
                %hold on 
                %plot(obj.t, obj.beta(obj.t)./obj.gamma(obj.t));
                
                subplot(obj.dim, 2, 2*ii) 
                plot(obj.t,imag(obj.lam(ii,:)),'k','LineWidth',3); hold on
                xlabel('Time','FontSize',18)
                ylabel(['\lambda_',sprintf('%d',ii)],'FontSize',18)
            end
            hold off
            subplot(3,2,1)
            title('Real Part','FontSize', 18)
            subplot(3,2,2)
            title('Imaginary Part','FontSize', 18)
        end
        function phasePlot(obj,fignum)
            figure(fignum)
            subplot(2,2,1)
            plot(obj.result(:,1),obj.result(:,2),'k','linewidth',3)
            xlabel('S');ylabel('I');
            subplot(2,2,2)
            plot(obj.result(:,1),obj.result(:,3),'k','linewidth',3)
            xlabel('S');ylabel('R');
            subplot(2,2,3)
            plot(obj.result(:,2),obj.result(:,3),'k','linewidth',3)
            xlabel('I'); ylabel('R')
        end
        function argonPlotEigen(obj) % Legacy
            lam1 = obj.lam(1,:);
            lam2 = obj.lam(2,:);
            lam3 = obj.lam(3,:);
            
            figure
            subplot(2,1,1)
            
            plot(real(lam1),imag(lam1),'k.')
            title('\lambda_1','FontSize',20)
            xlabel('Real Part','FontSize',18)
            ylabel('Imaginary Part','FontSize',18)
           
            subplot(2,1,2)
            plot(real(lam2),imag(lam2),'k.',...
                 real(lam3),imag(lam3),'b.')
            title('\lambda_2 and \lambda_3','FontSize',20)
            xlabel('Real Part','FontSize',18)
            ylabel('Imaginary Part','FontSize',18)
            l = legend('\lambda_2','\lambda_3');
            set(l,'FontSize',18)
        end
        function plotEigenVector(obj,t)
            v = obj.V(:,:,t);
            figure
            for ii = 1:length(v)
                quiver3(0,0,0,v(ii,1),v(ii,2),v(ii,3))
                hold on;
            end
            hold off
        end
        function h = singlePlot(obj,idx, fignum, varargin)
            
            if idx == 'All'
                idx = length(obj.t);
            end
            
            S = obj.result(1:idx,1);
            I = obj.result(1:idx,2);
            R = obj.result(1:idx,3);
            
            lam1 = obj.lam(1,1:idx);
            lam2 = obj.lam(2,1:idx);
            lam3 = obj.lam(3,1:idx);
            
            time = obj.t(1:idx);
            
            h = figure(fignum);
            set(gcf,'units','normalized','outerposition',[0 0 1 1])
            subplot(3,2,[1,2])
            
            plot(time, S, time, I, time, R,'LineWidth',3,varargin{:})
            grid on
            axis([0 obj.tend 0 1]);
            l=legend('Susceptible','Infected','Recovered','Location','northoutside','Orientation','horizontal');
            set(l,'FontSize',18)
            
            %ti = title(sprintf('\\beta = %3.2e, \\gamma = %3.2e',obj.beta, obj.gamma));
            %set(ti, 'FontSize',20)
            xlabel('Time','FontSize',18)
            ylabel('S,I,R','FontSize',18)
            
            subplot(3,2,[3,5])
            plot(real(lam2),imag(lam2),'k.',...
                 real(lam3),imag(lam3),'r.')
            grid on

            axis([min(min(real(obj.lam))), max(max(real(obj.lam))),...
                 min(min(imag(obj.lam))), max(max(imag(obj.lam)))])
            
            hold on
            line('XData', [min(min(real(obj.lam))), max(max(real(obj.lam)))],...
                 'YData', [0, 0]);
            hold on
            line('XData', [0, 0],...
                 'YData', [min(min(imag(obj.lam))), max(max(imag(obj.lam)))]);
             
            xlabel('Real Part','FontSize',18)
            ylabel('Imaginary Part','FontSize',18)
            l = legend('\lambda_2','\lambda_3','Location','northeast');
            set(l,'FontSize',18)
             
            
            subplot(3,2,4)
            plot(time,real(lam2),'k','LineWidth',3)
            grid on
            hold on
            line('XData', [0,obj.tend], 'YData',[0,0]);
            axis([0, obj.tend, min(min(real(obj.lam))),  max(max(real(obj.lam)))]);
            xlabel('Time','FontSize',18)
            ylabel('\lambda_2','FontSize',18)
            subplot(3,2,6)
            plot(time,real(lam3),'k','LineWidth',3)
            grid on
            hold on
            line('XData', [0,obj.tend], 'YData',[0,0]);
            axis([0, obj.tend, min(min(real(obj.lam))),  max(max(real(obj.lam)))]);
            xlabel('Time','FontSize',18)
            ylabel('\lambda_3','FontSize',18)
            
        end
        function makeMovie(obj,fignum,varargin)
            frames(length(obj.t)) = struct('cdata',[],'colormap',[]);
            for ii = 1:length(obj.t)
                h = obj.singlePlot(ii,fignum);
                frames(ii) = getframe(h);
                close(h)
            end   
            obj.Frames = frames;
        end
        function saveEigenData(obj)
            DataFolder = '../../Data/EigenData_v';
            ver = '1';
            [y,m,d]    = datevec(date());
            timestamp  = [num2str(m),'-',num2str(d),'-',num2str(y)];
            DataFolder = [DataFolder, ver,'_',timestamp];
            
            lam1 = obj.lam(1,:)';
            lam2 = obj.lam(2,:)';
            lam3 = obj.lam(3,:)';
            TIME = obj.t';
            
            T_lam = table(TIME,lam1,lam2,lam3);    
        end
    end
end