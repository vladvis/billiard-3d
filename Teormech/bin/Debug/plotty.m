function plotty(filename)
    fid = fopen(filename);

    R = 0.0571500;
    figure('units','normalized','outerposition',[0 0 1 1]);
    hold on;
    
    h = plot(nan,'.');
    axis equal;
    
    
    X = [];
    Y = [];
    set(h,'XData',X,'YData',Y);
    
    tline = fgets(fid);
    nums = sscanf(tline,'%f');
    balls = nums(1);
    
    t=[0:pi/180:2*pi]; 
    xc=R*cos(t); 
    yc=R*sin(t);
    
    Hc = [];
    for i=1:1:balls
        Hc = [Hc plot(nan)];
    end
    
    tline = fgets(fid);
    while ischar(tline)
        disp(tline);
        drawnow;
        nums = sscanf(strrep(tline,',',' '),'%f');
        
        for i=1:1:balls
            X = [X nums(3*i-1)];
            Y = [Y nums(3*i)];
            set(Hc(i),'XData',xc+nums(3*i-1),'YData',yc+nums(3*i));
            set(h,'XData',X,'YData',Y);
        end
        tline = fgets(fid);
    end
    hold off;
    
    fclose(fid);
end

