//@River-super

//注意！ ：
//代码按照STC89C52RC SSD1306 128*64OLED 定制
//并不代表仅支持这些硬件
//可对代码进行更改以适配您的硬件

//代码仅支持I2C通讯协议的OLED屏



#include <STC89C5xRC.H>		//STC89C52RC的仿真头文件
#include <intrins.h> 

typedef unsigned char uchar;

sbit SDA = P1^0;
sbit SCL = P1^1;

void I2Cstart()	//I2C开始通讯
{
	SDA=0;
	SCL=0;
	SDA=1;
	//_nop_();
	SCL=1;
	//_nop_();
	SDA=0;
	//_nop_();
	SCL=0;
}

void I2Cstop()	//I2C结束通讯
{
	SDA=0;
	SCL=0;
	SDA=0;
	//_nop_();
	SCL=1;
	//_nop_();
	SDA=1;
	//_nop_();
	SCL=0;
}

void I2Ctell(uchar Tdata)	//I2C通讯内容
{
	int i;
	for(i=0;i<8;i++)
	{
		int Idata = (Tdata & 0x80)? 1 : 0;
		SDA=0;
		SCL=0;
		SDA=Idata;
		//_nop_();
		SCL=1;
		//_nop_();
		SDA=Idata;
		//_nop_();
		SCL=0;
		Tdata<<=1;
	}
	
}

bit I2Cwait()	//I2C等待回应
{
	bit ack;
  SDA = 1;
  //_nop_();
  SCL = 1;
  //_nop_();
  ack = SDA;
  SCL = 0;
  //_nop_();
	return ack;
}

void I2Cprint(uchar ip,uchar Dcmd)	//通讯整合
{
	I2Cstart();
	I2Ctell(0x78);
	I2Cwait();
	I2Ctell(ip);
	I2Cwait();
	I2Ctell(Dcmd);
	I2Cwait();
	I2Cstop();
} 

void OLEDstart()	//屏幕初始化
{
	I2Cprint(0x00,0xAE);
	I2Cprint(0x00,0x8D);I2Cprint(0x00,0x14);	  
	I2Cprint(0x00,0xD5);I2Cprint(0x00,0x80);
	I2Cprint(0x00,0xA8);I2Cprint(0x00,0x3F);
	I2Cprint(0x00,0xD3);I2Cprint(0x00,0x00);
	I2Cprint(0x00,0x40);
	I2Cprint(0x00,0xA1);
	I2Cprint(0x00,0xC8);
	I2Cprint(0x00,0xDA);I2Cprint(0x00,0x12);
	I2Cprint(0x00,0x81);I2Cprint(0x00,0xFF);
	I2Cprint(0x00,0xD9);I2Cprint(0x00,0xF3);
	I2Cprint(0x00,0xD8);I2Cprint(0x00,0x3F);
	I2Cprint(0x00,0x20);I2Cprint(0x00,0x00);
	I2Cprint(0x00,0xA4);
	I2Cprint(0x00,0xA6);
	I2Cprint(0x00,0xAF);
}

void I2CVisitPage(uchar page,uchar col)	//屏幕页定义
{
	I2Cprint(0x00,0xb0 + page);
	I2Cprint(0x00,((col & 0xf0) >> 4) | 0x10);
	I2Cprint(0x00,col & 0x0f);
}

void OLEDAllLight()	//屏幕全亮
{
	int pi,pj;
	I2CVisitPage(0,0);
	I2Cstart();
	I2Ctell(0x78);
	I2Cwait();
	I2Ctell(0x40);
	I2Cwait();
	for(pi=0;pi<8;pi++)
	{
		for(pj=0;pj<128;pj++)
		{
			I2Ctell(0xFF);
			I2Cwait();
		}
	}
	I2Cstop();
}

void OLEDAllDark()	//屏幕全暗
{
	int pi,pj;
	I2Cstart();
	I2Ctell(0x78);
	I2Cwait();
	I2Ctell(0x40);
	I2Cwait();
	for(pi=0;pi<8;pi++)
	{
		for(pj=0;pj<128;pj++)
		{
			I2Ctell(0x00);
			I2Cwait();
		}
	}
	I2Cstop();
}

void OLEDDrawPicture(uchar Pdata[])	//显示图片（128*64）
{
	int pi;
	I2CVisitPage(0,0);
	I2Cstart();
	I2Ctell(0x78);
	I2Cwait();
	I2Ctell(0x40);
	I2Cwait();
	for(pi=0;pi<1024;pi++)
	{
		I2Ctell(Pdata[pi]);
		I2Cwait();
	}
	I2Cstop();
}
