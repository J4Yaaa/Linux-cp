#include "com_copy.h"

void copy_files(const char* src,const char* dst,int mode)
{
    int src_fd = open(src,O_RDONLY);
    int dst_fd = open(dst,O_WRONLY | O_CREAT, mode);
    char buf[256] = {0};
    while(read(src_fd,buf,sizeof(buf)-1))
    {
        write(dst_fd,buf,strlen(buf));
        memset(buf,0x00,sizeof(buf));
    }
    close(src_fd);
    close(dst_fd);
}

void cope_dir(const char* src,const char* dst)
{
    DIR* pdir = opendir(src);
    if(pdir == NULL)
    {
        perror("open dir");
        exit(1);
    }
    struct dirent* pd = NULL;
    while((pd = readdir(pdir)) != NULL)
    {
        if(strcmp(pd->d_name,".") == 0 || strcmp(pd->d_name,"..") == 0)
        {
            continue;
        }
        char* psrc = (char*)malloc(strlen(src)+1+strlen(pd->d_name));
        sprintf(psrc,"%s/%s",src,pd->d_name);
        char* pdst = (char*)malloc(strlen(dst)+1+strlen(pd->d_name));
        sprintf(pdst,"%s/%s",dst,pd->d_name);
        struct stat buf;
        if(stat(psrc,&buf) == -1)
        {
            perror("stat");
            exit(1);
        }
        if(S_ISREG(buf.st_mode))
        {
            copy_files(psrc,pdst,buf.st_mode);
        }
        else if(S_ISDIR(buf.st_mode))
        {
            mkdir(pdst,buf.st_mode);
            cope_dir(psrc,pdst);
        }
    }
}

void SrcIsReg(char* argv[],struct stat dst_buf,struct stat src_buf)
{
    // a) 目标不存在
    if(stat(argv[2],&dst_buf) == -1)
    {//不存在的话就拷贝文件
        copy_files(argv[1],argv[2],src_buf.st_mode);
    }

    // b) 目标是文件
    if(S_ISREG(dst_buf.st_mode))
    {//如果目标文件已经存在，询问要不要替换
        char ask = 'n';
        printf("cp: overwrite ‘%s’?",argv[2]);
        scanf(" %[Y|y|N|n]",&ask);
        if(ask == 'Y' || ask == 'y')
        {//如果选择覆盖，就进行文件拷贝
            truncate(argv[2],0);
            copy_files(argv[1],argv[2],src_buf.st_mode);
        }
        else
        {//如果不选择覆盖的话，就直接结束进程
            exit(0);
        }
    }

    // c) 目标是目录
    //    如果目标是目录的话，就需要改变目标路径为 argv[2]/argv[1]
    else if(S_ISDIR(dst_buf.st_mode))
    {
        char tmp[256] = {0};
        sprintf(tmp,"%s%s",argv[2],argv[1]);
        //更改目录以后进行文件拷贝，使源文件称为目标目录的子文件
        copy_files(argv[1],tmp,src_buf.st_mode);
    }

}

void SrcIsDir(char* argv[],struct stat dst_buf,struct stat src_buf)
{
    // a) 目标不存在,应该创建目录
    if(stat(argv[2],&dst_buf) == -1)
    {
        mkdir(argv[2],src_buf.st_mode);
        cope_dir(argv[1],argv[2]);
    }

    // b) 目标文件是文件，不合法，直接退出
    if(S_ISREG(dst_buf.st_mode))
    {
        exit(1);
    }

    // c) 目标文件是目录，使源文件递归的成为目标文件的子目录
    if(S_ISDIR(src_buf.st_mode))
    {
        char tmp[256] = {0};
        sprintf(tmp,"%s/%s",argv[2],argv[1]);
        mkdir(tmp,src_buf.st_mode);
        cope_dir(argv[1],tmp);
    }
}
