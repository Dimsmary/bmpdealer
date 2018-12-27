#include <stdio.h>
#include <stdlib.h>
#define bm 0x4d42
#define len_header 14		//����ͷ�ļ�����
#define len_info 40			//������Ϣͷ����
#define onebyte 256
#define twobyte 65536
#define threebyte 16777216
#define fourbyte 4294967296
#define byte2mb 1048576
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char u8;
typedef unsigned int u16;
typedef struct BMP_HEADER	//����ͷ�ļ��ṹ
{
	WORD bfType;				//�ļ���ʶ��
	DWORD bfSize;				//����BMP�ļ��Ĵ�С
	WORD r1;
	WORD r2;					//����
	DWORD bfOffBits;			//ƫ����
}BMPHEADER;
typedef struct BMP_INFO		//������Ϣͷ
{
	DWORD biSize;				//��Ϣͷ�����ֽ���
	DWORD	biWidth;			//ͼ���ȣ����ص�λ��
	DWORD biHeight;			//�߶�
	WORD biPlanes;			//λ����
	WORD biBitCount;			//һ�����ص�ռ��bit
	DWORD biCompression;		//ѹ������
	DWORD biSizeImage;		//λͼ��С
	DWORD biXpelsPerMeter;	//ˮƽ�ֱ��� ����/��
	DWORD biYpelsPERMeter;	//��ֱ�ֱ���
	DWORD biClrUsed;			//ʵ��ʹ�õĵ�ɫ������
	DWORD biClrImportant;		//��ҪӰ�����ɫ������Ŀ
}BMPINFO;
typedef struct RGB_QUAD
{
	WORD rgbblue;
	WORD rgbgreen;
	WORD rgbred;
	WORD rgbreversed;			//��ɫ����Ϣ
}RGBQUAD;

int infodealer(BMPHEADER *header, BMPINFO *info, u8 *h);

int main(int argc, char *argv[])
{

	errno_t err;
	FILE *fp;
	BMPHEADER bmpHeader;
	BMPINFO bmpInfo;
	u8 bmpPreHeader[len_header + len_info];
	if ((err = fopen_s(&fp, "in.bmp", "rb")) != 0)	//�Զ����ƴ�Bmp���ж��Ƿ�򿪳ɹ�
	{
		switch (err)
		{
		case 1:
			printf("��Ȩ�޷���\n");
			break;
		case 2:
			printf("Ŀ¼���ļ�������\n");
			break;
		}
	}
	else
	{
		size_t h;
		printf("�ļ��Ѵ�\n");
		h = fread(bmpPreHeader, len_header + len_info, 1, fp); //����������������
		if (h)	//�ж��Ƿ��ȡ�ļ����ɹ�
		{
			printf("�ļ��Ѷ�ȡ\n");
			infodealer(&bmpHeader,&bmpInfo,bmpPreHeader); //���������Ϣת��ɽṹ
			if (bmpHeader.bfType == bm)	//�ж��ļ�ͷ����
			{
				printf("����ȷ�ϣ�BMP\n");
				printf("�ļ���С��%3.2fMb\n", (float)bmpHeader.bfSize/byte2mb);
				printf("����ƫ����Ϊ��%d�ֽ�\n", bmpHeader.bfOffBits);
				printf("��Ϣͷ��СΪ��%d�ֽ�\n", bmpInfo.biSize);
				printf("ͼ���СΪ��%dx%d\n", bmpInfo.biWidth,bmpInfo.biHeight);
				printf("ÿ������ռ��%dλ\n", bmpInfo.biBitCount);
			}
			else
			{
				printf("���Ǳ�׼��BMP�ļ�\n");
			}
		}
		else
		{
			printf("�ļ���ȡʧ��");
		}
		

	}
	return 0;
}
int infodealer(BMPHEADER *header, BMPINFO *info, u8 *h)
{
	header->bfType = h[0x1] * onebyte + h[0x0];
	header->bfSize = h[0x5] * threebyte + h[0x4] * twobyte + h[0x3]*onebyte + h[0x2];
	header->bfOffBits = h[0xd] * threebyte + h[0xc] * twobyte + h[0xb] * onebyte + h[0xa];
	//�����ļ�ͷ
	info->biSize = h[0x11] * threebyte + h[0x10] * twobyte + h[0xf] * onebyte + h[0xe];
	info->biWidth = h[0x15] * threebyte + h[0x14] * twobyte + h[0x13] * onebyte + h[0x12];
	info->biHeight = h[0x19] * threebyte + h[0x18] * twobyte + h[0x17] * onebyte + h[0x16];
	//info->biPlanes = h[0x1b] * onebyte + h[0x1a];
	//λ���� �Ǳ�Ҫ
	info->biBitCount = h[0x1d] * onebyte + h[0x1c];
	info->biCompression = h[0x25] * threebyte + h[0x24] * twobyte + h[0x23] * onebyte + h[0x22];
	info->biSizeImage = h[0x29] * threebyte + h[0x28] * twobyte + h[0x27] * onebyte + h[0x26];
	info->biXpelsPerMeter = h[0x2d] * threebyte + h[0x2c] * twobyte + h[0x2b] * onebyte + h[0x2a];
	info->biYpelsPERMeter = h[0x31] * threebyte + h[0x30] * twobyte + h[0x2f] * onebyte + h[0x2e];
	info->biClrImportant = h[0x35] * threebyte + h[0x34] * twobyte + h[0x33] * onebyte + h[0x32];
	//������Ϣͷ
	return 0;
}