#ifndef CREW_HPP
#define CREW_HPP

#include <string>
#include <vector>
#include<stack>



class Crew{
  public:
  int ID;
  std::string Name;
  bool isImposter;
  int tasksCompleted;
  int kills;
  std::stack <std::string> actionHistory;
  Crew(int id, const std::string& nameplayer , bool role=false);
   
  void addAction(const std::string& action);
  std::string lastAction()const;


};
class Node{
    public:
    Crew* player;
    Node*next;

    Node(Crew*p);
};

//crew manager
class crewManager{
public:
   Node*head;
   int playerCount;
   crewManager(); //constructor
   ~crewManager(); //Destructor

   void addPlayer(int ID,const std:: string&Name);
   void removePlayer(int ID); //when have more voting
   void assignRoles(int imposterCount ); //how many immpostors that we want to be
   std::vector <Crew *>getplayerVector(); // contains all players in game
   void sortLeaderBoard(); //task completed + sort by kill
   void displayPlayers();
};

#endif