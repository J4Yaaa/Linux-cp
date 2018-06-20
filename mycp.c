#include "com_copy.h"

int main(int argc,char* argv[])
{
    //0.判断输入参数的合法性
    if(argc != 3)
    {
        printf("you shoule input : %s src dst\n",argv[0]);
        exit(1);
    }

    //1.判断源文件是否存在
    if(stat(argv[1],&src_buf) == -1)
    {//不存在，退出
        perror("stat");
        exit(1);
    }

    //2.源是文件
    if(S_ISREG(src_buf.st_mode))
    {
        SrcIsReg(argv,dst_buf,src_buf);
    }

    //3.源文件是目录
    else if(S_ISDIR(src_buf.st_mode))
    {
        SrcIsDir(argv,dst_buf,src_buf);
    }
    return 0;
}
