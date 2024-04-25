make: pre_load PlayerStateMachine.o EnemyStateMachine.o game clean

pre_load:
	rm -f ./game

PlayerStateMachine.o: PlayerStateMachine.cpp
	g++ -c PlayerStateMachine.cpp -o PlayerStateMachine.o -I/opt/homebrew/Cellar/raylib/5.0/include --std=c++11

EnemyStateMachine.o: EnemyStateMachine.cpp
	g++ -c EnemyStateMachine.cpp -o EnemyStateMachine.o -I/opt/homebrew/Cellar/raylib/5.0/include --std=c++11

game: 
	g++ Main.cpp PlayerStateMachine.o EnemyStateMachine.o -o game -I/opt/homebrew/Cellar/raylib/5.0/include -L/opt/homebrew/Cellar/raylib/5.0/lib -lraylib --std=c++11

clean:
	rm *.o