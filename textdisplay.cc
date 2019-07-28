#include "textdisplay.h"

TextDisplay::TextDisplay(){}

int TextDisplay::random(int x, int y){
	srand(time(nullptr));
	return rand()%y+x;
}

Position TextDisplay::randPos(){
	int x = random(0, BOARDHEIGHT);
	int y = random(0, BOARDWIDTH);

	bool check = true;

	while(check){
		check = false;
		
		for(int i = -1; i <= 1; ++i){
			for(int j = -1; j <= 1; ++j){
				if(theDisplay[x+i][y+j] != '.'){
					check = true;
					break;
				}
			}
		}

		if(check == false) break;

		x = random(0, BOARDHEIGHT);
		y = random(0, BOARDWIDTH);
	}

	return Position{x, y};
}


void TextDisplay::generateStairs(){
	Position p = randPos();
	
	//.......
	
	theDisplay[p.x][p.y] = '\\';
	++stairsCount;
}

void TextDisplay::generateDragon(int r, int c){
	int x = random(-1, 3);
	int y = random(-1, 3);

	while(x == 0 && y == 0){
		y = random(-1, 3);
	}

	theDisplay[r+x][c+y] = 'D';
	++enemyCount;
}

void TextDisplay::generate(int floor){
	//generating in order....
	Position p;

	//player
	if(playerCount == 0){
		++playerCount;
		p = randPos();
		theDisplay[p.x][p.y] = '@';
	}
	//stairs
	if(stairsCount == 0){
		generateStairs();
	}
	
	//potions
	while(potionCount != TOTALPOTION){
		p = randPos();
		theDisplay[p.x][p.y] = random(0, 6);
		++potionCount;
	}

	//treasure
	//barrier suit
	if(floorNumber == floor && treasureCount == 0){
		p = randPos();
		theDisplay[p.x][p.y] = 'B';
		generateDragon(p.x, p.y);
		++treasureCount;
	}

	while(treasureCount != TOTALTREASURE){
		p = randPos();
		int n = random(0, 8);

		if(n >= 0 && n <= 4){ //0,1,2,3,4 -> normal gold
			theDisplay[p.x][p.y] = 6;
		}
		else if(n == 5 || n == 6){
			theDisplay[p.x][p.y] = 7;
		}
		else{ //dragon hoard
			theDisplay[p.x][p.y] = 9;
			generateDragon(p.x, p.y);
		}
		
		++treasureCount;
	}

	//enemies
	while(enemyCount != TOTALENEMY){
		p = randPos();
		int n = random(0, 18);
		char e = 'E';

		if(n >= 0 && n <= 2){ //vampire
			e = 'V';
		}
		else if(n >= 3 && n <= 6){ //werewolf
			e = 'W';
		}
		else if(n >= 7 && n <= 11){ //goblin
			e = 'N';
		}
		else if(n == 12 || n == 13){ //phoenix
			e = 'X';
		}
		else if(n == 14 || n == 15){ //troll
			e = 'T';
		}
		else{ //merchant
			e = 'M';
		}
		
		theDisplay[p.x][p.y] = e;
		++enemyCount;
	}	
	
}



istream &operator>> (istream&in, TextDisplay &td){
	char c;
	for(int i = 0; i < td.BOARDHEIGHT; ++i){
		for(int j = 0; j < td.BOARDWIDTH; ++j){
			in >> c;
			td.theDisplay[i][j] = c;

			//checks what the input is
			if(c == '.' || c == '|' || c == '-' || c == '#' || c == '+') continue;
			else if(c == '\\') ++td.stairsCount;
			else if(c == '@') ++td.playerCount;
			else if(c-'0' >= 0 && c-'0' <= 5) ++td.potionCount;
			else if(c-'0' >= 6 && c-'0' <= 9){
			       if(c-'0' == 8){//if merchant hoard
				       td.theDisplay[i][j] = 'M';
				       ++td.enemyCount;
			       }
			       else 
				       ++td.treasureCount;
			}
			else if(c == 'B') ++td.treasureCount;
			else ++td.enemyCount;	
		}
	}
	return in;
}


ostream &operator<< (ostream &out, const TextDisplay &td){
	for(int i = 0; i < td.BOARDHEIGHT; ++i){
		for(int j = 0; j < td.BOARDWIDTH; ++j){
			out << td.theDisplay[i][j];
		}
	}
	return out;
}
