src_dir = '/home/eunbyung/Works/data/ucf101_jpeg/';

fid_fname = fopen('srclist_fnames.txt');
fid_nframes = fopen('srclist_nframes.txt');

fname_line = fgetl(fid_fname);
nframe_line = fgetl(fid_nframes);
while ischar(fname_line)
  nframe = str2num(nframe_line);
  filelist=[];
  for i=1:nframe
    filelist = [filelist ' ' fullfile(src_dir,strcat(fname_line,'_f',num2str(i,'%04u'),'.jpg '))];
  end

  %mkdir(fullfile(src_dir,fname_line));
  command = ['mv ' filelist ' ' fullfile(src_dir,fname_line) '/'];
  [status, cmdout] = system(command);
  fprintf('%s: status(%d)\n', fname_line, status);

  fname_line = fgetl(fid_fname);
  nframe_line= fgetl(fid_nframes);
end

fclose(fid_fname);
fclose(fid_nframes);
