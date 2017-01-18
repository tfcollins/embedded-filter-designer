%% Generate MEX function
mex GCC='/usr/bin/gcc-4.7' -lliquid genFilt.c

%% Load weights from ADI designer
k = load('filttest.mat');
d = k.d;

% Pick required taps
numTaps = 128;

N = 1;% 2 -> use all amplitude | 1 -> Use only available frequencies (Best)

pass_bands = length(d.B1Amplitudes)*N;
stop_bands = length(d.B2Amplitudes)*N;

% des[numBands X 1]: Desired response
des = [d.B1Amplitudes,d.B2Amplitudes];
des = des(1:2/N:end);

% bands[numBands X 2]: Band edges [0 0.5]
% Convert BxFrequencies to Bin edges
binCenters = d.B1Frequencies;
bands1 = linspace(binCenters(1)/2,binCenters(end)/2,length(binCenters)*N);
binCenters = d.B2Frequencies;
bands2 = linspace(binCenters(1)/2,binCenters(end)/2,length(binCenters)*N);
bands = [bands1,bands2];

% weights[numBands X 1]: Relative error weighting
weights = [k.W1,k.W2];
weights = weights(1:2/N:end);
%weights = ones(1,numBands);

numBands = length(bands)/2;

%% Checks
assert(isequal(size(weights),[1 numBands]),'weights Wrong Size');
assert(isequal(size(bands),[1 numBands*2]),'bands Wrong Size');
assert(isequal(size(des),[1 numBands]),'des Wrong Size');

%% Run
h = genFilt(numTaps, numBands, bands, des, weights, pass_bands, stop_bands);

%% Determine response
fvtool(h);

%% Determin MATLAB version
%Hd = design(d,'equiripple','B1Weights',k.W1,'B2Weights',k.W2,'SystemObject',false);
%A = [d.B1Amplitudes,d.B2Amplitudes];
%W = [k.W1,k.W2];
%h2 = firpm(numTaps-1, bands.*2, A, W);
fvtool(k.Hd);

%[h.', k.Hd.Numerator.']
