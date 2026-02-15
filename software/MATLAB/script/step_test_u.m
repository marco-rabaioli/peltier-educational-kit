clear all
close all
clc

% This script allows you to perform a step test by applying a step to the variable "u"


start_communication     % open the communication


%% STEP PARAMETERS:

amplitude = 60;     % step amplitude [-100% / +100%]
duration = 250;     % duration [s]

temp = readTemp(1); 
setActuators(0,0,0);    % power off all actuators

figure
plot(0,temp,'xr-','linewidth',3)
grid on
hold on

c1=clock;

u=[];
t=[];
y1=[];
y2=[];

time = 0;
xlabel('Time [s]')
ylabel('Temperature [°C]')


while time<duration
    setActuators(amplitude,0,0);    % send the command to actuator
    % read temperature sensors
    temp1 = readTemp(1)     
    temp2 = readTemp(2)
    c=clock-c1;
    time=c(6) + c(5)*60;

    plot(time,temp1,'xr','linewidth',3)
    plot(time,temp2,'xb','linewidth',3)

    t=[t;time];
    u=[u;60];
    y1=[y1;temp1];
    y2=[y2;temp2];

    pause(0.3);

end



setActuators(0,0,1);    % power on the fan in order to cool down the hot side

% transfer function computing

[L,T,K,a,Pm_digital] = method_of_areas_vectors(t,y1,amplitude);
[L,T,K,a,Pm_analog] = method_of_areas_vectors(t,y2,amplitude);

Pm_digital
Pm_analog

