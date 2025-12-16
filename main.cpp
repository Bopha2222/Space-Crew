#include "ginclude/gui_manager.hpp"
#include <iostream>

using namespace std;

int main()
{
    cout << "==================================" << endl;
    cout << "   AMONG US - GUI VERSION" << endl;
    cout << "==================================" << endl;
    cout << "Initializing..." << endl;
    
    GUIManager guiManager;
    
    if (!guiManager.initialize())
    {
        cerr << "Failed to initialize GUI!" << endl;
        cerr << "Make sure fonts/Arial.ttf exists!" << endl;
        return -1;
    }
    
    cout << "Starting game loop..." << endl;
    guiManager.run();
    
    cout << "Game closed. Goodbye!" << endl;
    return 0;
}