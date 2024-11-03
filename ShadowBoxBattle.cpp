#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <limits>

using namespace std;

#ifdef _WIN32
    #define CLEAR_SCREEN() system("cls")  //Windows
#else
    #define CLEAR_SCREEN() system("clear")  // Linux/MacOS
#endif

const int MP = 3;//number of misses for the powerpunch to be activated

bool search(string dir[],int game_round);
int attacker(int *missp);
void defender(int *missai);
void print(string dir[],int game_round,int turn);
void manual();
int attacker_powerpunch();
void defender_powerpunch();
bool valid_direction(string dir[],int game_round);


int main(){

  int num,ub=3,lb=0,roundwa,roundwd,ans,wincount=0,missplayer,missai;

  srand(time(0));

  cout <<"Here is a link to the soundtrack of the game.Play it while playing the game\n";
  cout << "for a better experience: https://youtube.com/playlist?list=PL9AEZBovoXLvP0-L11FAquLfA-MRowfP_&si=VHhGqfgcH-AppIoe \n";



  do {
       cout <<"\n1-Start\n2-Manual\n";
       while (!(cin >> ans)){
         cin.clear();
         cin.ignore(123,'\n');
         cout<<"Invalid Input\n";
         cout <<"\n1-Start\n2-Manual\n";
       }
       if (ans==2){
           manual();
           cout<< "\n";
       }else if(ans!=1 && ans!=2)cout << "Invalid Input. Pick a right option\n";
  }while (ans!=1);
      while(ans==1){
        roundwa=0;
        roundwd=0;
        int i=0;
        CLEAR_SCREEN();
        //missplayer and missai variables are used to identify if the player
        //or the ai should have Powerpunch available(3 misses activate Powerpunch)
        missplayer=0;
        missai=0;
        //using rand to get a number between 1 and 2
        //in order to pick who is going to start the round as an attacker
        if (((rand() % (2 - 1 + 1)) + 1)==1){
            cout << "You are starting this round as an attacker\n";
            /*The player starts the round as an attacker and
            before every round, it is being checked if either the player
            or the AI has Powerpunch available.The loop keeps going until the player
            knocks out the ai or the ai knocks the player out and the entire program stops
            because of exit(0)*/
            do{
                if(missplayer==MP){
                    //after every Powerpunch round the miss count is updated to 0
                    roundwa = attacker_powerpunch();
                    missplayer=0;
                }else  roundwa = attacker(&missplayer);
                if (roundwa==2){
                  if(missai==MP){
                    defender_powerpunch();
                    missai=0;
                  }else defender(&missai);
                }
            }while(roundwa!=1);

        }else{
             cout << "You are starting this round as a defender\n";
             //same exact logic as before but this time
             //the ai is starting as an attacker
            do{
               if(missai==MP){
                  defender_powerpunch();
                  missai=0;
               }else defender(&missai);
                if(missplayer==MP){
                  roundwa = attacker_powerpunch();
                  missplayer=0;
               }else roundwa = attacker(&missplayer);
            }while(roundwa!=1);
             }
             //simple win streak consept
             ++wincount;
             cout << "Wins: "<< wincount << "\n";
             cout << "You Won!";
             do{
               cout << "Want to keep going(1-YES/2-NO)? ";
               while(!(cin >> ans)){
                   cin.clear();
                   cin.ignore(123,'\n');
                   cout<<"Invalid Input\n";
                   cout << "Want to keep going(1-YES/2-NO)? ";
               }
               if(ans!=1 && ans!=2)cout <<"Invalid Input. Pick a right option(1-YES/2-NO)\n";
             }while(ans!=1 && ans!=2);

      }


    return 0;
}











/*This function is used when the player is attacking the ai and to count and update the consecutive first attack misses of the player.
The function returns 1 or 2 depending on who "won" the round(1 if the player won,2 if the AI slipped a punch*/
int attacker(int *missp){
    int num,ub=3,lb=0,game_round=1;
    string punch[3],aidefence[3];
    string choices[4]={"1","2","3","4"};
    bool found,valid;
        cout <<"Pick a direction to punch\nPunches: ";
        cin >> punch[game_round-1];
        while(!(valid_direction(punch,game_round))){
            cout << "Invalud input Pick a valid option(1-UP,2-DOWN,3-LEFT,4-RIGHT)\n";
            cout <<"\nPick a direction to punch\nPunches: ";
            cin >> punch[game_round-1];
        }
        //we are getting a random number between 0 and 3 and based on the answer
        //the ai basically chooses a direction to slip through the "choices" array
         num =((rand() % (ub - lb + 1)) + lb);
         aidefence[game_round-1]=choices[num];
         cout << "Ai's slip: "<< aidefence[game_round-1]<< "\n";
         if (punch[game_round-1]==aidefence[game_round-1]){                                                         //if the player's punch matches the ai's slip then it counts as a hit for the player
             cout <<"Hit! Two to go\n";
             ++game_round;
             *missp=0;                                                                                    //if the player hits the ai, the missing streak is interrupted so the powerpunch won't be
         }else{                                                                                           //available to the player for at least the next three rounds
           ++*missp;        // if the player misses, the count of consecutive misses is updated
           if(*missp==MP){                                                                        //if the number of consecutive misses is 3 then a message is printed to let the player know that he has Powerpunch available next round
             cout << "\nThird miss in a row!You have Powerpunch available\n";                     //else a regular miss message is printed
           }else cout << "\nMiss! Now defend!\n";
           return 2;
         }
            //This loop starts if the attacker hits the AI and keeps going until either the player knocks out the AI or the AI slips a punch
            do{
               do{
                   print(punch,game_round,1);
                   cin >> punch[game_round-1];
                   valid = valid_direction(punch,game_round);
                   found = search(punch,game_round);
                   if(!valid) {
                        cout<<"\nInvalid input. Pick a valid option(1-UP,2-DOWN,3-LEFT,4-RIGHT)\n";
                   }else if (found) cout <<"Cant punch same direction as previous rounds\n";
               }while (found || !valid);
               //This loop keeps going until the random number/direction is not used in a previous round
               do{
                   num =((rand() % (ub - lb + 1)) + lb);
                   aidefence[game_round-1]=choices[num];
                   found=search(aidefence,game_round);
                }while (found);
                cout << "Ai's slip: "<< aidefence[game_round-1]<< "\n";
                if (punch[game_round-1]==aidefence[game_round-1]){
                if (game_round==2) cout << "Hit! One to go\n";
                if (game_round==3) cout << "Knockout!\n";
                ++game_round;
                }else{
                     cout << "\nMiss! Now defend!\n";
                     return 2;
                }
           }while(game_round<=3);

    //if game_round is 4 then it means that the player knocked out the AI so 1 is returned
    if (game_round>3)return 1;
}


/*This function is used when the player has to defend against the AI and to count and update the consecutive
first attack misses of the AI through "missai". The function returns 1 if the player slipped a punch.
If the AI knocks the player out then the entire program is shut down.Same logic is applied in the defender
function as it is applied in the attacker one but in reverse.*/
void defender(int *missai){

    int num,ub=3,lb=0,game_round=1;
    string aipunch[3],defence[3];
    string choices[4]={"1","2","3","4"};
    bool found,valid;

        cout << "Pick a direction to slip\nSlips: ";
        cin >> defence[game_round-1];
        while(!(valid_direction(defence,game_round))){
            cout << "Invalud input Pick a valid option(1-UP,2-DOWN,3-LEFT,4-RIGHT)\n";
            cout <<"\nPick a direction to slip\nSlips: ";
            cin >> defence[game_round-1];
        }
         num=((rand() % (ub - lb + 1)) + lb);
         aipunch[game_round-1]=choices[num];
         cout << "Ai's punch "<<aipunch[game_round-1]<< "\n";
          if (aipunch[game_round-1]==defence[game_round-1]){
            cout<<"You got hit!\n";
            ++game_round;
            *missai=0;
          }else{
             ++*missai;
             if(*missai==MP){
            cout << "\nThird AI's miss in a row!AI has Powerpunch available\n";
           }else cout << "\nWell defended! Now attack!\n";
             return ;
          }
                do{
                      do{
                           print(defence,game_round,2);
                           cin >> defence[game_round-1];
                           valid = valid_direction(defence,game_round);
                           found = search(defence,game_round);
                           if (!valid){
                               cout << "Invalid input.Pick a valid option(1-UP,2-DOWN,3-LEFT,4-RIGHT)\n";
                           }else if(found) cout << "Can't slip same direction as previous rounds\n";
                      }while (found || !valid);
                      do{
                           num =((rand() % (ub - lb + 1)) + lb);
                           aipunch[game_round-1]=choices[num];
                           found=search(aipunch,game_round);
                      }while (found);
                      cout << "Ai's punch "<<aipunch[game_round-1]<< "\n";
                      if (aipunch[game_round-1]==defence[game_round-1]){
                          if(game_round==2)cout<<"You got hit!\n";
                          if(game_round==3) exit(0);
                          ++game_round;

                      }else{
                          cout << "\nWell defended! Now attack!\n";
                          return ;
                      }

              }while(game_round<=3);


}



/*This is the function that is used for the Powerpunch round when the player is attacking.It is basically the first part of the attacker
function but one punch is enough to knockout the AI */
int attacker_powerpunch(){
    int num,ub=3,lb=0,game_round=1;
    string punch[3],aidefence[3];
    string choices[4]={"1","2","3","4"};
    bool valid;
    do{
           cout <<"\nPOWERPUNCH\n";
           cout <<"Pick a direction to punch\nPowerpunch: ";
           cin >> punch[game_round-1];
           valid = valid_direction(punch,game_round);
           if(!valid) cout<<"\nInvalid input. Pick a valid option(1-UP,2-DOWN,3-LEFT,4-RIGHT)\n";
    }while(!valid);
    num=((rand() % (ub - lb + 1)) + lb);
    aidefence[game_round-1]=choices[num];
    cout << "Ai's slip: "<< aidefence[game_round-1]<< "\n";
    if (punch[game_round-1]==aidefence[game_round-1]){
      cout<<"Knockout by Powerpunch!\n";
      return 1;
    }else return 2;

}

//This is the analogous Powerpunch function of the previous one,used when the player has to defend against a Powerpunch
void defender_powerpunch(){
    int num,ub=3,lb=0,game_round=1;
    string aipunch[3],defence[3];
    string choices[]={"1","2","3","4"};
    bool valid;

     do{
           cout << "\nAI WILL POWERPUNCH \n";
           cout << "Pick a direction to slip\nSlip: ";
           cin >> defence[game_round-1];
           valid = valid_direction(defence,game_round);
           if(!valid)cout<< "Invalid input. Pick a valid option(1-UP,2-DOWN,3-LEFT,4-RIGHT)\n";
    }while(!valid);
    num=((rand() % (ub - lb + 1)) + lb);
    aipunch[game_round-1]=choices[num];
    cout << "Ai's punch "<<aipunch[game_round-1]<< "\n";
    if (aipunch[game_round-1]==defence[game_round-1]){
            exit(0);
    }else return ;

}
/*this function is made to check if a player punched or slipped in the same direction with a previous round.
It's also used to force the ai to not punch or slip to the same direction with a previous round*/
bool search(string dir[],int game_round){
int i;
for (i=0;i<game_round-1;++i){
    if (dir[i]==dir[game_round-1]){
        return true;
    }
}
return false;

}


//I don't know if it's the right choice to make this function but at least it feels right to me
void print(string dir[],int game_round,int turn){
    int i;
    if(turn==1){
       cout<< "Punches: ";
       for (i=0;i<game_round-1;++i){
           cout<< dir[i]<<",";
       }
    }else{
        cout<< "Slips: ";
        for (i=0;i<game_round-1;++i){
           cout<< dir[i]<<",";
        }

    }

}



//this function is made to check the validity of the directions of punches and slips
bool valid_direction(string dir[],int game_round){
    if(dir[game_round-1]!="1" && dir[game_round-1]!="2" && dir[game_round-1]!="3" && dir[game_round-1]!="4"){
        return false;
    }else return true;
}




void manual(){

cout<<"\nShadowboxing Battle is a turn-based game where players alternate between\n";
cout<<"being the attacker and defender. As the attacker, you choose a direction to punch\n";
cout<<"(1-Up, 2-Down, 3-Left, or 4-Right), while the AI chooses a direction to slip and\n";
cout<<"avoid your punch. If your punch matches the AI's slip, you land a hit.\n";
cout<<"As the defender, you choose a direction to slip (1-Up, 2-Down, 3-Left, or 4-Right)\n";
cout<<"while the AI punches. If the AI's punch matches your slip,\n";
cout<<"you get hit. The first player to successfully hit their\n";
cout<<"opponent three times in a row wins the round. But there's a catch.\n";
cout<<"When either one of the player or the AI misses three punches in a row,\n";
cout<<"Powerpunch round is activated for the one who missed them.\n";
cout<<"In the Powerpunch round, one punch is enough to knock out the opponent\n";
cout<<"After each round you win, you can choose to play again or exit.\n";
cout<<"If you get knocked out in a round,think of what would have happened in real life.\n";
cout<<"Stay strategic, mixing up your moves and predicting your opponent's next action to win!\n";
cout<<"\nAlso you can't punch or slip to the same direction as before in a single round:)\n";

}


//The only ai generated parts here are the manual(almost) and the CLEAR_SCREEN function
//Everything else is my creation :)


