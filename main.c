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
        int exp;
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

static char* lectureHistory[MAX_CHARNAME] = {NULL};

// ������ ���� ���θ� Ȯ���ϴ� �Լ��� ����
int isLectureTaken(int player, const char* lectureName) 
{
     int i;
    // ���� ����� Ȯ���Ͽ� lectureName�� �̹� ������ �������� Ȯ��
    for (i=0;i<MAX_CHARNAME;++i) 
    {
        if (lectureHistory[i] != NULL && strcmp(lectureHistory[i], lectureName) == 0) 
        {
            return 1;  // �̹� ������ ����
        }
    }
    return 0;  // �������� ���� ����
}



float calcAverageGrade(int player) {
     int i; 
     float totalGrade = 0.0;
     int numGrades = smmdb_len(LISTNO_OFFSET_GRADE + player);

     for (i=0;i<numGrades;i++)
      {
        void *gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
        int gradeIndex = smmObj_getNodeGrade(gradePtr);


        totalGrade += smmGradeValue[gradeIndex];
       }

    return (numGrades > 0) ? (totalGrade / numGrades) : 0.0;
}



int isGraduated(void) //�÷��̾ ���� ������ �����ߴ����� Ȯ���ϴ� �Լ�
{

    int i;
    for (i = 0;i<player_nr;i++) 
    {    // ���� �÷��̾��� ���� ������ ���� ���� �̻����� Ȯ��
        if (cur_player[i].accumCredit >= GRADUATE_CREDIT) 
        {
            return 1; // ���� ���� ����
        }
    }
    return 0; //���� ��� �÷��̾ �������� ����
}


void printGrades(int player)
{    
    int i; 
    int gradeCount = smmdb_len(LISTNO_OFFSET_GRADE + player);

    if (gradeCount == 0)
    {
        printf("������ ���ǰ� �����ϴ�.\n");
        return;  // ���ǰ� ������ �� �̻� �������� �ʰ� �Լ��� �����մϴ�.
    }

    for (i = 0; i < gradeCount; i++)
    {
        void *gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
        printf("%s : %f\n",smmObj_getNodeName(gradePtr),smmGradeValue[smmObj_getNodeGrade(gradePtr)]);
    }
     printf(" average : %f\n",calcAverageGrade(player) );
}
 /*
smmObjGrade_e takeLecture(int player, char *lectureName, int credit) 
{
    smmObjLecture* Lecture = (smmObjLecture*)malloc(sizeof(smmObjLecture));
    strncpy(Lecture->name, lectureName, MAX_CHARNAME);
    Lecture->credit = credit;
    
    
    void *gradePtr = smmObj_genObject(lectureName, smmObjType_grade, 0, credit, 0, randomGrade);
    smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
}
*/




void printPlayerStatus(void)//�÷��̾��� �̸�, ��ġ, �� ����, �������� ���
{
     int i;
     
     for (i=0;i<player_nr;i++)
     {
         void *boardPtr = smmdb_getData(LISTNO_NODE, (cur_player[i].position) % board_nr);
         
         printf("==============================================\n%s at %i.%s, credit %i, energy %i\n==============================================\n", 
                      cur_player[i].name,
                      cur_player[i].position,
                      smmObj_getNodeName(boardPtr),
                      cur_player[i].accumCredit,
                      cur_player[i].energy
                      );
     }
}



void generatePlayers(int n, int initEnergy)//generate a new player
{
     //n time loop
     int i;
     for(i=0;i<n;i++) {
           
           // input name
         printf("Input player %i's name:", i+1); //��E���� ��?���� 
         scanf("%s",cur_player[i].name);
         fflush(stdin);
    
    //set position
    cur_player[i].position= 0;
    
    //set energy
    cur_player[i].energy = initEnergy;
    cur_player[i].accumCredit = 0;
    cur_player[i].flag_graduate = 0;
    cur_player[i].exp == 0;
    }
}


int rolldie(int player) //�ֻ��� ���� ����ϰ�, 'g'�� ������ �÷��̾��� ������ ���
{
    char c;
    printf("\n\n This is %s's turn! ::: Press any key to roll a die (press g to see grade): \n",cur_player[player].name);
    c = getchar();
    fflush(stdin);
    

    if (c == 'g' || c == 'G')
        printGrades(player);

    
    return (rand()%MAX_DIE + 1);
}


//action code when a player stays at a node
void actionNode(int player)
{   
    //���� ��ġ�� ����� Ÿ�԰� ����  
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    int type = smmObj_getNodeType(boardPtr);
    char *name = smmObj_getNodeName(boardPtr);
    void* gradePtr; 
    
    switch(type)
{
    case SMMNODE_TYPE_LECTURE: // ���ǳ�� 
    {
        void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
        // ���� ��ġ�� ���� ���� ��������
        int currentNodeType = smmObj_getNodeType(boardPtr);
        char* currentLectureName = smmObj_getNodeName(boardPtr);
        
        // ������ ������ �������� Ȯ��
        int alreadyTaken = isLectureTaken(player, currentLectureName);
        
        if (smmObj_getNodeCredit(boardPtr) > 0 && cur_player[player].energy >= smmObj_getNodeEnergy(boardPtr))
        {
            //���� ���� �Է�                                
            char choice;
            int grade = 0;
            int validInput = 0;

            while (!validInput)
            {
                if (alreadyTaken) //�̹� ������ ���  
               {
                 printf("You've already taken the lecture \"%s\".\n", currentLectureName);
                 validInput = 1; //  �ݺ��� ����
                }
                printf("Do you want to take the lecture? (y/n): ");
                scanf(" %c", &choice);

                if (choice == 'y' || choice == 'Y')
                {
                  // �����ϴ� ���
                  printf("You've taken the lecture \"%s\".\n", smmObj_getNodeName(boardPtr));
                  // ���� �� ���� ó��  
                  cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
                  cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
                  // �÷��̾ ���� ����� �����ߴ��� Ȯ��
                   if (cur_player[player].accumCredit >= GRADUATE_CREDIT)
                    {
                      cur_player[player].flag_graduate = 1;
                      printf("%s has accumulated enough credits to graduate!\n", cur_player[player].name);
                     }

                  grade = rand() % SMMGRADE_MAX;
                  gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, (smmObjGrade_e)grade);
                  smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
                  
                  /*���� �̷¿� �߰��ϱ� 
                  lectureObj = smmObj_genObject(name, 0, 0, smmObj_getNodeCredit(boardPtr), 0, (smmObjGrade_e)grade);
                  smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
                  */
                  printf("Grade for \"%s\": %s (%f)\n", smmObj_getNodeName(boardPtr),smmGradeName[grade],smmGradeValue[grade]);

                  validInput = 1; // ��ȿ�� �Է��̹Ƿ� �ݺ��� ����
                 }
                 else if (choice == 'n' || choice == 'N')
                 {
                     // ����ϴ� ���
                    printf("You've dropped the lecture \"%s\".\n", smmObj_getNodeName(boardPtr));
                    validInput = 1; // ��ȿ�� �Է��̹Ƿ� �ݺ��� ����
                  }
            else
                 {
                 // �ٸ� �� �Է� �� ó��
                printf("Invalid choice. Please enter 'y' or 'n'.\n");
                 }
              }
          }
    else
        { // ������ ������ �޽��� ���  
        printf("Not enough energy to take the lecture.\n");
        }
    break;
   }
   
   case SMMNODE_TYPE_RESTAURANT: // �Ĵ���  
   {   
        int remainer = 0 ;
        void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
        printf("You have reached a restaurant!\n");
        // ���� �÷��̾��� �������� ���ϱ�
        remainer = cur_player[player].energy += smmObj_getNodeEnergy(boardPtr); 
        printf(" Let's eat in %s and charge %d energies. (remained energy : %i)\n",smmObj_getNodeName(boardPtr),smmObj_getNodeEnergy(boardPtr),remainer);
        
        break;   
   }

    case SMMNODE_TYPE_LABORATORY: // ����ǳ��  
{
    // ���� �������� Ȯ��
    if (cur_player[player].exp == 1)
    {
        int energyCost = 3; // ���� �õ����� �ҿ�Ǵ� ������
        int diceResult = rolldie(player);

        if (cur_player[player].energy >= energyCost) // ����� �������� �ִ��� Ȯ��
        {
            int escapeValue = rolldie(player);

            if (diceResult > escapeValue)
            {   // ���� ����
                printf("Experiment successful! Moving to home.\n");
                cur_player[player].position = SMMNODE_TYPE_HOME; // ���� ���� �� Ȩ ���� �̵�
                cur_player[player].exp = 0;  // ���� ���� ����
            }
            else
            {
                printf("Experiment failed! Staying in the laboratory.\n");
            }
            cur_player[player].energy -= energyCost;
        }
        else
        {
            printf("Not enough energy to attempt the experiment.\n");
        }
        
        break;
    }
    else
    {
        printf("This is not experiment time. You can go through this lab.\n");
    }
    break;
}

    case SMMNODE_TYPE_HOME:
   { //�������� ���� ������ ���� 
      
       printf(" returned to HOME! energy charged by %d\n", INITENERGY);
       cur_player[player].energy += INITENERGY;
       break;
   }
     
    case SMMNODE_TYPE_GOTOLAB:
{
        int escapeValue = abs(rolldie(player) - 2);
        printf("OMG! This is experiment time!!\n");
        printf("Escape value for the experiment: %d\n", escapeValue);
        cur_player[player].position = SMMNODE_TYPE_LABORATORY; // ����� ���� �̵�
        cur_player[player].exp = 1; // ���� ���·� ����  
    break;
}
       
    case SMMNODE_TYPE_FOODCHANCE:
   {
        printf("You've got a food chance! Press any key to draw a food card: ");
        int userInput;
        scanf("%d", &userInput);
        //����ī��̱�  
        FoodCard *foodCard = (FoodCard *)smmdb_getData(LISTNO_FOODCARD, rand() % food_nr);
        
        printf("You've drawn a food card: %s (get energy :%d)\n", smmObj_getFoodName(foodCard),smmObj_getFoodEnergy(foodCard));
        //�������߰�  
        cur_player[player].energy += smmObj_getFoodEnergy(foodCard);
        
       
        break;
   }
     
    case SMMNODE_TYPE_FESTIVAL:
   {
        printf("You've reached a festival! Press any key to draw a festival card: ");
        int userInput;
        scanf("%d", &userInput);
        //����ī��̱�  
        FestivalCard *festCard = (FestivalCard *)smmdb_getData(LISTNO_FESTCARD, rand() % festival_nr);
        printf("%s\n", smmObj_getFestName(festCard));
      
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
        // �ֻ����� �̵��� ����� �����͸� ����
        void *boardPtr = smmdb_getData(LISTNO_NODE, (cur_player[player].position + i) % board_nr);
        printf("  => Jump to %s\n", smmObj_getNodeName(boardPtr));
    }
       // �÷��̾��� ��ġ ������Ʈ
     cur_player[player].position += step ;
     
     // ������ ��忡 ������ ��� ���带 �� ���� �� ������ ó��
   if (cur_player[player].position >= board_nr)
    {
        void *firstBoardNode = smmdb_getData(LISTNO_NODE, 0);
        cur_player[player].energy += smmObj_getNodeEnergy(firstBoardNode);
        cur_player[player].position %= board_nr;
    }
     // �÷��̾��� ���� ��ġ�� �ش��ϴ� ��� ���� ���
     void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
     printf("%s go to node %i(name: %s)\n", cur_player[player].name, cur_player[player].position, 
     smmObj_getNodeName(boardPtr));
     
     // SMMNODE_TYPE_HOME ���� �� ���� ó��
    if (smmObj_getNodeType(boardPtr) == SMMNODE_TYPE_HOME)
    {
    // �÷��̾��� ���°� ���� �������� Ȯ���ϰ�, ���� ���¶�� ���� ����
        if (cur_player[player].flag_graduate == 1)
        {
            printf("%s has graduated! Game over.\n", cur_player[player].name);
            exit(0);
        }
        if (cur_player[player].position != step) 
        {                     
             cur_player[player].energy += INITENERGY;                           
        }
     #if 0   
     //����ð� ����� �� 
     if(smmObj_getNodeType(boardPtr) == SMMNODE_TYPE_GOTOLAB )
      {
        int escapeValue = rolldie(player);
          printf("OMG! This is experiment time!!\n");
          printf("Escape value for the experiment: %d\n", escapeValue);
       }
    #endif
    }
     
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
    
    FILE *logFile = fopen("logfile.txt", "a"); // "a"�� ���� ���Ͽ� �̾ ���⸦ �ǹ�

    if (logFile != NULL) {
        fprintf(logFile, "This is a log message.\n");

        // ... ���α׷� ���� ...

        fprintf(logFile, "Another log message.\n");

        fclose(logFile);
    } else {
        printf("Error opening the log file.\n");
    }
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
    printf("%i. name: %s, energy: %i\n", j, smmObj_getFoodName(foodCard), smmObj_getFoodEnergy(foodCard));
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
    printf("%i. %s\n ", k, smmObj_getFestName(festCard));
}
   

    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
           printf("\n\n\n*********************************************************************\n=====================================================================\n                          Sookmyung Marble\n\n                          Let's Graduate!!!\n=====================================================================\n*********************************************************************\n\n\n");
           printf("\nHow many people are playing? :\n");
           scanf("%d", &player_nr);
           fflush(stdin);
    } while (player_nr < 0 || player_nr > MAX_PLAYER);
     
     cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
     generatePlayers(player_nr, initEnergy);
    
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (!isGraduated()) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)
        //rolldie �Լ� ��� 
        
        if(cur_player[turn].exp == 1)
        {
           die_result = 0;
        }
        else
        {
            die_result = rolldie(turn);
        }
        
        //4-3. go forward
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn+1)%player_nr;
        
        if (cur_player[turn].flag_graduate == 1)
        {
           printf("%s has graduated!\n", cur_player[turn].name);
        
           /* Iterate through the grades of the player and print information
           void *gradePtr = smmdb_getHead(LISTNO_OFFSET_GRADE + i);
           while (gradePtr != NULL)
           {
            printf("   - Lecture: %s, Credit: %d, Grade: %s\n",
                   smmObj_getNodeName(gradePtr),
                   smmObj_getNodeCredit(gradePtr),
                   smmGradeName[smmObj_getNodeGrade(gradePtr)]);

            gradePtr = smmdb_getNext(gradePtr);
            }*/
         }
     
     }
    //5. Game End
    printf("Game has ended!\n");
    

   free(cur_player);
   
   return 0;
}

