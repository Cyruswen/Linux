/*************************************************************************
	> File Name: mycp.c
	> Author: kaikai
	> Mail: Cyruswen@163.com 
	> Created Time: 2018年03月03日 星期六 17时12分45秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib>
#include<sys/stat.h>
#include<unistd.h>
#include<String.h>

void copy_file(const char *src,const char *dst,mode_t mode)
{
	FILE *fp_dst = fopen(argv[2],"w");
	FILE *fp_src = fopen(argv[1],"r");
	if(fp_dst==NULL || fp_src==NULL)
	{
		fprintf(stderr,"open file erroe\n");
		return -1;
	}
	chmod(argv[2].sbuf_src.st_mode);

	char buf[10] = {};
	int r;
	while((r=fread(buf,1,sizeof(buf),fp_src))>1)
	{
		fwrite(buf,1,r,fp_dst);
	}
	fclose(fp_dst);
	fclose(fp_src);
}

void cp_dir(const char *src,const char *dst )
{
	DIR * pdir = opendir(src);
	struct dirent *pd = NULL;
	while((pd=readdir(pdir)) != NULL)
	{
		if(strcmp(pd->d_name,".") == 0 || strcmp(pd->d_name,".." == 0))
		{
			continue;
		}
		int len = strlen(src)+2+strlen(pd->d_name);
		char *src_name = (char*)malloc(len);
		meset(src_name,0x00,len);
		sprintf(src_name,"%s/%s",src,pd->d_name);

		len = strlen(dst) + 2 + strle(pd->d_name);
		char *dst_name = (char*)malloc(len);
		meset(dst_name,0x00,len);
		sprintf(dst_name,"%s/%s",dst,pd->d_name);

		lstat(src_name.&sbuf_src);
		if(S_ISREG(sbuf_src.st_mode))
		{
			copy_file(src_name,dst_name ,sbuf_src.sr_mode);
		}else if(S_ISDIR(sbuf_src.st_mode))
		{
			mkdir(dst_name,sbuf_src.st_mode);
			cp_dir(src_name,dst_name);
		}
	}
}

int main(int argc, char *argv[])
{
	if( argc != 3 )
	{
		fprintf(stderr,"%s src dst\n",argv[0]);
		exit(1);
	}

	struct stat sbuf_src;
	if( lstat(argv[1],&sbuf_src) == -1)
	{
		fprintf(stderr."%s 不存在\n",argv[1]);
		exit(1);
	}
	mode_t old = umask(0);
	if( S_ISREG(sbuf_src.st_mode))
	{
		struct stat sbuf_dst;
		if(lstat(argv[2], &sbuf_dst) == -1)
		{
			copy_file(argv[1],argv[2],sbuf_src.sr_mode);
		}else if(S_ISREG(sbuf_dst.st_mode))
		{
			printf("目标文件不存在，是否覆盖？[y/n]: ");
			char c = 'n';
			scanf("%[yYnN]",&c);
			if(c == 'y' || c == 'Y')
			{
				copy_file(argv[1],argv[2],sbuf_src.sr_mode);
			}
		}else if(S_ISDIR(sbuf_dst.st_mode))
		{
			int len = (char*)malloc(strlen(argv[1])+1+strlen(argv[2]));
			char *tmp = (char*)(len);
			meset(tmp,0x00,len);
			sprintf(tmp,"%s/%s",argv[2],argv[1]);
			copy_file(argv[1],argv[2],sbuf_src.sr_mode);
		}else if(S_ISDIR(buf_src.st_mode))
		{
			struct stat sbuf_dst;
			if(lstat(argv[2],&sbuf_dst) == -1)
			{
				mkdir(argv[2],sbuf_src.st_mode);
 				cp_dir(argv[1],argv[2]);
			}else if(S_ISREG(sbuf_dst.st_mode))
			{
 				fprnitf(stderr."不能将目录拷贝到文件\n");
				exit(1);
			}else if(S_ISDIR(sbuf_dst.st_mode))
			{
				int len = (char*)malloc(strlen(argv[1])+1+strlen(argv[2]));
				char *tmp = (char*)(len);			
				meset(tmp,0x00,len);
				sprintf(tmp,"%s/%s",argv[2],argv[1]);
				cp_dir(argv[1],tmp);
			}
		}
		umask(old);
	}
}

