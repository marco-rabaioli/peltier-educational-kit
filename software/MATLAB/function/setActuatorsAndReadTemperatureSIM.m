function temp = setActuatorsAndReadTemperatureSIM(in)
%This function read the temperature and send the pwm value to the
%actuators. In addiction using this function it's possible to control the load disturbances
% loadR -> PWM resistance on load
% loadF -> PWM FAN on load
% This function is used in Simulink.


pwm=in(1);
loadR=in(2);
loadF=in(3);

if loadF>1
    loadF=255;
end

sensorNumber=2;

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


strc="%c";



strc=strc+pwm+"r"+loadR+"f"+loadF;
d=0;

flush(btGL);

switch(sensorNumber)
    case 1 
        strc=strc+"a";
    case 2
        strc=strc+"b";
end  


try
    t = writeread(btGL,strc);
    flush(btGL);
    temp(1) = str2double(t);
    temp(2) = readTemp(2);
catch
    temp(1) = 25;
    temp(2) = 25;
    display("SERIAL ERROR !!!")
    
end

end

