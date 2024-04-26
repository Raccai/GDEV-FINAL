How to Run:
1. Download Laurel_Layug_Marcelo_FinalProject.zip and extract it.
2. Once extracted, add a Raylib folder to the directory.
3. Then, go to Raylib's w64devkit and go to the directory where the Laurel_Layug_Marcelo_FinalProject folder is located.
4. Type "g++ Main.cpp PlayerStateMachine.cpp EnemyStateMachine.cpp Cloud.cpp GameUI.cpp -o game -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm"
5. Run game.exe.

Note:
   - game.exe is running based on Level 1 (level1.txt). If you want to go to different levels (Level 2 and Level 3), go to Main.cpp and change line 136
     to std::string map_config = "level2.txt"; or std::string map_config = "level3.txt";

