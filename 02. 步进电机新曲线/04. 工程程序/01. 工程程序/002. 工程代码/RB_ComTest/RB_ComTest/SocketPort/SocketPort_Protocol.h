///////////////////////////////////////////////////////////////////////////////////////////////////
//���ڶ�������ͨѶʱ������֡��ʽ��������ӡ����ָ���ʹ�ñ��ڶ����֡��ʽ�⣨ֱ�����ݴ��䣩����������˿����Ӿ�ͳһʹ�ñ��ڶ��塣
//����֡��ʽ	0xF0,0xF1,cmd,cmdSize,[data],[data1],[data2],[��],0xEC
//
//  ��0xF0, 0xF1��	������֡��ʼ����
//  ��cmd��			����ǰ����ָ֡�����ͣ�BYTE��
//  ��cmdSize��		��ָ��������ȣ�WORD����ʾ����������Ч���ݣ�data, data1, ��, ������0xFC, CRCλ���ĸ�����
//  ��data, data1, data2,...��	��ָ�������BYTE�����ݡ�cmd����ͬ���ɡ�cmdSize��ָ���������������
//  ��0xEC��		������֡������
//  ������TCP����������Ƶ�����У�鹦�ܣ����ֻҪ���ݴ���ɹ�����ʾ��ǰָ��������ͣ�����ָ�����Ҫ��Э���ʽҪ�󷵻ز���״̬��Ϣ��ʹ��ԭ cmd ���������Ա������ط���ָ��ͷ���������Ӧ����

// cmdSize�� data ���ֲ���ֵ��ѭ���¶���Ҫ�󡣲�ͬ����������ֵ����ʱ������ѭ���¹���
//  ���������ݾ���������8λ����ֵ��BYTE�����ݷ�ʽ���д��䣬����ָ��е�16λ��32λ��ֵͨ���ֲ�/�ϲ����У�
//  ��8λ����ֵ(Byte)		������ʱֱ��ʹ�ò���ֵ��
//  ��16λ����ֵ(Word)	������ʱ˳��Ϊ[LoByte][HiByte]������8λ�ں󣬵�8λ��ǰ�����ϲ�Ϊһ��16λ�������� Data = [HiByte<<8] | [LoByte]��
//  ��32λ����ֵ(Dword)	������ʱ˳��Ϊ[LoWord][HiWord]������16λ�ں󣬵�16λ��ǰ�����ϲ�Ϊһ��32λ�������� Data = [HiWord<<16] | [LoWord]������ÿ��Word���� 16λ����ֵ��ʽ��ϣ��������Ĵ���ʱ˳��Ϊ [LoWord_LoByte][LoWord_HiByte][HiWord_LoByte][HiWord_HiByte]��
//  ��32λ�����ͣ�float������32λ���ʹ����ʽһ����
//
// modify by hzm 2015-09-27
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

//sizeof(pBits) >= 2
#define PROTOCOL_MAKEDATA_WORD(pBits)		WORD(((WORD)((pBits[1]) & 0xFF)<<8) | ((WORD)(pBits[0]) & 0xFF))
//sizeof(pBits) >= 4
#define PROTOCOL_MAKEDATA_DWORD(pBits)		DWORD(((DWORD)((pBits[3]) & 0xFF)<<24) | ((DWORD)((pBits[2]) & 0xFF)<<16) | ((DWORD)((pBits[1]) & 0xFF)<<8) | ((DWORD)((pBits[0]) & 0xFF)))
// dwData --> fData
#define PROTOCOL_MAKEDATA_DWORD_TO_FLOAT(fData,dwData)	\
{	\
	DWORD dwTmpForTrans = (dwData);				\
	(fData) = *(float*)(&dwTmpForTrans);		\
}
// fData --> dwData
#define PROTOCOL_MAKEDATA_FLOAT_TO_DWORD(dwData,fData)	\
{	\
	float fTmpForTrans = (fData);				\
	(dwData) = *(DWORD*)(&fTmpForTrans);		\
}

//sizeof(pBits) >= 2
#define PROTOCOL_GETDATA_WORD(wData,pBits)		\
{	\
	(pBits[0])	= (BYTE)(wData);				\
	(pBits[1])	= (BYTE)((WORD)(wData)>>8);		\
}
//sizeof(pBits) >= 4
#define PROTOCOL_GETDATA_DWORD(dwData,pBits)	\
{	\
	(pBits[0])	= (BYTE)(((DWORD)(dwData)) & 0xFF);			\
	(pBits[1])	= (BYTE)(((DWORD)(dwData)>>8) & 0xFF);		\
	(pBits[2])	= (BYTE)(((DWORD)(dwData)>>16) & 0xFF);		\
	(pBits[3])	= (BYTE)(((DWORD)(dwData)>>24) & 0xFF);		\
}

// ����֡��ʽ	0xF0,0xF1,cmd,cmdSize,[data],[data1],[data2],[��],0xEC
enum GCB_PROTOCOL_DATAFRAME_INDEX
{
	gcb_pt_data_start1	= 0,			//0xF0
	gcb_pt_data_start2	= 1,			//0xF1
	gcb_pt_data_cmd,					//cmd
	gcb_pt_data_cmdsize1,				//cmdSize = (cmdsize2<<8) | cmdsize1
	gcb_pt_data_cmdsize2,				//

	gcb_pt_data_startfixcount,			//����֡��ʼ�ι̶���������
	gcb_pt_data_endfixcount = 1,		//����֡�����ι̶���������

	gcb_pt_data_len_min	= gcb_pt_data_startfixcount+gcb_pt_data_endfixcount,			//����֡��С���ȣ�cmdSize=0�����������ֽ�ָ�
	gcb_pt_data_len_max = gcb_pt_data_len_min + 0xFFFF,			//����֡��󳤶ȣ�cmdSize=0xFFFF��
};
#define gcb_protocol_dataframe_start1				0xF0		//����֡��ʼ��ʶ1
#define gcb_protocol_dataframe_start2				0xF1		//����֡��ʼ��ʶ2
#define gcb_protocol_dataframe_end					0xEC		//����֡������ʶ


//======= GCB���ü���ӡ����ָ�� =======

// PC --> GCB --> PC
#define cmd_gcb_getversion			0x01		//�汾
#define cmd_gcb_getbuffersize		0x02		//�û�������С

#define cmd_gcb_init				0x10		//�忨��ʼ��
#define cmd_gcb_print_ready			0x11		//��ӡ׼��

//======= �忨����ָ�� =======
#define cmd_gcb_repeater_config		0x30		//�¼��忨����ָ�PC-->GCB-->�忨��
#define cmd_pc_repeater_config		0x31		//�¼��忨����ָ��忨-->GCB-->PC��

//======= ��ӡ����ָ�� =======
//������֡��ʽҪ��