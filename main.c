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



/*
int isGraduated(void) {

    int i;
    for (i = 0; i < player_nr; i++) {
        if (cur_player[i].accumCredit < GRADUATE_CREDIT) {
            return 0; // 아직 누구도 졸업하지 않음
        }
    }
    return 1; // 모든 플레이어가 졸업함
}
*/



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
         printf("====================================\n%s : credit %i, energy %i, position %i\n====================================\n", 
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
    printf("\nPress any key to roll a die (press g to see grade): \n");
    c = getchar();
    fflush(stdin);
    

    if (c == 'g')
        printGrades(player);

    
    return (rand()%MAX_DIE + 1);
}


//action code when a player stays at a node
void actionNode(int player)
{
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    int type = smmObj_getNodeType(boardPtr);
    char *name = smmObj_getNodeName(boardPtr);
    void* gradePtr; 
    
    switch(type)
{
    case SMMNODE_TYPE_LECTURE:
    {
        void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);

        if (smmObj_getNodeCredit(boardPtr) > 0 && cur_player[player].energy >= smmObj_getNodeEnergy(boardPtr))
        {
                                           
            char choice;
            int validInput = 0;

            while (!validInput)
            {
                printf("Do you want to take the lecture? (y/n): ");
                scanf(" %c", &choice);

                if (choice == 'y' || choice == 'Y')
                {
                  // 수강하는 경우
                  printf("You've taken the lecture \"%s\".\n", smmObj_getNodeName(boardPtr));

                 cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
                  cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);

                  // grade 생성
                  gradePtr = smmObj_genObject(smmObj_getNodeName(boardPtr), smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, getRandomGrade());
                  smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
                  /*
                   grade = rand() % smmObjGrade_max;
                    gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, (smmObjGrade_e)grade);
                    smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
                  */
                  printf("Grade for \"%s\": %d\n", smmObj_getNodeName(boardPtr), smmObj_getNodeGrade(gradePtr));

                  validInput = 1; // 유효한 입력이므로 반복문 종료
                 }
                 else if (choice == 'n' || choice == 'N')
                 {
                     // 드랍하는 경우
                    printf("You've dropped the lecture \"%s\".\n", smmObj_getNodeName(boardPtr));
                    validInput = 1; // 유효한 입력이므로 반복문 종료
                  }
            else
                 {
                 // 다른 값 입력 시 처리
                printf("Invalid choice. Please enter 'y' or 'n'.\n");
                 }
              }
          }
    else
        {
        printf("Not enough energy to take the lecture.\n");
        }
    break;
   }
   
   case SMMNODE_TYPE_RESTAURANT:
   {   
        printf("You have reached a restaurant!\n");
         // 음식 카드 뽑기, 정보 출력  
        FoodCard *foodCard = (FoodCard *)smmdb_getData(LISTNO_FOODCARD, rand() % food_nr);
        printf("You drew a food card: %s, Energy: %d\n", foodCard->name, foodCard->energy);
        
        // 현재 플레이어의 에너지에 더하기
        cur_player[player].energy += foodCard->energy; 
             
        break;   
   }

    case SMMNODE_TYPE_LABORATORY:
   {   // (실험중 상태)
     void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
     if (smmObj_getNodeType(boardPtr) == SMMNODE_TYPE_LABORATORY && smmObj_getNodeType(smmdb_getData(LISTNO_NODE, cur_player[player].position - 1)) == SMMNODE_TYPE_GOTOLAB)
     {
         int energyCost = 3; // 실험 시도마다 소요되는 에너지
         int diceResult = rolldie(player);

         if (cur_player[player].energy >= energyCost) // 충분한 에너지가 있는지 확인
         {
            int escapeValue = rolldie(player) - 2;

            if (diceResult > escapeValue)
            {   // 실험 성공
                printf("Experiment successful! Moving to home.\n");
                cur_player[player].position = SMMNODE_TYPE_HOME; // 실험 성공 시 홈 노드로 이동
            }
            else
            {
                printf("Experiment failed! Staying in the laboratory.\n");
            }
          }
        cur_player[player].energy -= energyCost;
      }
    else
      {
        printf("This is not experiment time. You can go through this lab.\n");
       }
    break;
   }

    case SMMNODE_TYPE_HOME:
   { //지나가는 순간 에너지 보충 
       cur_player[player].energy += 7;
       break;
   }
     
    case SMMNODE_TYPE_GOTOLAB:
   {
        if (cur_player[player].position == SMMNODE_TYPE_GOTOLAB)
        {
            cur_player[player].position = SMMNODE_TYPE_LABORATORY;  // 실험실 노드로 이동
            
            int escapeValue = rolldie(player) - 2;
            printf("You've entered the laboratory.\n");
            printf("Escape value for the experiment: %d\n", escapeValue);
            
        }
        break;
   }
       
    case SMMNODE_TYPE_FOODCHANCE:
   {
        printf("You've got a food chance! Enter any number to draw a food card: ");
        int userInput;
        scanf("%d", &userInput);

        FoodCard *foodCard = (FoodCard *)smmdb_getData(LISTNO_FOODCARD, rand() % food_nr);
        
        printf("You've drawn a food card: %s\n", foodCard->name);
        cur_player[player].energy += foodCard->energy;
        
        break;
   }
     
    case SMMNODE_TYPE_FESTIVAL:
   {
        printf("You've reached a festival! Enter any number to draw a festival card: ");
        int userInput;
        scanf("%d", &userInput);

        FestivalCard *festCard = (FestivalCard *)smmdb_getData(LISTNO_FESTCARD, rand() % festival_nr);
        
        break;
   }

    default:
        break;
    }
   
}




void goForward(int player, int step)
{
     int i;
     printf("result: %d\n",step);
     for (i = 0; i<=step; ++i)
    {
        // 주사위로 이동한 노드의 포인터를 구함
        void *boardPtr = smmdb_getData(LISTNO_NODE, (cur_player[player].position + i) % board_nr);
        printf("  => Jump to %s\n", smmObj_getNodeName(boardPtr));
    }
       // 플레이어의 위치 업데이트
     cur_player[player].position += step ;
     
     // 마지막 노드에 도달한 경우 보드를 한 바퀴 돈 것으로 처리
   if (cur_player[player].position >= board_nr)
    {
        void *firstBoardNode = smmdb_getData(LISTNO_NODE, 0);
        cur_player[player].energy += smmObj_getNodeEnergy(firstBoardNode);
        cur_player[player].position %= board_nr;
    }
     // 플레이어의 현재 위치에 해당하는 노드 정보 출력
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







