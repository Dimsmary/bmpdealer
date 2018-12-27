#include <stdio.h>
#include <stdlib.h>
#define bm 0x4d42
#define len_header 14		//定义头文件长度
#define len_info 40			//定义信息头长度
#define onebyte 256
#define twobyte 65536
#define threebyte 16777216
#define fourbyte 4294967296
#define byte2mb 1048576
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char u8;
typedef unsigned int u16;
typedef struct BMP_HEADER	//定义头文件结构
{
	WORD bfType;				//文件标识符
	DWORD bfSize;				//整个BMP文件的大小
	WORD r1;
	WORD r2;					//保留
	DWORD bfOffBits;			//偏移量
}BMPHEADER;
typedef struct BMP_INFO		//定义信息头
{
	DWORD biSize;				//信息头所需字节数
	DWORD	biWidth;			//图像宽度（像素单位）
	DWORD biHeight;			//高度
	WORD biPlanes;			//位面数
	WORD biBitCount;			//一个像素点占几bit
	DWORD biCompression;		//压缩类型
	DWORD biSizeImage;		//位图大小
	DWORD biXpelsPerMeter;	//水平分辨率 像素/米
	DWORD biYpelsPERMeter;	//垂直分辨率
	DWORD biClrUsed;			//实际使用的调色板引数
	DWORD biClrImportant;		//重要影响的颜色索引数目
}BMPINFO;
typedef struct RGB_QUAD
{
	WORD rgbblue;
	WORD rgbgreen;
	WORD rgbred;
	WORD rgbreversed;			//调色板信息
}RGBQUAD;

int infodealer(BMPHEADER *header, BMPINFO *info, u8 *h);

int main(int argc, char *argv[])
{

	errno_t err;
	FILE *fp;
	BMPHEADER bmpHeader;
	BMPINFO bmpInfo;
	u8 bmpPreHeader[len_header + len_info];
	if ((err = fopen_s(&fp, "in.bmp", "rb")) != 0)	//以二进制打开Bmp并判断是否打开成功
	{
		switch (err)
		{
		case 1:
			printf("无权限访问\n");
			break;
		case 2:
			printf("目录或文件不存在\n");
			break;
		}
	}
	else
	{
		size_t h;
		printf("文件已打开\n");
		h = fread(bmpPreHeader, len_header + len_info, 1, fp); //将数据流导入数组
		if (h)	//判断是否读取文件流成功
		{
			printf("文件已读取\n");
			infodealer(&bmpHeader,&bmpInfo,bmpPreHeader); //将数组的信息转码成结构
			if (bmpHeader.bfType == bm)	//判断文件头类型
			{
				printf("类型确认：BMP\n");
				printf("文件大小：%3.2fMb\n", (float)bmpHeader.bfSize/byte2mb);
				printf("数据偏移量为：%d字节\n", bmpHeader.bfOffBits);
				printf("信息头大小为：%d字节\n", bmpInfo.biSize);
				printf("图像大小为：%dx%d\n", bmpInfo.biWidth,bmpInfo.biHeight);
				printf("每个像素占：%d位\n", bmpInfo.biBitCount);
			}
			else
			{
				printf("不是标准的BMP文件\n");
			}
		}
		else
		{
			printf("文件读取失败");
		}
		

	}
	return 0;
}
int infodealer(BMPHEADER *header, BMPINFO *info, u8 *h)
{
	header->bfType = h[0x1] * onebyte + h[0x0];
	header->bfSize = h[0x5] * threebyte + h[0x4] * twobyte + h[0x3]*onebyte + h[0x2];
	header->bfOffBits = h[0xd] * threebyte + h[0xc] * twobyte + h[0xb] * onebyte + h[0xa];
	//处理文件头
	info->biSize = h[0x11] * threebyte + h[0x10] * twobyte + h[0xf] * onebyte + h[0xe];
	info->biWidth = h[0x15] * threebyte + h[0x14] * twobyte + h[0x13] * onebyte + h[0x12];
	info->biHeight = h[0x19] * threebyte + h[0x18] * twobyte + h[0x17] * onebyte + h[0x16];
	//info->biPlanes = h[0x1b] * onebyte + h[0x1a];
	//位面数 非必要
	info->biBitCount = h[0x1d] * onebyte + h[0x1c];
	info->biCompression = h[0x25] * threebyte + h[0x24] * twobyte + h[0x23] * onebyte + h[0x22];
	info->biSizeImage = h[0x29] * threebyte + h[0x28] * twobyte + h[0x27] * onebyte + h[0x26];
	info->biXpelsPerMeter = h[0x2d] * threebyte + h[0x2c] * twobyte + h[0x2b] * onebyte + h[0x2a];
	info->biYpelsPERMeter = h[0x31] * threebyte + h[0x30] * twobyte + h[0x2f] * onebyte + h[0x2e];
	info->biClrImportant = h[0x35] * threebyte + h[0x34] * twobyte + h[0x33] * onebyte + h[0x32];
	//处理信息头
	return 0;
}