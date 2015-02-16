src_dir = '/home/eunbyung/Works/data/ucf101';
dst_dir = '/home/eunbyung/Works/data/ucf101_jpeg';

split1_train = '/home/eunbyung/Works/data/ucfTrainTestlist/trainlist01.txt'
split1_train = '/home/eunbyung/Works/data/ucfTrainTestlist/trainlist01.txt'
split1_test = '/home/eunbyung/Works/data/ucfTrainTestlist/testlist01.txt'
%train_strings = fileread(split1_train);

WIDTH=340;
HEIGHT=256;
src_list = dir(src_dir);
src_list = src_list(3:end);
parfor i=1:length(src_list)
  obj = VideoReader(fullfile(src_dir,src_list(i).name));
  k=0;
  while obj.hasFrame
    cur_frame = obj.readFrame;
    k=k+1;
    jpeg_name = strcat(src_list(i).name(1:end-4),'_f',num2str(k,'%04u'),'.jpg');
    cur_frame = imresize(cur_frame,[HEIGHT,WIDTH]);
    imwrite(cur_frame, fullfile(dst_dir,jpeg_name),'jpg');
  end
  fprintf('(%d:%d)',i,length(src_list));
end
