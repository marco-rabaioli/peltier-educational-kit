function temp = readTemp(sensorNumber)
%   This function reads the temperature sensors
%   1 -> Digital sensor
%   2 -> Analog sensor

global btGL;

flush(btGL);

switch(sensorNumber)
    case 1
        t = writeread(btGL,"%b");
    case 2
        t = writeread(btGL,"%a");
end  

temp = str2double(t);



end

