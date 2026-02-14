function setActuators(pwm,loadR,loadF)
%This function read the temperature and send the pwm value to the
%actuators.
% pwm   -> Peltier power action
% loadR -> PWM resistance power
% loadF -> FAN 0-OFF 1-ON

sensorNumber=1;     % 1-digital reading  2-analog reading

global btGL;    % get the connection parameters

if loadF>=1
    loadF=1;
end

if loadR>50
    loadR=50;
end

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

flush(btGL);

switch(sensorNumber)
    case 1 
        strc=strc+"a";
    case 2
        strc=strc+"b";
end  

t = writeread(btGL,strc);
temp = str2double(t);

end

