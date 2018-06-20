#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

struct stat src_buf;
struct stat dst_buf;

void copy_files(const char* src,const char* dst,int mode);

void SrcIsReg(char* argv[],struct stat dst_buf,struct stat src_buf);

void SrcIsDir(char* argv[],struct stat dst_buf,struct stat src_buf);
