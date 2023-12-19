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
    // 해당 플레이어의 성적 이력 배열에 저장
    GradeRecord *record = &gradeHistory[player][gradeHistoryCount[player]];
    strncpy(record->lectureName, lectureName, MAX_CHARNAME - 1);
    record->grade = grade;

    // 성적 이력 개수 증가
    gradeHistoryCount[player]++;
}
#endif


static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = //배열의 크기는 맥스캐릭터네임으로 잡음 
{
       "강의", 
       "식당",
       "실험실",
       "집",
       "실험실로 이동",
       "음식찬스",
       "축제시간"
       
};// 게임보드 노드의 이름 배열  



char* smmObj_getRandomGrade(int type) 
{
      return (char*)smmGradeName[type];
      
} //강의에 대한 랜덤 성적 반환 

char* smmObj_getTypeName(int type) 
{
      return (char*)smmNodeName[type];
      
} //노드 유형에 대해 이름 반환 


//FEST OBJECT

char* smmObj_getFestName(void* obj)
{
      FestivalCard* ptr = (FestivalCard*)obj;
      return ptr->name;
} // 축제카드 반환  


//FOOD OBJECT 

char* smmObj_getFoodName(void* obj)
{
      FoodCard* ptr = (FoodCard*)obj;
      return ptr->name;
}//음식카드 이름반환  

int smmObj_getFoodEnergy(void* obj)
{
    FoodCard* ptr = (FoodCard*)obj;
      return ptr->energy;
}//음식카드 에너지반환  


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
} // 게임객체 생성, 반환  

   //free(ptr);
   return ptr;
}

char* smmObj_getNodeName(void* obj)
{
      smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->name;
}//객체이름반환  

int smmObj_getNodeType(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->type;
}//객체노드유형반환  

int smmObj_getNodeCredit(void* obj)
{
   smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->credit;
}//객체 학점반환  

int smmObj_getNodeEnergy(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->energy;
}//객체 소요 에너지 반환 

int smmObj_getNodeGrade(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
      return ptr->grade;
} // 노드 성적 반환 
