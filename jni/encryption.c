
//#include <string.h>
//#include <jni.h>
#include <android/log.h>
#include "com_example_jniTestDGZ_CallNativeC.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MAX_LENGTH 4                                               //将128位二进制分割成4段，每段32bit
#define BOUNDARY   0xffffffff                                      //每段的最大值
#define OPERAND    4294967296                                      //进位时，逢2的32次方，向前段进1
#define WEISHU     256

#define LOG_TAG "test-encryption"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG ,LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  ,LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  ,LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR ,LOG_TAG, __VA_ARGS__)

void Initialization_Input_values(unsigned int *initi);             //随机数初始化输入
void ILMAP(unsigned int *Xt);                                      //将unsigned int *Xt计算一次所得到的值再置于unsigned int *Xt
void MakeKey(unsigned int *Mkey,unsigned int *KeyInfo,unsigned int *initm);            //256bit进行搅拌

JNIEXPORT jdouble JNICALL Java_com_example_jniTestDGZ_CallNativeC_encryption
  (JNIEnv *env, jobject obj)
{
	LOGD("call main function...");
	clock_t  begin,end;                                            //获取程序运行的开始时刻、结束时刻
	double   cost;                                                 //获取程序运行的时间，单位是秒
	unsigned int KeyInfoArr[4*MAX_LENGTH]={0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf};
	unsigned int MKeyArr[MAX_LENGTH];
	unsigned int Minit[2*MAX_LENGTH];

	//Initialization_Input_values(Minit);
	begin = clock();
	int i;
	for(i=0;i<100;i++)
		MakeKey(MKeyArr,KeyInfoArr,Minit);
	//return(*env)->NewStringUTF(env, MakeKey(MKeyArr,KeyInfoArr,Minit));
	end = clock();
	cost = (double)(end-begin)/CLOCKS_PER_SEC;

	LOGD("end....");
	return cost;
	//return(*env)->NewStringUTF(env, "first test jni ,,test logd ---from dgz");
	//LOGD("Program running time is：%f seconds\n", cost);
	//return (*env);

	//exit(0);
}

void Initialization_Input_values(unsigned int *initi)
{
	srand((unsigned int)time(NULL));
	//使用系统时间来作为随机数生成的种子
	int i;
	for(i=0;i<2*MAX_LENGTH;i++)
	{
		initi[i]=(unsigned int)((long double)BOUNDARY*rand()/RAND_MAX);
	}
}
void ILMAP(unsigned int *Xt)
{
	unsigned int ForwardCarryValue1,ForwardCarryValue2;
	unsigned long long int AfterInvePlusOne[MAX_LENGTH];
	unsigned long long int ToArray[MAX_LENGTH];
	unsigned long long int FinalResult[2*MAX_LENGTH];
	unsigned long long int TempValue[12];
	int i;
	for(i=0;i<MAX_LENGTH;i++)
	{
		ToArray[i]=Xt[i];
	}
	for(i=0;i<MAX_LENGTH;i++)                                   //求反
	{
		AfterInvePlusOne[i]=BOUNDARY-Xt[i];
	}
	AfterInvePlusOne[MAX_LENGTH-1]=AfterInvePlusOne[MAX_LENGTH-1]+1;//加1
	for(i=MAX_LENGTH-1;i>=1;i--)                                //判断加1后是否需要进位
	{
		if (AfterInvePlusOne[i]>BOUNDARY)
		{
			ForwardCarryValue1=AfterInvePlusOne[i]/OPERAND;
			AfterInvePlusOne[i-1]=AfterInvePlusOne[i-1]+ForwardCarryValue1;
			AfterInvePlusOne[i]=AfterInvePlusOne[i]-ForwardCarryValue1*OPERAND;
			continue;
		}
		else break;
	}
	AfterInvePlusOne[0]=AfterInvePlusOne[0]-(AfterInvePlusOne[0]/OPERAND)*OPERAND;

	TempValue[0]=ToArray[0]*AfterInvePlusOne[0];                        //做乘法运算
	TempValue[1]=TempValue[0]/OPERAND;
	TempValue[2]=TempValue[0]-TempValue[1]*OPERAND;
	FinalResult[0]=TempValue[1];
	FinalResult[1]=TempValue[2];

	TempValue[0]=ToArray[0]*AfterInvePlusOne[1];
	TempValue[1]=ToArray[1]*AfterInvePlusOne[0];
	TempValue[2]=ToArray[0]*AfterInvePlusOne[1]/OPERAND;
	TempValue[3]=ToArray[1]*AfterInvePlusOne[0]/OPERAND;
	TempValue[4]=TempValue[0]-TempValue[2]*OPERAND;
	TempValue[5]=TempValue[1]-TempValue[3]*OPERAND;
	FinalResult[1]+=TempValue[2]+TempValue[3];
	FinalResult[2]=TempValue[4]+TempValue[5];

	TempValue[0]=ToArray[0]*AfterInvePlusOne[2];
	TempValue[1]=ToArray[1]*AfterInvePlusOne[1];
	TempValue[2]=ToArray[2]*AfterInvePlusOne[0];
	TempValue[3]=ToArray[0]*AfterInvePlusOne[2]/OPERAND;
	TempValue[4]=ToArray[1]*AfterInvePlusOne[1]/OPERAND;
	TempValue[5]=ToArray[2]*AfterInvePlusOne[0]/OPERAND;
	TempValue[6]=TempValue[0]-TempValue[3]*OPERAND;
	TempValue[7]=TempValue[1]-TempValue[4]*OPERAND;
	TempValue[8]=TempValue[2]-TempValue[5]*OPERAND;
	FinalResult[2]+=TempValue[3]+TempValue[4]+TempValue[5];
	FinalResult[3]=TempValue[6]+TempValue[7]+TempValue[8];

	TempValue[0]=ToArray[0]*AfterInvePlusOne[3];
	TempValue[1]=ToArray[1]*AfterInvePlusOne[2];
	TempValue[2]=ToArray[2]*AfterInvePlusOne[1];
	TempValue[3]=ToArray[3]*AfterInvePlusOne[0];
	TempValue[4]=ToArray[0]*AfterInvePlusOne[3]/OPERAND;
	TempValue[5]=ToArray[1]*AfterInvePlusOne[2]/OPERAND;
	TempValue[6]=ToArray[2]*AfterInvePlusOne[1]/OPERAND;
	TempValue[7]=ToArray[3]*AfterInvePlusOne[0]/OPERAND;
	TempValue[8]=TempValue[0]-TempValue[4]*OPERAND;
	TempValue[9]=TempValue[1]-TempValue[5]*OPERAND;
	TempValue[10]=TempValue[2]-TempValue[6]*OPERAND;
	TempValue[11]=TempValue[3]-TempValue[7]*OPERAND;
	FinalResult[3]+=TempValue[4]+TempValue[5]+TempValue[6]+TempValue[7];
	FinalResult[4]=TempValue[8]+TempValue[9]+TempValue[10]+TempValue[11];

	TempValue[0]=ToArray[1]*AfterInvePlusOne[3];
	TempValue[1]=ToArray[2]*AfterInvePlusOne[2];
	TempValue[2]=ToArray[3]*AfterInvePlusOne[1];
	TempValue[3]=ToArray[1]*AfterInvePlusOne[3]/OPERAND;
	TempValue[4]=ToArray[2]*AfterInvePlusOne[2]/OPERAND;
	TempValue[5]=ToArray[3]*AfterInvePlusOne[1]/OPERAND;
	TempValue[6]=TempValue[0]-TempValue[3]*OPERAND;
	TempValue[7]=TempValue[1]-TempValue[4]*OPERAND;
	TempValue[8]=TempValue[2]-TempValue[5]*OPERAND;
	FinalResult[4]+=TempValue[3]+TempValue[4]+TempValue[5];
	FinalResult[5]=TempValue[6]+TempValue[7]+TempValue[8];

	TempValue[0]=ToArray[2]*AfterInvePlusOne[3];
	TempValue[1]=ToArray[3]*AfterInvePlusOne[2];
	TempValue[2]=ToArray[2]*AfterInvePlusOne[3]/OPERAND;
	TempValue[3]=ToArray[3]*AfterInvePlusOne[2]/OPERAND;
	TempValue[4]=TempValue[0]-TempValue[2]*OPERAND;
	TempValue[5]=TempValue[1]-TempValue[3]*OPERAND;
	FinalResult[5]+=TempValue[2]+TempValue[3];
	FinalResult[6]=TempValue[4]+TempValue[5];

	TempValue[0]=ToArray[3]*AfterInvePlusOne[3];
	TempValue[1]=TempValue[0]/OPERAND;
	TempValue[2]=TempValue[0]-TempValue[1]*OPERAND;
	FinalResult[6]+=TempValue[1];
	FinalResult[7]=TempValue[2];
	for ( i=0;i<2*MAX_LENGTH;i++)                                //乘以4
	{
		FinalResult[i]=FinalResult[i]<<2;
	}
	for (i=2*MAX_LENGTH-1;i>=1;i--)                             //乘以4后，判断是否要进位
	{
		if (FinalResult[i]>BOUNDARY)
		{
			ForwardCarryValue2=FinalResult[i]/OPERAND;
			FinalResult[i-1]=FinalResult[i-1]+ForwardCarryValue2;
			FinalResult[i]=FinalResult[i]-ForwardCarryValue2*OPERAND;
		}
	}
	FinalResult[0]=FinalResult[0]-(FinalResult[0]/OPERAND)*OPERAND;
	for (i=0;i<2*MAX_LENGTH;i++)
	{
		Xt[i]=(unsigned int)FinalResult[i];
	}
}
void MakeKey(unsigned int *Mkey,unsigned int *KeyInfo,unsigned int *initm)
{
	int i;
		for(i=0;i<4*MAX_LENGTH;i++)
		{
			int j;
			for(j=0;j<=KeyInfo[i];j++)
			{
				ILMAP(initm);
			}

			Mkey[0]=initm[0]^initm[MAX_LENGTH+0];
			Mkey[1]=initm[1]^initm[MAX_LENGTH+1];
			Mkey[2]=initm[2]^initm[MAX_LENGTH+2];
			Mkey[3]=initm[3]^initm[MAX_LENGTH+3];
		}
		LOGI("128bit encryption keys are: \n");//************************************************************************
		for(i=0;i<MAX_LENGTH;i++)//********************************************************************
		{
			LOGI("%08x\t",Mkey[i]);
		}
		//printf("\n");//**************************************************************************************

}

