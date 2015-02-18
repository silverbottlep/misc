src_dir = '/home/eunbyung/Works/data/ucf101';
dst_dir = '/home/eunbyung/Works/data/ucf101_jpeg';

srclist = '/home/eunbyung/Works/data/ucfTrainTestlist/srclist.txt';
ground_truth = '/home/eunbyung/Works/data/ucfTrainTestlist/classInd.txt';

% reading label data;
gt = containers.Map;
fid = fopen(ground_truth,'r');
nclass = 0;
while ~feof(fid)
    nclass = nclass + 1;
    line = fgetl(fid);
	B = strsplit(line,' ');	
	gt(B{2}) = str2num(B{1});
end
fprintf('%d categories\n', nclass);
fclose(fid);

% reading filename and associated the number of frames
fid = fopen(srclist,'r');
frames = zeros(ntrain,1);
N = 0;
while ~feof(fid)
    N = N + 1;
    line = fgetl(fid);
	B = strsplit(line,' ');	
	names{N} = B{3};
	frames(N) = str2num(B{2});
end
fprintf('%d name frames\n', N);
fclose(fid);
[names frame_idx] = sort(names);

%train_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/trainlist01.txt';
%test_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/testlist01.txt';
%train_out_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/train1.txt';
%test_out_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/test1.txt';
%train_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/trainlist02.txt';
%test_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/testlist02.txt';
%train_out_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/train2.txt';
%test_out_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/test2.txt';
train_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/trainlist03.txt';
test_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/testlist03.txt';
train_out_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/train3.txt';
test_out_fname = '/home/eunbyung/Works/data/ucfTrainTestlist/test3.txt';


% train split
fid = fopen(train_fname,'r');
ntrain = 0;
label = zeros(ntrain,1);
trainlist = {};
while ~feof(fid)
    ntrain = ntrain + 1;
    line = fgetl(fid);
	A = strsplit(line,' ');
	[pathstr,name,ext] = fileparts(A{1});
	label(ntrain) = str2num(A{2})-1;
	trainlist{ntrain} = name;
end
fprintf('%d training examples\n',ntrain);
fclose(fid);
trainlist = sort(trainlist);

fid = fopen(train_out_fname,'w');
start_idx=1;
for i=1:ntrain
	for j=start_idx:N
		if strcmp(names{j}, trainlist{i})
			temp = regexprep(trainlist{i},'_g[0-9][0-9]_c[0-9][0-9]','');
			temp = regexprep(temp,'v_','');
			fprintf(fid, '%s %d %d\n', trainlist{i}, gt(temp)-1, frames(frame_idx(j)));
			fprintf('%d: %s %d %d\n', i, trainlist{i}, gt(temp)-1, frames(frame_idx(j)));
			start_idx = j+1;
			break;
		end
	end
end
fclose(fid);

% test split
fid = fopen(test_fname,'r');
ntest = 0;
label = zeros(ntest,1);
testlist = {};
while ~feof(fid)
    ntest = ntest + 1;
    line = fgetl(fid);
	A = strsplit(line,' ');
	[pathstr,name,ext] = fileparts(A{1});
	testlist{ntest} = name;
end
fprintf('%d test examples\n',ntest);
fclose(fid);
testlist = sort(testlist);

fid = fopen(test_out_fname,'w');
start_idx=1;
for i=1:ntest
	for j=start_idx:N
		if strcmp(names{j}, testlist{i})
			temp = regexprep(testlist{i},'_g[0-9][0-9]_c[0-9][0-9]','');
			temp = regexprep(temp,'v_','');
			fprintf(fid, '%s %d %d\n', testlist{i}, gt(temp)-1, frames(frame_idx(j)));
			fprintf('%d: %s %d %d\n', i, testlist{i}, gt(temp)-1, frames(frame_idx(j)));
			start_idx = j+1;
			break;
		end
	end
end
fclose(fid);
