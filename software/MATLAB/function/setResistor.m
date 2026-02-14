function setResistor(pwm)
% This function sends the load resistor action

global btGL;
str="%r";

pwm=int32(pwm);

if pwm <-100
    pwm=-100;
    display("Saturation reached!!")
end

if pwm >100
    pwm=100;
    display("Saturation reached!!")

end

str=str+pwm;
t = writeread(btGL,str);

end

