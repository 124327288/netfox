#define FACE_ARRAY_NUM      20 //20����

#define RTF_CTRL_ID   WM_USER+101

//���ͼƬ�ļ��ļ���
#define    FACE_PIC_FOLDER_STR            "CustomFaceRecv"
#define    SEND_FILE_MAX_SIZE     1024*500 //������ļ����ܴ���500k
//����λ�ö���

#define ONE_PLAYER_WIDTH       178  //���
#define ONE_PLAYER_HIGH        176  //�߶�
#define ONE_PLAYER_BLANK_W        20  //���
#define ONE_PLAYER_BLANK_H      15  //���
#define PLAYER_ROW_NUM              5   //ÿһ����Ŀ
#define PLAYER_COL_NUM          2  //����

#define VIDEO_SHOW_ADD_W       10  //������
#define VIDEO_SHOW_ADD_H       25  //

//#define BK_PLAYER_WIDTH       178   //���
//#define BK_PLAYER_HIGH        176  //�߶�
//#define BK_PLAYER_BLANK_W        15  //���
//#define BK_PLAYER_BLANK_H       15  //���

//������ťλ������
#define COMMON_BUT_ROW_NUM     4
#define COMMON_BUT_COL_NUM     2
#define COMMON_BUT_WIDTH       90
#define COMMON_BUT_HIGH        35
#define COMMON_BUT_BLANK_W     10
#define COMMON_BUT_BLANK_H     10



//��chat room
// w= 1, 3, 1
// h = 1,1,3
//SCREEN_W
//SCREEN_H
#define CHAT_ROOM_BLANK_W   5  //���
#define CHAT_ROOM_BLANK_H   5  //���


//���ö���
#define DEFAULT_ROLL_STR  "��ӭ����!���ﴴ���ṩ����֧��(cctvdodo.zj.com)"

//UDP socket 
//��������,����,��Ƶ,�ļ�����
//�����˿ڷ���,��������,����,�ļ�����
#define ONE_ROOM_MIC_LIST_MAX_NUM  10 //ĳ��������������


//VIDEO
#define VIDEO_PIC_W    176
#define VIDEO_PIC_H    170

#define VIDEO_SHOW_W    176
#define VIDEO_SHOW_H    144

//�����ļ�
#define ACTION_FILE_LENGTH  1024*3  //3K
#define ACTION_FILE_NAME   "Chat//Actions.txt"

//�û��б�
#define CHAT_ROOM_USER_LIST_COL_NUM  3 //3��

//
#define CLIENT_REC_MESSAGE    10001 //�ͻ��˽��յ���Ϣ
//#define WM_CMD_REFRUSH_TREE   90000  //ˢ����ͼ
//MAIN
#define FLY_CMD_REFRUSH_TREE   30001  //ˢ����ͼ  90001������
#define FLY_CMD_REFRUSH_LIST   30002  //ˢ��List  90001������
#define FLY_CMD_REFRUSH_LIST_ONLY_ONE   30003  //ˢ��List ��ѯ����

//CHATROOM
#define FLY_CMD_REFRUSH_ROOM_PLAYER_LIST   40000  //ˢ��List  
#define FLY_CMD_CHAT_INSERT_ONE_MSG 30003 // WM_USER+40000  //����һһ�������¼ ������??
#define FLY_CMD_REFRUSH_ONE_ROOM_INFO   40002  //ˢ�·�������  
#define FLY_CMD_REFRUSH_ROOM_MIC_LIST   40003  //ˢ��M����  
#define FLY_CMD_REFRUSH_ONE_ROOM_LIST   40004  //ˢ��һ�������б�  
#define FLY_CMD_REFRUSH_KIND_LIST   40005  //ˢ��һ�������б� 
#define FLY_CMD_INTO_ONE_ROOM   40006  //ˢ��һ�������б�   
#define FLY_CMD_SHOW_FACE_PIC   40007  //��ʾ���ͼ
#define FLY_CMD_START_SEND_HEART_MSG   WM_USER+ 4008 //40008  //������������
#define FLY_CMD_REFRUSH_SOUND_CTRL   40009  //�䶯����������

//#define ROOM_LIST_COL_NUM    11   //�����б�COL��
#define ROOM_COL_NUM    13+3+1
#define DB_ROOM_COL_NUM    13+3   //���ݿ�COL
 //
#define GRID_COL_NUM   8
#define GRID_COL_ROOM_ID  2 //�����������
#define GRID_COL_ADD_ROOM  6 //��ӷ���
#define GRID_COL_INTO_ROOM  7 //���뷿��

//179,241,169--->������ɫ
//210,255,210--->���ɫ
//0,176,157--->��ͼɫ
#define GRID_BACK_COLOR RGB(179,241,169) //��ɫ
#define GRID_ROW_COLOR RGB(210,255,210)

#define DLG_ALL_BACK_COLOR RGB( 213,222, 242)//��ɫ

//���ö���

#define MAX_FRIENDS_NUM 50   //��������
//home 4.0
//#define XML_DOC_VERSION "MSXML2.DOMDocument.3.0"

//
#define SELECT_ONLINE_PLAYER_COL_NUM  2


//�˵�
#define ID_GFX_VOIPITEM   1000+1
//#define _ID_GFX_FILEITEM	"�����ļ�"
#define ID_GFX_FILEITEM    1000+2

/*
typedef struct OneRowData
{
	CString headStr;
	int imageInt;
	int colWidth;
} oneRowData;
*/

//Add by 2006-10-3
#define MAX_SEND_CHAT_MSG_NUM 5  //5s,5
#define MAX_GET_CHAT_MSG_NUM  25
#define MAX_GET_CHAT_MSG_NUM_TEN  10  //10������ 
//End add



//�ؼ���ʾλ��
#define REG_CONTROL_X    73
#define REG_CONTROL_Y    70
#define REG_CONTROL_H     20 //�߶�
#define REG_CONTROL_W     95 //chang��
#define REG_CONTROL_W1     120 //chang��

#define REG_CONTROL_Y_PWD    110
#define REG_CONTROL_Y_PWD1    270


#define REG_BUTTON_X    60
#define REG_BUTTON_Y    180
#define REG_BUTTON_H     23 //�߶�
#define REG_BUTTON_W     83 //chang��