//
//  smm_object.h
//  SMMarble object
//
//  Created by Juyeop Kim on 2023/11/05.
//
#include "smm_common.h"

#ifndef smm_object_h
#define smm_object_h

#define SMMNODE_TYPE_LECTURE            0
#define SMMNODE_TYPE_RESTAURANT         1
#define SMMNODE_TYPE_LABORATORY         2
#define SMMNODE_TYPE_HOME               3
#define SMMNODE_TYPE_GOTOLAB            4
#define SMMNODE_TYPE_FOODCHANCE         5
#define SMMNODE_TYPE_FESTIVAL           6

#define SMMNODE_TYPE_MAX                7
#define SMMGRADE_MAX                     9

typedef struct {
    char name[100]; // 음식 카드의 이름
    int energy;     // 음식 카드의 에너지
} FoodCard;


typedef struct {
    char name[100]; 
} FestivalCard;


static char smmGradeName[9][MAX_CHARNAME] = {
	"A+",
	"A0",
	"A-",
	"B+",
	"B0",
	"B-",
	"C+",
	"C0",
	"C-"
};


typedef enum smmObjType {
         smmObjType_board = 0,
         smmObjType_card,
         smmObjType_grade
}smmObjType_e ;

// 학점 열거형 정의
typedef enum smmObjGrade {
         smmObjGrade_Ap = 0,
         smmObjGrade_A0,
         smmObjGrade_Bp,
         smmObjGrade_B0,
         smmObjGrade_Bm,
         smmObjGrade_Cp,
         smmObjGrade_C0,
         smmObjGrade_Cm
}smmObjGrade_e;

typedef struct smmObject {
       
       char name[MAX_CHARNAME];
       smmObjType_e objtype;
       int type;
       int credit;
       int energy; 
       smmObjGrade_e grade;
} smmObject_t;



smmObjGrade_e getRandomGrade(void);


/* node type :
    lecture,
    restaurant,
    laboratory,
    home,
    experiment,
    foodChance,
    festival
*/

//성적을 저장하는 함수  
void saveGrade(int player, const char *lectureName, smmObjGrade_e grade);

//object generation
void* smmObj_genObject(char* name, smmObjType_e objtype ,int type, int credit, int energy, smmObjGrade_e grade);

//member retrieving
char* smmObj_getNodeName(void* obj);
int smmObj_getNodeType(void* obj);
int smmObj_getNodeCredit(void* obj);
int smmObj_getNodeEnergy(void* obj);

//element to string
char* smmObj_getTypeName(int type);


#endif /* smm_object_h */
