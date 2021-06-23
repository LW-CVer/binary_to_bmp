#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include <string>
#include <dirent.h>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
int getFiles(const std::string path, std::vector<std::string>& files, std::string suffix)
{
    int iFileCnt = 0;
    DIR* dirptr = NULL;
    struct dirent* dirp;

    if ((dirptr = opendir(path.c_str())) == NULL)  //打开一个目录
    {
            return 0;
    }
    while ((dirp = readdir(dirptr)) != NULL) {
        if ((dirp->d_type == DT_REG) &&
            0 == (strcmp(strchr(dirp->d_name, '.'),
                         suffix.c_str())))  //判断是否为文件以及文件后缀名
        {
                files.push_back(dirp->d_name);
        }
	 ++iFileCnt;
    }
    closedir(dirptr);

    return iFileCnt;
}



bool chao_imageToStreamFile(cv::Mat image, std::string filename)
{
	if (image.empty()) return false;
	const char *filenamechar = filename.c_str();
	FILE *fpw = fopen(filenamechar, "wb");//如果没有则创建，如果存在则从头开始写
	if (fpw == NULL)
	{
		fclose(fpw);
		return false;
	}
	int channl = image.channels();//第一个字节  通道
	int rows = image.rows;     //四个字节存 行数
	int cols = image.cols;   //四个字节存 列数
 
	fwrite(&channl, sizeof(char), 1, fpw);
	fwrite(&rows, sizeof(char), 4, fpw);
	fwrite(&cols, sizeof(char), 4, fpw);
	char* dp = (char*)image.data;
	if (channl == 3)
	{
		for (int i = 0; i < rows*cols; i++)
		{
			fwrite(&dp[i * 3], sizeof(char), 1, fpw);
			fwrite(&dp[i * 3 + 1], sizeof(char), 1, fpw);
			fwrite(&dp[i * 3 + 2], sizeof(char), 1, fpw);
		}
	}
	else if (channl == 1)
	{
		for (int i = 0; i < rows*cols; i++)
		{
			fwrite(&dp[i], sizeof(char), 1, fpw);
		}
	}
	fclose(fpw);
	return true;
}

bool chao_StreamFileToImage(std::string filename, cv::Mat &image)
{
	const char *filenamechar = filename.c_str();
	FILE *fpr = fopen(filenamechar, "rb");
	if (fpr == NULL)
	{
		fclose(fpr);
		return false;
	}
	int channl(0);
	int imagerows(0);
	int imagecols(0);
	fread(&channl, sizeof(char), 1, fpr);//第一个字节 通道
	fread(&imagerows, sizeof(char), 4, fpr); //四个字节存 行数
	fread(&imagecols, sizeof(char), 4, fpr); //四个字节存 列数
	if (channl == 3)
	{
		image = cv::Mat::zeros(imagerows,imagecols, CV_8UC3);
		char* pData = (char*)image.data;
		for (int i = 0; i < imagerows*imagecols; i++)
		{
			fread(&pData[i * 3], sizeof(char), 1, fpr);
			fread(&pData[i * 3 + 1], sizeof(char), 1, fpr);
			fread(&pData[i * 3 + 2], sizeof(char), 1, fpr);
		}
	}
	else if (channl == 1)
	{
		image = cv::Mat::zeros(imagerows, imagecols, CV_8UC1);
		char* pData = (char*)image.data;
		for (int i = 0; i < imagerows*imagecols; i++)
		{
			fread(&pData[i], sizeof(char), 1, fpr);
		}
	}
	fclose(fpr);
	return true;
}

int main()
{ 
  printf("0");
  std::string dir_name="/home/nvidia/lw_temp/saved/";
  std::vector<std::string> files;
  //cv::Mat src = cv::imread(file_name);
  //std::string filename = "./test.bin";
  //chao_imageToStreamFile(src, filename);
  const char* saved_path="./result/";
  if(access(saved_path,F_OK)){
      mkdir(saved_path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
  //printf("1");
  getFiles(dir_name,files,".bin");
  //printf("2");
  for(auto file : files){
      cv::Mat dst;
      printf("%s\n",file.c_str());
      int pos=file.find(".");
      chao_StreamFileToImage(dir_name+file, dst);
      cv::imwrite(saved_path+file.substr(0,pos)+".bmp",dst);
  } 
  return 0;
}
