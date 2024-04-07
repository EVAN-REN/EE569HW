% Demo for Structured Edge Detector (please see readme.txt first).

%% set opts for training (see edgesTrain.m)
opts=edgesTrain();                % default options (good settings)
opts.modelDir='models/';          % model will be in models/forest
opts.modelFnm='modelBsds';        % model name
opts.nPos=5e5; opts.nNeg=5e5;     % decrease to speedup training
opts.useParfor=0;                 % parallelize if sufficient memory

%% train edge detector (~20m/8Gb per tree, proportional to nPos/nNeg)
tic, model=edgesTrain(opts); toc; % will load model if already trained

%% set detection parameters (can set after training)
model.opts.multiscale=0;          % for top accuracy set multiscale=1
model.opts.sharpen=2;             % for top speed set sharpen=0
model.opts.nTreesEval=4;          % for top speed set nTreesEval=1
model.opts.nThreads=4;            % max number threads for evaluation
model.opts.nms=0;                 % set to true to enable nms

%% evaluate edge detector on BSDS500 (see edgesEval.m)
if(0), edgesEval( model, 'show',1, 'name','' ); end

%% detect edge and visualize results
I = imread('Pig.jpg');
tic, E=edgesDetect(I,model); toc

thresholds = [0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95];
F_measure = zeros(10, 0);
for a = 1:10


E_binary = E > (1-thresholds(a));


% imshow(1-E_binary);

%E = 1 - E;  
% figure(1); im(E);

prms.out = 'edge_detection_results.mat';
prms.thrs = 10;                 
prms.maxDist = 0.01;                  
prms.thin = 1;                        


% [thrs, cntR, sumR, cntP, sumP, V] = edgesEvalImg(E, G, prms);


load('Pig_GT.mat'); 

[thrs, cntR, sumR, cntP, sumP, V] = edgesEvalImg(E_binary, 'Pig_GT.mat', prms);
% save('edge_detection_results.mat', 'thrs', 'cntR', 'sumR', 'cntP', 'sumP', 'V');
num_gt = 5;  


precision = zeros(num_gt, 1);
recall = zeros(num_gt, 1);

for i = 1:num_gt

    mean_precision = sum(cntP(i)) / sum(sumP(i));
    mean_recall = sum(cntR(i)) / sum(sumR(i));
    
    precision(i) = mean_precision;
    recall(i) = mean_recall;
end

mean_precision_all = mean(precision);
mean_recall_all = mean(recall);

F_measure(a) = 2 * (mean_precision_all * mean_recall_all) / (mean_precision_all + mean_recall_all);
fprintf('F Measure:      %.4f\n', F_measure(a));
end

% disp('Precision and Recall for Each Ground Truth:');
% disp('------------------------------------------');
% disp(' GT | Precision | Recall ');
% disp('------------------------------------------');
% for i = 1:num_gt
%     fprintf(' %2d |   %.4f   |  %.4f\n', i, precision(i), recall(i));
% end
% disp('------------------------------------------');
% fprintf('Mean Precision: %.4f\n', mean_precision_all);
% fprintf('Mean Recall:    %.4f\n', mean_recall_all);
% fprintf('F Measure:      %.4f\n', F_measure);



plot(thresholds, F_measure);

title(' Function');
xlabel('thresholds');
ylabel('F_measure');
