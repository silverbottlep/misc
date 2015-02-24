src_dir = '/home/eunbyung/Works/data/ucf101_opt_jpeg/';
dst_dir = '/home/eunbyung/Works/data/ucf101_opt_jpeg_tar/';


fid_fname = fopen('srclist_fnames.txt');
fid_nframes = fopen('srclist_nframes.txt');

fname_line = fgetl(fid_fname);
nframe_line = fgetl(fid_nframes);
while ischar(fname_line)
	nframe = str2num(nframe_line);
	filelist=[];
	for i=1:nframe-1
		filelist = [filelist ' ' strcat(fname_line,'_f',num2str(i,'%04u'),'_optx','.jpg ')];
		filelist = [filelist ' ' strcat(fname_line,'_f',num2str(i,'%04u'),'_opty','.jpg ')];
	end

	command = ['tar cf ' strcat(dst_dir, fname_line, '.tar ') filelist];
	[status, cmdout] = system(command);
	fprintf('%s: statis(%d)\n', fname_line, status);

	fname_line = fgetl(fid_fname);
	nframe_line= fgetl(fid_nframes);
end

fclose(fid_fname);
fclose(fid_nframes);
