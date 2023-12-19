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

#if 0
void saveGrade(int player, const char *lectureName, smmObjGrade_e grade)
{
    // �ش� �÷��̾��� ���� �̷� �迭�� ����
    GradeRecord *record = &gradeHistory[player][gradeHistoryCount[player]];
    strncpy(record->lectureName, lectureName, MAX_CHARNAME - 1);
    record->grade = grade;

    // ���� �̷� ���� ����
    gradeHistoryCount[player]++;
}
#endif


static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = //�迭�� ũ��� �ƽ�ĳ���ͳ������� ���� 
{
       "����", 
       "�Ĵ�",
       "�����",
       "��",
       "����Ƿ� �̵�",
       "��������",
       "�����ð�"
       
};// ���Ӻ��� ����� �̸� �迭  



char* smmObj_getRandomGrade(int type) 
{
      return (char*)smmGradeName[type];
      
} //���ǿ� ���� ���� ���� ��ȯ 

char* smmObj_getTypeName(int type) 
{
      return (char*)smmNodeName[type];
      
} //��� ������ ���� �̸� ��ȯ 


//FEST OBJECT

char* smmObj_getFestName(void* obj)
{
      FestivalCard* ptr = (FestivalCard*)obj;
      return ptr->name;
} // ����ī�� ��ȯ  


//FOOD OBJECT 

char* smmObj_getFoodName(void* obj)
{
      FoodCard* ptr = (FoodCard*)obj;
      return ptr->name;
}//����ī�� �̸���ȯ  

int smmObj_getFoodEnergy(void* obj)
{
    FoodCard* ptr = (FoodCard*)obj;
      return ptr->energy;
}//����ī�� ��������ȯ  


//NODE OBJECT


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
} // ���Ӱ�ü ����, ��ȯ  

   //free(ptr);
   return ptr;
}

char* smmObj_getNodeName(void* obj)
{
      smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->name;
}//��ü�̸���ȯ  

int smmObj_getNodeType(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->type;
}//��ü���������ȯ  

int smmObj_getNodeCredit(void* obj)
{
   smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->credit;
}//��ü ������ȯ  

int smmObj_getNodeEnergy(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->energy;
}//��ü �ҿ� ������ ��ȯ 

int smmObj_getNodeGrade(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->grade;
} // ��� ���� ��ȯ 
