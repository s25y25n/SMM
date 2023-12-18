//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"


#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"



//board configuration parameters

static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;

typedef struct player {
        int energy;
        int position;
        char name[MAX_CHARNAME];
        int accumCredit;
        int flag_graduate;
} player_t;


static player_t *cur_player;
//static player_t cur_player[MAX_PLAYER];

#if 0
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PLAYER];
static char player_name[MAX_PLAYER][MAX_CHARNAME];
#endif

//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated
void generatePlayers(int n, int initEnergy); //generate a new player
void printGrades(int player); //print grade history of the player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif

smmObjGrade_e getRandomGrade(void)
{ //학점을 랜덤으로 받아오는 함수 
    srand(time(NULL));
    int randomIndex = rand() % 8;  
    return (smmObjGrade_e)randomIndex;
}
#if 0 
smmGrade_e takeLecture(int player, char *lectureName, int credit){//랜덤으로 강의 수강하는 함수 
           
}
#endif

void printGrades(int player) // 이거 응용해서 들은 과목인지 확인하는 코드도 짜게 될 것. 
{
     int i;
     void *gradePtr;
     for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
         printf("%s : %i\n",smmObj_getNodeName(gradePtr),smmObj_getNodeGrade(gradePtr));
     }
     
}

void printPlayerStatus(void)
{
     int i;
     
     for (i=0;i<player_nr;i++)
     {
         printf("%s : credit %i, energy %i, position %i\n", 
                      cur_player[i].name,
                      cur_player[i].accumCredit,
                      cur_player[i].energy,
                      cur_player[i].position);
     }
}



void generatePlayers(int n, int initEnergy)//generate a new player
{
     //n time loop 
     int i;
     for(i=0;i<n;i++) {
           
           // input name
         printf("Input player %i's name:", i+1); //¾E³≫ ¹®±¸ 
         scanf("%s",cur_player[i].name);
         fflush(stdin);
    
    //set position
    cur_player[i].position= 0;
    
    //set energy
    cur_player[i].energy = initEnergy;
    cur_player[i].accumCredit = 0;
    cur_player[i].flag_graduate = 0;
    }
}


int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): \n");
    c = getchar();
    fflush(stdin);
    
#if 1
    if (c == 'g')
        printGrades(player);
#endif
    
    return (rand()%MAX_DIE + 1);
}


//action code when a player stays at a node
void actionNode(int player)
{
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    int type = smmObj_getNodeType(boardPtr);
    char *name = smmObj_getNodeName(boardPtr);
    void* gradePtr; 
    
    printf("Debug: Current player position: %d, Node type: %d, Node name: %s\n", cur_player[player].position, type, name);
    
    switch(type)
    {
    // case 강의수강 :
        case SMMNODE_TYPE_LECTURE :
             
             if(smmObj_getNodeCredit(boardPtr) > 0) 
             {
                                               
            cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
            cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
            
            //grade 생성      
        gradePtr = smmObj_genObject(name, smmObjType_grade , 0, smmObj_getNodeCredit(boardPtr), 0 , getRandomGrade() );
        smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
              }
          else {
                   break;
                }
          default:
                   break;
/*
         case SMMNODE_TYPE_RESTAURANT:
        // SMMNODE_TYPE_RESTAURANT에 대한 처리
        break;

    case SMMNODE_TYPE_LABORATORY:
         // SMMNODE_TYPE_LABORATORY에 대한 처리 (실험중 상태)
        if (rand() % 6 + 1 > 3) // 주사위가 4 이상이면 실험 성공
        {
            // 실험 성공 시 노드 이동
            cur_player[player].position = 0; // 예시로 홈 노드로 이동
        }
        else
        {
            // 실험 실패 시 아무 동작 없음
        }
        break;
        break;

    case SMMNODE_TYPE_HOME:
         cur_player[player].energy += 5;
        break;

    case SMMNODE_TYPE_GOTOLAB:
        if (cur_player[player].position == SMMNODE_TYPE_GOTOLAB)
        {
            cur_player[player].position = SMMNODE_TYPE_LABORATORY; // 실험중 상태에서만 노드 이동
        }
        break;
        
    case SMMNODE_TYPE_FOODCHANCE:
        // SMMNODE_TYPE_FOODCHANCE에 대한 처리
        break;
        
    case SMMNODE_TYPE_FESTIVAL:
        // SMMNODE_TYPE_FESTIVAL에 대한 처리
        break;


    default:
        // 알 수 없는 노드 타입에 대한 처리
        break;
    }
    */
}
}


void goForward(int player, int step)
{
     cur_player[player].position += step ;
     void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
     
     printf("%s go to node %i(name: %s)\n", cur_player[player].name, cur_player[player].position, 
     smmObj_getNodeName(boardPtr));
     
}




int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i,j,k;
    int initEnergy;
    int turn = 0;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while (fscanf(fp, "%s%i%i%i", name, &type, &credit, &energy) == 4 ) //read a node parameter set
    {
        //store the parameter set
        smmObjType_e objtype = smmObjType_board;
        smmObject_t *boardObj = (smmObject_t*)smmObj_genObject(name, objtype , type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        
        if(type == SMMNODE_TYPE_HOME)
            initEnergy = energy;
            board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
   
     for (i = 0;i<board_nr;i++)
{
     void* boardObj = smmdb_getData(LISTNO_NODE, i);
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                     i, smmObj_getNodeName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
}
   // printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
   
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s%d", name, &energy) == 2) //read a food parameter set
    {
        //store the parameter set
        FoodCard *foodObj = (FoodCard *)malloc(sizeof(FoodCard));
          strcpy(foodObj->name, name);
          foodObj->energy = energy;
        smmdb_addTail(LISTNO_FOODCARD, foodObj);
        food_nr++;
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    
    for (j = 0; j<food_nr; j++)
{
    FoodCard *foodCard = (FoodCard *)smmdb_getData(LISTNO_FOODCARD, j);
    printf("%i. name: %s, energy: %i\n", j, foodCard->name, foodCard->energy);
}
    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", name) == 1) //read a festival card string
    {
       //store the parameter set
        FestivalCard *festObj = (FestivalCard *)malloc(sizeof(FestivalCard));
          strcpy(festObj->name, name);
        smmdb_addTail(LISTNO_FESTCARD, festObj);
        festival_nr++;
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
  
   for (k = 0; k<festival_nr; k++)
{
    FestivalCard *festCard = (FestivalCard *)smmdb_getData(LISTNO_FESTCARD, k);
    printf("%i. %s\n", k, festCard->name);
}
    
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
            printf("\nHow many people are playing? :\n");
            scanf("%d", &player_nr);
            fflush(stdin);
    } while (player_nr < 0 || player_nr > MAX_PLAYER);
     
     cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
     generatePlayers(player_nr, initEnergy);
    
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)
        //rolldie 함수 사용 
        die_result = rolldie(turn);
        
        //4-3. go forward
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn+1)%player_nr;
    }
    
   return 0;
}







