make: pre_load PlayerStateMachine.o EnemyStateMachine.o GameUI.o Cloud.o game clean

pre_load:
	rm -f ./game

PlayerStateMachine.o: PlayerStateMachine.cpp
	g++ -c PlayerStateMachine.cpp -o PlayerStateMachine.o -I/opt/homebrew/Cellar/raylib/5.0/include --std=c++11

EnemyStateMachine.o: EnemyStateMachine.cpp
	g++ -c EnemyStateMachine.cpp -o EnemyStateMachine.o -I/opt/homebrew/Cellar/raylib/5.0/include --std=c++11
	
GameUI.o: GameUI.cpp
	g++ -c GameUI.cpp -o GameUI.o -I/opt/homebrew/Cellar/raylib/5.0/include --std=c++11

Cloud.o: Cloud.cpp
	g++ -c Cloud.cpp -o Cloud.o -I/opt/homebrew/Cellar/raylib/5.0/include --std=c++11

game: 
	g++ Main.cpp Cloud.o PlayerStateMachine.o GameUI.o EnemyStateMachine.o -o game -I/opt/homebrew/Cellar/raylib/5.0/include -L/opt/homebrew/Cellar/raylib/5.0/lib -lraylib --std=c++11

clean:
	rm *.o