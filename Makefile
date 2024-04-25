make: pre_load PlayerStateMachine.o EnemyStateMachine.o game clean

pre_load:
	rm -f ./game

PlayerStateMachine.o: PlayerStateMachine.cpp
	g++ -c PlayerStateMachine.cpp -o PlayerStateMachine.o -I raylib/ --std=c++11

EnemyStateMachine.o: EnemyStateMachine.cpp
	g++ -c EnemyStateMachine.cpp -o EnemyStateMachine.o -I raylib/ --std=c++11

game: 
	g++ Main.cpp PlayerStateMachine.o EnemyStateMachine.o -o game -I raylib/ -L raylib/ -lraylib --std=c++11

clean:
	rm *.o