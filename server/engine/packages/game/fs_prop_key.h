#ifndef _FS_PROP_KEY_H_
#define _FS_PROP_KEY_H_


// ����һЩȫ����Ҫ���ʵ�key, ��ű�����һ��
// ��ʱ����ң�NPC�ȷ���һ��
// ��ʽ :  
//		I_USER_xx, C_USER_xx
//		I_NPC_xx, C_NPC_xx
//		I_ITEM_xx, C_NPC_xx,
//		I_UTIL_xx(ͨ��)
//


// user
#define K_USER_LAYOUT           "layout"      /*����*/ 
#define K_USER_DIR              "dir"         /*����*/
#define K_USER_NAME             "name"        /*����*/                                                                                                                                                        
#define KT_USER_TITLE          "title"       /*��ν*/
#define KT_USER_EFFECTS        "effects"     /*������Ч*/
#define KT_USER_NAME_COLOR     "nameColor"   /*������ɫ*/
#define KT_USER_TITLE_COLOR    "titleColor"  /*��ν��ɫ*/

// npc
#define K_NPC_ICON              "icon"        /*��Դ���*/
#define K_NPC_NAME              "name"        /*����*/
#define K_NPC_TITLE             "title"       /*��ν*/
#define K_NPC_VISIBLE           "visible"     /*��˭�ɼ�, -1�򲻿ɼ�, 0�ɼ�����ʶ�����Ϊ����Ҳ��ܿ���*/
#define K_NPC_EFFECTS           "effects"     /*NPC��Ч*/
#define K_NPC_STORY_FLAG        "storyFlag"   /*����NPC��־*/
#define K_NPC_DIR               "dir"         /*����*/
#define K_NPC_NAME_COLOR        "nameColor"   /*������ɫ*/
#define K_NPC_TITLE_COLOR       "titleColor"  /*��ν��ɫ*/

// summon
#define K_SUMM_RES              "res"         /*��Դ���*/
#define K_SUMM_NAME             "name"        /*����*/

#endif
