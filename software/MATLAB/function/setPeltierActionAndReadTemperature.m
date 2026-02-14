function temp = setPeltierActionAndReadTemperature(pwm)
%This function read the temperature and send the pwm value to the
%actuator. 

sensorNumber=1;

global btGL;


pwm=int32(pwm);

if pwm <-100
    pwm=-100;
    display("Saturation reached!!")
end

if pwm >100
    pwm=100;
    display("Saturation reached!!")

end

flush(btGL);

switch(sensorNumber)
    case 1
        t = writeread(btGL,"%b");
    case 2
        t = writeread(btGL,"%a");
end  


temp = str2double(t);
str="%p";

str=str+pwm;

t = writeread(btGL,str);

end

