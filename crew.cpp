#include"Crew.hpp"
#include<iostream>
#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<random>
using namespace std;



Crew::Crew(int id, const std::string &nameplayer, bool role)
  :
    ID(id),
    Name(nameplayer),
    isImposter(role),
    tasksCompleted(0),
    kills(0){}




void Crew::addAction(const std::string &action)
{
    actionHistory.push(action);
}
    std::string Crew:: lastAction()const{
        if(!actionHistory.empty()){
            return actionHistory.top();
        }else{
            return "Nothing Actions";
        }
    }

Node:: Node(Crew*p)
    :
        player(p),
        next(nullptr){}
crewManager:: crewManager()
:
  head(nullptr) , playerCount(0){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
  }
  crewManager::~crewManager(){
    Node*t=head;
    while(t!=nullptr){
        Node*nextNode=t->next;
        delete t->player;
        delete t;
        t=nextNode;
    }
  }
  //Add Player
  void crewManager::addPlayer(int ID ,const std::string& Name){
    Crew*newCrew=new Crew(ID,Name);
    Node*newNode=new Node(newCrew);

    newNode->next=head;
    head=newNode;
    playerCount++;

  }
  void crewManager:: removePlayer(int ID){ // using DLL
    Node*t=head;
    Node*prev=nullptr;

    while(t!=nullptr){
        if(t->player->ID==ID){
           if(prev==nullptr){
            head=t->next;
           }else{
            prev->next=t->next;
           
           delete t->player;
           delete t;
           playerCount--;
           return;
           }
        }
        prev=t;
        t=t->next;
    }
  }

vector<Crew*>crewManager::getplayerVector(){ // sorting roles and leaderboard(converted ll to vector)
    vector<Crew*> vec;

    Node*t = head;
    while(t!=nullptr){
        vec.push_back(t->player);
        t=t->next;
    }
    return vec;
}
    void crewManager:: assignRoles(int imposterCount){
        if(imposterCount>=playerCount){
            cout<<"Too many Imposters.."<<endl;
            return;
        }
        vector<Crew*> players=getplayerVector();

        std::random_device rdd;
        std::mt19937 g(rdd());
        std::shuffle(players.begin(), players.end(), g);
          for (size_t i = 0; i < players.size(); ++i) {
        players[i]->isImposter = (static_cast<int>(i) < imposterCount);
         }  
        }
    
    
    void crewManager:: sortLeaderBoard(){
        vector<Crew*> players=getplayerVector();
      sort(players.begin() , players.end() , [](Crew*a , Crew*b){
        int scoreA=a->kills+a->tasksCompleted;
        int scoreB=b->kills+b->tasksCompleted;
        return scoreA>scoreB;
      });
      cout<<"\n===LeaderBoard==="<<endl;
      for(Crew* p: players){
        cout<<p->Name<<" | Kills : "<<p->kills<<" | Tasks : "<<p->tasksCompleted<<endl;
      }


    }
    void crewManager:: displayPlayers(){
        Node*t=head;
        cout<<"List of Crew"<<endl;
        while(t!=nullptr){
            cout<<"ID : "<<t->player->ID<<"| Name : "<<t->player->Name<<"| Role : "<<(t->player->isImposter? "Imposter" : "Crewmate")<<endl;
            t=t->next;
        }
    }


int main(){
  crewManager cm;

    // Add players
    cm.addPlayer(1, "Red");
    cm.addPlayer(2, "Blue");
    cm.addPlayer(3, "Green");
    cm.addPlayer(4, "Yellow");
    cm.addPlayer(5, "Pink");

    cout << "\n--- Players Added ---\n";
    cm.displayPlayers();

    // Assign roles (e.g., 2 imposters)
    cout << "\n--- Assigning Roles ---\n";
    cm.assignRoles(2);
    cm.displayPlayers();

    // Simulate some actions
    // Adding kills and tasks manually for testing
    vector<Crew*> players = cm.getplayerVector();
    players[0]->kills = 2;
    players[1]->tasksCompleted = 3;
    players[2]->kills = 1;
    players[2]->tasksCompleted = 4;

    // Show leaderboard
    cm.sortLeaderBoard();

    // Remove player with ID 3
    cout << "\n--- Removing Player ID 3 ---\n";
    cm.removePlayer(3);
    cm.displayPlayers();

    return 0;
}

