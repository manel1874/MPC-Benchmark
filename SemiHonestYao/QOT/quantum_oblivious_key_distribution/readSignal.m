function [ data, symbolPeriod, samplingPeriod, type, numberOfSymbols ] = readSignal( fname )
%READSIGNALDATA Reads signal data to "visualizer".
%   [ data, samplingFrequency ] = READSIGNALDATA(fid, type, symbolPeriod, samplingPeriod)
%   just reads data ("data") from a file ("fid")
%   knowing the data parameters ("type", "symbolPeriod" and "samplingPeriod") and 
%   returning the new sampling simulation frequency ("samplingFrequency").

fid = fopen(fname,'r');

type = fscanf(fid,'Signal type: %s\n',1);
symbolPeriod = fscanf(fid,'Symbol Period (s): %f\n',1);
samplingPeriod = fscanf(fid,'Sampling Period (s): %f\n',1);
fscanf(fid,'// ### HEADER TERMINATOR ###\n',1);

%% Some Standard types
tb = 'Binary';
tc1 = 'TimeDiscreteAmplitudeDiscreteComplex';
tc2 = 'TimeDiscreteAmplitudeContinuousComplex';
tc3 = 'TimeContinuousAmplitudeDiscreteComplex';
tc4 = 'TimeContinuousAmplitudeContinuousComplex';
tc5 = 'BandpassSignal';
toxy = 'OpticalSignalXY';

%% Get global variable "nRead"
%nReadr = getGlobalnRead;
nReadr = 256;
% 
% %% Get global variable "t_binary"
% t_binaryr = getGlobalt_binary;
t_binaryr = 'int';
% 
% %% Get global variable "t_real"
%t_realr = getGlobalt_real;
t_realr = 'double'
% 
% %% Get global variable "t_complex"
% t_complexr = getGlobalt_complex;
t_complexr = 'double';
% 
% %% Get global variable "t_complex"
% global valueXY;
% valueXY=struct('x',complex(0+1i*0),'y',complex(0+1i*0));
% 
% 

%% Sampling frequency    
samplingFrequency = 1/samplingPeriod;

%% Number of samples per period
samplesPerSymbol = (symbolPeriod/samplingPeriod);

%% Read data
if strcmp(type, tb) % Binary signals
    data = fread(fid, nReadr, t_binaryr);
    data = data';
    
   numberOfSymbols = (length(data)/samplesPerSymbol);
    
    return;
end

 
if strcmp(type, tc1) || strcmp(type, tc2) || strcmp(type, tc3) || strcmp(type, tc4) || strcmp(type, tc5)% Complex signals
   data = fread(fid, 2*samplesPerSymbol*nReadr, t_complexr);
   data = data(1:2:end) + 1i.*data(2:2:end);
   data = real(data)' + imag(data)'.*1i;
   
   numberOfSymbols = (length(data)/samplesPerSymbol);
   
   return;
end

if strcmp(type, toxy)
   data = fread(fid, 4*double(samplesPerSymbol)*nReadr, t_complexr);
   
   numberOfSymbols = (length(data)/samplesPerSymbol);
   
   return;
end

% Other signals
data = fread(fid, double(samplesPerSymbol)*nReadr, t_realr);
data = data';

numberOfSymbols = (length(data)/samplesPerSymbol);

