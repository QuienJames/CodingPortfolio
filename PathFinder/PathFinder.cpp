//JAMES ANTHONY DY QUIEN 
//PATHFINDER 

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

#define MAXINT 2147483647
#define MAXWIDTH 100
#define MAXHEIGHT 100
#define COLONY 10000000//2 0's

void bestRoute(int checkPointID,int checkPointCheck[18],double pheromone[][18],int bestRouteList[18],int bestRoutePlace,const int checkPointsNum);
void antAlgorithm(double pheromoneStart[18], double pheromoneGoal[18], double pheromone[][18],const int checkPointsNum,int checkPointsShuffle[18],int lengths[][18],int startLengths[18],int goalLengths[18]);
double antAlgorithmHelper(double pheromone[][18],const int checkPointsNum,int checkPointsShuffle[18],int lengths[][18],int startLengths[18],int goalLengths[18]);
void shuffle(const int checkPointsNum,int checkPoints[18]);
void permutationStart(int startLengths[18],int checkPointsNum,int goalLengths[18],int shortest[1],int lengths[][18],int checkPointCheck[18]);
void permutation(int currentLength,int checkPointsNum,int goalLengths[18],int shortest[1],int lengths[][18],int checkPointCheck[18], int checkPointID);
int spreadSearch(const int Ax,const int Ay,const int Bx,const int By,const int width,const int height,int grid[][MAXHEIGHT]);
int spreadSearchHelper(const int Ax,const int Ay,const int Bx,const int By,const int currLength,const int width,const int height,int grid[][MAXHEIGHT]);
void clearGrid(const int height, const int width,int grid[][MAXHEIGHT]);

//DEFINE PROGRAM TO FIND SHORTEST PATH BETWEEN CHECKPOINTS
//HAVE IT SUCH THAT AT THE END IT ALSO CHECKS IF IT'S ALREADY CALCULATED OTHER SHORTEST CHECKPOINTS ALONG THE WAY
//ALSO MAKE SURE IT CLEANS UP THE INT ARRAY AFTERWARDS

using namespace std;

class Orienteering {
	public:
		void main();
};

void Orienteering::main() {
	
// TODO: Implement this function
	string line;
	int height;
	int width;
	char place;
	cin>>width>>height;	
//HOLD H AND W VALUES

//CONSTRUCT INT ARRAY
// -1 ARE THE WALLS
//MAX int VALUE AT EVERYTHING ELSE

//CONSTRUCT INT ARRAY OF CHECKPOINT COORDINATES
//18 MAX COORDINATES
//HOLD NUMBER OF COORDINATES

	int grid[width][MAXHEIGHT];
	int checkPoints[18][2];
	int start[2];
	int goal[2];
	int checkPointsNum = 0;
	
	for(int i=0; i<height;i++){
		for(int j=0;j<width;j++){
			cin>>place;
			if(place=='#')
				grid[j][i] = -1;
			else{
				grid[j][i] = MAXINT;
				if(place=='@'){
					checkPoints[checkPointsNum][0] = j;//width
					checkPoints[checkPointsNum][1] = i;//height
					checkPointsNum++;
				}
				else if(place=='S'){
					start[0] = j;
					start[1] = i;
				}
				else if(place=='G'){
					goal[0] = j;
					goal[1] = i;
				}
			}
		}
	}
	
	/*printcheck
	for(int i=0; i<height;i++){
		for(int k=0; k<width;k++)
			cout<<grid[k][i];
		cout<<endl;
	}
	cout<<"start "<<start[0]<<start[1]<<endl;
	cout<<"goal "<<goal[0]<<goal[1]<<endl;

	for(int i=0; i<checkPointsNum;i++){
		cout<<"checkPoint:"<<i<<" "<<checkPoints[i][0]<<checkPoints[i][1]<<endl;
	}	
	end printcheck*/
	
	if(checkPointsNum>18){
		cout<<-1;
		return;
	}
	if(checkPointsNum==0){
		int value=spreadSearch(start[0],start[1],goal[0],goal[1],width,height,grid);
		cout<<value;
		return;
	}
	else if(checkPointsNum==1){
		int first=spreadSearch(start[0],start[1],checkPoints[0][0],checkPoints[0][1],width,height,grid);
		int second=spreadSearch(checkPoints[0][0],checkPoints[0][1],goal[0],goal[1],width,height,grid);
		if(first==-1||second==-1){
			cout<<-1;
			return;
		}
		else{
			cout<<first+second;
			return;
		}
	}
	
	//HERE IT'S MORE THAN ONE CHECKPOINT
	int checkPointCheck[18];
	for(int i=0;i<checkPointsNum;i++)
		checkPointCheck[i]=0;	

	int lengths[checkPointsNum][18];
	for(int i=0; i<checkPointsNum;i++)
		lengths[i][i]=0;
	for(int i=1; i<checkPointsNum;i++){
		for(int j=0; j<i;j++){
			lengths[i][j]=spreadSearch(checkPoints[i][0],checkPoints[i][1],checkPoints[j][0],checkPoints[j][1],width,height,grid);
			if (lengths[i][j]==-1){
				cout<<-1;
				return;
			}	
			lengths[j][i] = lengths[i][j];
		}
	}

	int startLengths[18];
	for(int i=0; i<checkPointsNum;i++)
		startLengths[i]=spreadSearch(start[0],start[1],checkPoints[i][0],checkPoints[i][1],width,height,grid);
		
	int goalLengths[18];
	for(int i=0; i<checkPointsNum;i++)
		goalLengths[i]=spreadSearch(goal[0],goal[1],checkPoints[i][0],checkPoints[i][1],width,height,grid);
		
	int shortest[1];
	shortest[0] = MAXINT;
	
	 if(checkPointsNum<12){//lower than 12
		permutationStart(startLengths,checkPointsNum,goalLengths,shortest,lengths,checkPointCheck);//permutations
		cout<<shortest[0];
		return;
	}
	else{
		double pheromoneStart[18]={};
		double pheromoneGoal[18]={};
		double pheromone[checkPointsNum][18];
		for(int i=0;i<checkPointsNum;i++){
			for(int k=0;k<checkPointsNum;k++){
				pheromone[i][k]=0;
			}
		}
		int checkPointsShuffle[18];
		for(int i=0;i<checkPointsNum;i++){
			checkPointsShuffle[i]=i;
		}
			
		for(int i=0;i<COLONY;i++){
			antAlgorithm(pheromoneStart,pheromoneGoal,pheromone,checkPointsNum,checkPointsShuffle,lengths,startLengths,goalLengths);
			shuffle(checkPointsNum, checkPointsShuffle);
		}
		
		//INPUT FUNCTION TO GOAL
		double mostPheromone=0;
		int bestRouteList[18];
		
		for(int i=0;i<checkPointsNum;i++){
			if(pheromoneStart[i]>mostPheromone){//we want most pheromone
				mostPheromone=pheromoneStart[i];
				bestRouteList[0]=i;
			}		
		}
	//	cout<<"BEST"<<endl;
		bestRoute(bestRouteList[0],checkPointCheck,pheromone,bestRouteList,0,checkPointsNum);
		
		int BestLength = startLengths[(bestRouteList[0])];
		for(int i=1;i<checkPointsNum;i++){
			BestLength = BestLength + lengths[(bestRouteList[i])][(bestRouteList[i-1])];
		}
		BestLength = BestLength + goalLengths[(bestRouteList[checkPointsNum-1])];
		cout<<BestLength;
		return;	
	}
}

int main(int argc, char* argv[]) {
	Orienteering o;
	o.main();
	return 0;
}

void bestRoute(int checkPointID,int checkPointCheck[18],double pheromone[][18],int bestRouteList[18],int bestRoutePlace,const int checkPointsNum){
	
	checkPointCheck[checkPointID]=1;
	bestRouteList[bestRoutePlace]=checkPointID;
//	cout<<"ID:"<<checkPointID<<endl;
	if(bestRoutePlace>=checkPointsNum){
		return;
	}
	
	int atGoal=1;	
	int nextPt;
	double mostPheromone=0;
	for(int i=0;i<checkPointsNum;i++){
		if(checkPointCheck[i]==1){
			i++;
		}
		if(checkPointCheck[i]==0){
			atGoal=0;
			if(pheromone[checkPointID][i]>mostPheromone){
				nextPt = i;
				mostPheromone = pheromone[checkPointID][i];
			}
		}
	}
	
	if(atGoal!=1){
//		cout<<"Most Pheromone/NextPt: "<<nextPt<<endl;
		bestRoute(nextPt,checkPointCheck,pheromone,bestRouteList,(bestRoutePlace+1),checkPointsNum);
	}
	return;
}

void antAlgorithm(double pheromoneStart[18], double pheromoneGoal[18], double pheromone[][18],const int checkPointsNum,int checkPointsShuffle[18],int lengths[][18],int startLengths[18],int goalLengths[18]){
	double pheromoneDensity;
	//cout<<"ant algo"<<endl;
	pheromoneDensity=(1/antAlgorithmHelper(pheromone,checkPointsNum,checkPointsShuffle,lengths,startLengths,goalLengths)); 

	pheromoneStart[(checkPointsShuffle[0])]=pheromoneStart[(checkPointsShuffle[0])]+pheromoneDensity;
	pheromoneGoal[(checkPointsShuffle[checkPointsNum-1])]=pheromoneGoal[(checkPointsShuffle[checkPointsNum-1])]+pheromoneDensity;
	for(int i=1;i<checkPointsNum;i++){
		pheromone[(checkPointsShuffle[i-1])][(checkPointsShuffle[i])] = pheromone[(checkPointsShuffle[i-1])][(checkPointsShuffle[i])] + pheromoneDensity;
		pheromone[(checkPointsShuffle[i])][(checkPointsShuffle[i-1])] = pheromone[(checkPointsShuffle[i-1])][(checkPointsShuffle[i])];
	}
}

double antAlgorithmHelper(double pheromone[][18],const int checkPointsNum,int checkPointsShuffle[18],int lengths[][18],int startLengths[18],int goalLengths[18]){
	double totalLength=0;
	totalLength = startLengths[(checkPointsShuffle[0])] + goalLengths[(checkPointsShuffle[checkPointsNum-1])];
	for(int i=1;i<checkPointsNum;i++){
		totalLength = totalLength + lengths[(checkPointsShuffle[i-1])][(checkPointsShuffle[i])];
	}
	//cout<<"algoHelper "<<totalLength/1000<<endl;
	return totalLength;
}


void shuffle(const int checkPointsNum,int checkPointsShuffle[18]){//Last argument is an array of checkpoints in order
	int random,temp;
	for(int i=0;i<checkPointsNum;i++){
		random = rand()%checkPointsNum;
		cout<<random<<endl;
		temp = checkPointsShuffle[i];
		checkPointsShuffle[i]=checkPointsShuffle[random];
		checkPointsShuffle[random]=temp;
	}
}

void clearGrid(const int height, const int width,int grid[][MAXHEIGHT]){
	for(int i=0; i<height;i++){
		for(int j=0;j<width;j++){
			if(grid[j][i]>=0)
				grid[j][i]=MAXINT;
		}
	}
}

void permutationStart(int startLengths[18],int checkPointsNum,int goalLengths[18],int shortest[1],int lengths[][18],int checkPointCheck[18]){
	for(int i=0;i<checkPointsNum;i++){
		permutation(startLengths[i],checkPointsNum,goalLengths,shortest,lengths,checkPointCheck,i);
	}
}

void permutation(int currentLength,int checkPointsNum,int goalLengths[18],int shortest[1],int lengths[][18],int checkPointCheck[18], int checkPointID){
	checkPointCheck[checkPointID]=1;
	//cout<<"ID:"<<checkPointID<<endl;
	if(currentLength>=shortest[0]){
		checkPointCheck[checkPointID]=0;
		return;
	}
		
	bool atGoal=1;	
	for(int i=0;i<checkPointsNum;i++){
		if(checkPointCheck[i]==0){
			atGoal=0;
			int newLength = currentLength + lengths[checkPointID][i];
			permutation(newLength, checkPointsNum, goalLengths, shortest, lengths, checkPointCheck, i);
			
			if(currentLength>=shortest[0]){
				checkPointCheck[checkPointID]=0;
				return;
			}
		}
	}
	
	if(atGoal){
		if((currentLength+goalLengths[checkPointID])<shortest[0]){
			shortest[0] = (currentLength+goalLengths[checkPointID]);
		}
	}
	
	checkPointCheck[checkPointID]=0;
	return;
}
		
	


int spreadSearch(const int Ax,const int Ay,const int Bx,const int By,const int width,const int height,int grid[][MAXHEIGHT]){
//Grid should already be clean here
	int currLength = 0;
	grid[Ax][Ay] = 0;
	int result;
	
	while(1){
		result = spreadSearchHelper(Ax,Ay,Bx,By,currLength,width,height,grid);
		if (result == -1)
			return -1;
		else if(result>0){
			/*
			for(int i=0; i<height;i++){
				for(int k=0; k<width;k++){
					if(grid[k][i]<10&&grid[k][i]>=0)
						cout<<"0";
					cout<<grid[k][i];
				}
				cout<<endl;
			}
			*/
			clearGrid(height,width,grid);
			return result;
		}
		currLength=currLength+1;
	}
}


int spreadSearchHelper(const int Ax,const int Ay,const int Bx,const int By,const int currLength,const int width,const int height,int grid[][MAXHEIGHT]){
	//return -1 if shortest not found
	//return shortest path otherwise
	//get 4 points of square
	//current length is the spread of the search at this interval
	
	bool updatePresent = 0;
	int left, right, top, bottom;
	left = Ax-currLength;
	right = Ax+currLength;
	top = Ay+currLength;
	bottom=Ay-currLength;
	
	if(left<0)
		left = 0;
	if(right>= width)
		right = width-1;
	if(bottom<0)
		bottom = 0;
	if(top>=height)
		top = height-1;
		
//		cout<<"Left"<<left<<endl;
//		cout<<"Right"<<right<<endl;
//		cout<<"top"<<top<<endl;
//		cout<<"bottom"<<bottom<<endl;
	
	for(int i=left;i<=right;i++){
		for(int j=bottom;j<=top;j++){
			
//			cout<<"grid"<<grid[i][j]<<endl;
//			cout<<"currlength"<<currLength<<endl;
			
			if(grid[i][j]==currLength){
				updatePresent = 1;
//				cout<<"Update present"<<endl;
				//Check if target checkpoint
				if((i==Bx)&&(j==By))
					return grid[i][j];
				//updateNeighbours
				//Left
				if(i!=0){
					if(grid[i-1][j]>(grid[i][j]+1)){
						grid[i-1][j] = (grid[i][j]+1);
					if((i-1==Bx)&&(j==By))
						return grid[i-1][j];
					}
				}
				//Right
				if(i!=width-1){
					if(grid[i+1][j]>(grid[i][j]+1)){
						grid[i+1][j] = (grid[i][j]+1);
					if((i+1==Bx)&&(j==By))
						return grid[i+1][j];
					}
				}
				//Down
				if(j!=0){
					if(grid[i][j-1]>(grid[i][j]+1)){
						grid[i][j-1] = (grid[i][j]+1);
					if((i==Bx)&&(j-1==By))
						return grid[i][j-1];
					}
				}
				//Up
				if(j!=height-1){
					if(grid[i][j+1]>(grid[i][j]+1)){
						grid[i][j+1] = (grid[i][j]+1);
					if((i==Bx)&&(j+1==By))
						return grid[i][j+1];
					}
				}
			}
		}
	}
	if(updatePresent==1)
		return 0;
	else 
		return -1;
}
	


	
