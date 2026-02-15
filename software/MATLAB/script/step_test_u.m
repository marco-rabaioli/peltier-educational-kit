clear all
close all
clc

% questo script consente di eseguire lo step test su u

%% APRO LA COMUNICAZIONE SERIALE

start_communication     % richiamo lo script per aprire la comunicazione


%% PARAMETRI STEP:

amplitude = 60;     % ampiezza step -100% / 100%
duration = 250;     % durata esperimento in secondi

temp = readTemp(1); 
setActuators(0,0,0);    % spengo tutti gli attuatori

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
    setActuators(amplitude,0,0);    % invio comando attuatori
    % leggo i sensori di temperatura
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



setActuators(0,0,1);    % abilito ventola lato controllato per raffreddare

% calcolo funzioni di trasferimento

[L,T,K,a,Pm_digital] = method_of_areas_vectors(t,y1,amplitude);
[L,T,K,a,Pm_analog] = method_of_areas_vectors(t,y2,amplitude);

Pm_digital

Pm_analog
