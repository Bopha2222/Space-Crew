# 🎮 Space Crew – How to Play

Space Crew is a 2D social deduction game inspired by *Among Us*, developed in **C++ with SFML**. Players must complete tasks while identifying hidden impostors.

## 🖥️ System Requirements

* Windows 10 / 11
* Keyboard & mouse
* Game executable and required CSV data files in the same directory

---

## 🚀 Starting the Game

1. Launch the game executable.
2. The **Welcome Screen** will appear.
3. Click **Start** to proceed to the login screen.

---

## 🔐 Login & Account System

### Login

* Enter your **username** and **password**.
* If credentials are correct, you will enter the game menu.
* After **3 failed login attempts**, the account will be temporarily locked.

### Create Account

* Click **Sign Up**.
* Password rules:

  * At least 8 characters
  * One uppercase letter
  * One lowercase letter
  * One number
  * One special character
* Duplicate usernames are not allowed.

### Roles

* **Admin**: Access admin panel, analytics, user and task management.
* **User**: Can play the game and view personal statistics.

---

## 🎯 Game Setup

1. Choose a **difficulty level**:

   * Easy
   * Medium
   * Hard
   * Custom
2. The system initializes **10 players**:

   * 1 human player
   * 9 bot players
3. One player is randomly selected as the **Impostor**.
4. Tasks are randomly assigned to crewmates.

---

## 🕹️ Gameplay Controls

### Movement

* Use keyboard keys to move your character around the spaceship.
* Movement is restricted by walls and room boundaries.

### Player Actions

* **Move**: Navigate through connected rooms
* **Perform Task**: Complete assigned missions using task passwords
* **Call Emergency Meeting**: Discuss suspicious behavior (cooldown applies)
* **Vote**: Vote to eject a suspected impostor

### Impostor Actions

* **Eliminate players** (with cooldown timer)
* **Avoid detection**
* **Blend in with crew behavior**

---

## 🗺️ Map & Rooms

* The spaceship contains **10 interconnected rooms**.
* Each task is assigned to a specific room.
* Players must navigate to the correct room to complete tasks.

---

## 📋 Task System

* Tasks are stored using a **Singly Linked List**.
* Each task requires a **password** to complete.
* Completing tasks contributes toward a crewmate victory.

---

## 🚨 Emergency Meetings & Voting

1. A meeting can be called if suspicious behavior is noticed.
2. Each player casts a vote.
3. Votes are processed in order using a **Queue**.
4. The player with the most votes is ejected.

---

## 🏆 Win Conditions

### Crewmates Win If:

* All tasks are completed, OR
* The impostor is successfully ejected

### Impostor Wins If:

* All crewmates are eliminated

---

## 📊 Leaderboard & Statistics

* Leaderboard shows **Top 10 players** by win points.
* Player profiles include:

  * Games played
  * Wins and losses
  * Win rate
* Scores are sorted using the **Bubble Sort algorithm**.

---

## 💾 Saving & Loading

* All data is saved automatically using **CSV files**:

  * User accounts
  * Game results
  * Tasks and passwords
  * Console logs
* Progress is restored when the game restarts.

---

## 🛠️ Admin Panel (Admin Only)

* View console activity logs
* Manage users and roles
* View and manage tasks
* Search system logs

---

## 🔚 Ending the Game

* After a win or loss:

  * Results are saved
  * Statistics are updated
  * You may choose to **Play Again** or return to the main menu

---

## ✅ Tips for New Players

* Complete tasks quickly as a crewmate
* Observe movement patterns
* Use emergency meetings wisely
* Impostors should avoid drawing attention

---

🎉 **Enjoy playing Space Crew!**
