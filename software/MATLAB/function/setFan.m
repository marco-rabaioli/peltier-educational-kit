function setFan(pwm)
% This function sends the fan state:
% 0 - OFF
% 1 - ON

global btGL;
str="%f";

pwm=int32(pwm);


if pwm >1
    pwm=1;
end

str=str+pwm;
t = writeread(btGL,str);

end

