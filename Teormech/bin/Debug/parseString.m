function parseString(str)
    nums = sscanf(strrep(str,',',' '),'%f');
    balls = (length(nums)-1)/3;
    
    for i=1:1:balls
        plot(nums(3*i-1), nums(3*i), '.');
    end
end

