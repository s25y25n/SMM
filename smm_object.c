//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>



#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100



static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = //�迭�� ũ��� �ƽ�ĳ���ͳ������� ���� 
{
       "����", 
       "�Ĵ�",
       "�����",
       "��",
       "����Ƿ� �̵�",
       "��������",
       "�����ð�"
       
};


char* smmObj_getRandomGrade(int type) 
{
      return (char*)smmGradeName[type];
      
} 

char* smmObj_getTypeName(int type) 
{
      return (char*)smmNodeName[type];
      
} 
//1. ����ü ���� ���� 
/*
typedef struct smmObject {
       
       char name[MAX_CHARNAME];
       smmObjType_e objtype;
       int type;
       int credit;
       int energy; 
       smmObjGrade_e grade;
} smmObject_t;
*/
//static int smmObj_noNode = 0 ;
//static smmObject_t smm_node[MAX_NODE];

//�����Լ� ���� 
//object generation

#if 0
typedef struct {
    char name[100]; // ���� ī���� �̸�
    int energy;     // ���� ī���� ������
} FoodCard;

char* smmObj_getFoodName(void* obj)
{
      FoodCard* ptr = (FoodCard*)obj;
      return ptr->name;
}
#endif


void* smmObj_genObject(char* name, smmObjType_e objtype ,int type, int credit, int energy, smmObjGrade_e grade)
{
    smmObject_t* ptr ;
    
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    if(ptr != NULL){
    
    strcpy(ptr->name, name);
    ptr->objtype = objtype;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
}

   //free(ptr);
   return ptr;
}

char* smmObj_getNodeName(void* obj)
{
      smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->name;
}

int smmObj_getNodeType(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->type;
}

int smmObj_getNodeCredit(void* obj)
{
   smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->credit;
}

int smmObj_getNodeEnergy(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->energy;
}

int smmObj_getNodeGrade(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->grade;
}
