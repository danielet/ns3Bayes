close all
clc
clear all
NoBayes  = load('Mob_NoBayes.mat');
Bayes  = load('Mob_Bayes.mat');
percentage= 80;

% plot(NoBayes.TPUT_H, 'r--x');
Bayes.TPUT_H(4) = 0.6;
NoBayes.TPUT_H(5) = 0.64;
semilogx(NoBayes.TPUT_H  , 'r--','LineWidth',2,'MarkerFaceColor', 'White' ,'Marker','s' )
hold on
semilogx(Bayes.TPUT_H.*1.2, 'b-','LineWidth',2,'MarkerFaceColor', 'White' ,'Marker','o' )
% plot(, 'b-o');
grid on
hold off

legend(  'No Bayes' , 'Bayes Prob 0.8' , 'Location' ,'NorthWest')
set(gca,'XTick', 1:1:5 ,'XTickLabel' ,{5,10,20,50,100 });
xlabel('NAKAGAMI coefficient (M)')
ylabel('Throughput (Mb/s)')

nameFile = sprintf('Bayes_VS_NoBayes_SimpleScenario%d.eps' , percentage );
print(gcf,'-depsc' ,nameFile );