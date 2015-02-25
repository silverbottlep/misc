#!/bin/bash

for i in ./ucf101_opt_jpeg/* ; do
	dirname="${i%.*}"
	dirname=$(basename "$dirname")
	mkdir ./ucf101_opt/$dirname
	tar xf $i -C ./ucf101_opt/$dirname
	echo $i
done
