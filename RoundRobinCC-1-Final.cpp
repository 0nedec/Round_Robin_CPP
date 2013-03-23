/*Round Robin Simulation
 *by Carlos A. Cedeño
 *0684931
 *NYiT Spring 2012
 *Operating Systems
 *Prof. Gass
*/


#include <iostream>
#include <stdio.h>
#include <deque>
#include <vector>

using namespace std;

class Process {
      private:
              int pid, pRemainingBurstTime, pTotalBurstTime, pArrivalTime, contextSwitch, pWaitTime, pTurnaroundTime;
              bool finished;
      public:
             Process();//constructor
			 void setAttributes();
             void setPid(int i);//sets PID
             void setPBurstTimes(int i);//sets remaing time for process completion time.
             void setPRemainingTime( int i);//sets the remainging burst time of the process to i;
             void setPArrivalTime(int i );//sets Arrival time.
             void setIsFinished();//sets Finished to true.
             void setTurnAroundTime();
             
             int getPid();//gets the PID
             int getPRemainingBurstTime();//Gets the amount of time remaining in the process before finished.
			 int getPArrivalTime();//Gets Arrival time
			 int getPWaitTime();//gets the waiting time process had in RQ
			 bool isFinished();//returns current state of finished variable
  
             void incrementPWaitTime( int quantum );//increments the wait time by quantum.
             void incrementPContextSwitch();//increments the times this particular Process has been switched. 
             void decrementPArrivalTime( int quantum );//decrements time before process is created by quantum specified.
             int calcTurnAroundTime();
             void display();
};

Process::Process() {
                   pid = 0;
                   pRemainingBurstTime = 0;
				   pTotalBurstTime = 0;
                   pArrivalTime = 0;
                   contextSwitch = 0;
                   pWaitTime = 0;
                   pTurnaroundTime = 0;
                   finished = false;
                   }
                   
void Process::setAttributes() {
	int npid, nburstTime, narrivalTime;
	cout << "Please enter process ID: "; cin >> npid; cout <<endl;
	cout << "Please enter burst time: "; cin >> nburstTime; cout <<endl;
	cout << "Please enter time to wait before placement in ready queue: "; cin >> narrivalTime; cout <<endl;
	setPid(npid);
	setPBurstTimes(nburstTime);
	setPArrivalTime(narrivalTime);
}

void Process::setPid( int i) {
     pid = i;
     }
void Process::setPBurstTimes( int i ) {
     pTotalBurstTime = i;
	 pRemainingBurstTime = i;
     }
void Process::setPRemainingTime( int i ) {
	pRemainingBurstTime = i;
	}
void Process::setPArrivalTime( int i ) {
     pArrivalTime = i;
     }
void Process::setIsFinished(){
        finished = true;
        }
		
int Process::getPid() {
    return pid;
}
int Process::getPRemainingBurstTime() {
    return pRemainingBurstTime;
}
int Process::getPArrivalTime() {
    return pArrivalTime;
}

int Process::getPWaitTime(){
	return pWaitTime;
}

bool Process::isFinished() {
     return finished;
}

void Process::setTurnAroundTime() {
     pTurnaroundTime = calcTurnAroundTime();
}

void Process::incrementPWaitTime (int quantum ) {

	pWaitTime += quantum;
}
void Process::incrementPContextSwitch() {
    contextSwitch++;
}
void Process::decrementPArrivalTime( int quantum ) {
		pArrivalTime -= quantum ;
}
int Process::calcTurnAroundTime() {
	pTurnaroundTime = pWaitTime + pTotalBurstTime;
	return (pTurnaroundTime);
}

void Process::display() {
       cout << "*****************************************" << endl;
       cout << "Pid: " << pid << endl;
       cout << "Remaing Burst Time: " << pRemainingBurstTime << endl;
       cout << "Initial Burst Time Given: " << pTotalBurstTime << endl;
       cout << "Time Until Arrival to Ready Queue: " << pArrivalTime << endl;
       cout << "Context Switches Thus Far: " << contextSwitch << endl;
       cout << "Time Spent Waiting in Ready Queue: " << pWaitTime << endl;
       cout << "Turnaround Time: " << pTurnaroundTime << endl;
       cout << "****************************************" << endl << endl;
}
     
	
//end of Process class definition


class CPU {

private:
  int  quantum;//timeslice 
  int completedProcessCounter;//counts completed processes used to show throughput

public:
  CPU();//constructor
  void setQuantum( int i );//sets the amount of time CPU will spend on each process.
  int runProcess(Process* p);//changes attributes of the running process accordingly, returns amount of time spent on process.
  int getCPC();//returns value of completedProcessCounter for calculation of throughput.
  };


CPU::CPU() {//constructor
  quantum = 0;
  completedProcessCounter= 0;
}

void CPU::setQuantum(int i){//sets amount of the time slice to run each process
  quantum = i;
}

int CPU::runProcess(Process* p){//simulates the running of a process also sets the is finished flag of the process; returns the amount of time spent on process. 
	int i;
	if (p->getPRemainingBurstTime() > quantum) { //if more time required to finish process...
		i = p->getPRemainingBurstTime() - quantum;//calculate time remaining for process to finish after this particular burst is finished.
		p->setPRemainingTime(i);//update time remaining.
		return (quantum);//since CPU spent entire quantum processing this process return quantum.
	}
	else {
		i = p->getPRemainingBurstTime();//if less time is required to finish process...
		p->setPRemainingTime( 0 );//finish the process..
		p->setIsFinished();//Set as finished. 
		completedProcessCounter++;
		return (i);//return only the time that was left in finishing the process.
	}
}

int CPU::getCPC() {//used for displaying throughput.
	return (completedProcessCounter);
}

//end of CPU class definition


class SimClock {
	 
    private: 
	int i; //iterator for traversing Queues.
	int I;//counter of Total time since Simulation started.

	public: 
    SimClock();
	void updateWQ( vector<Process*>& WQ, int quantum); //function for updating process time in wait queue;
	void updateRQ( deque <Process*>& RQ, int quantum);//function for updating process time in ready queue;
	void incrementI( int quantum);
	int getI();
	
	};
SimClock::SimClock() {
               i = 0;//iterator to traverse vector or deque
               I = 0;
               }
	
void SimClock::updateWQ ( vector<Process*>& wQ, int quantum) {//decrements the time-until-arrival-to-readyqueue for each process in Waiting queue.
	
	for (i = 0; i < wQ.size(); i++) {//for every quantum every process in WaitQ is decremented that amount of time it has to wait to move to ReadyQ.

		wQ[i]->decrementPArrivalTime( quantum);
	}
	
} 
void SimClock::updateRQ ( deque<Process*>& RQ, int quantum ) {//increments the waiting time for each process in Ready Queue.
	for (i = 0; i < RQ.size()-1; i++) { //for every element in Queue except the last which is being used by CPU;
		RQ[i]->incrementPWaitTime( quantum );
	}
}

void SimClock::incrementI (int quantum ) {
	I += quantum ;
	}
int SimClock::getI() {
	return (I);
	
}
//end of SimClock Class definition
	





void processCreator ( vector<Process*>& prcsPtr, deque<Process*>& rQ ) {//checks to see if  time for each process has reached zero. If so places Process in ReadyQ
    int i; //iterator to traverse list
    

	for (i = 0; i < prcsPtr.size(); i++ ) {//for every process in list prcsPtr
       	if (prcsPtr[i]->getPArrivalTime() == 0) {//checks if it's arrival time has reached zero... if so...
		rQ.push_front(prcsPtr[i]);//place it in Ready Queue
		}
	}	
}

bool finishedRunningAllProc( vector<Process*>& Q) {//checks to see if all Processes have finished running.
     for (int i = 0; i < Q.size(); i++ ) {//for all processes in Q
         if (Q[i]->isFinished()){//if its finished...
         continue;//keep going
         }
         else {
              return (false);//if one is found to not be finished return false
              }
         }
    return(true);//otherwise retunrs true...
}
  

	
void roundRobin(vector<Process*>& wQ, deque<Process*>& rQ, CPU& cpu1, SimClock& clock) {
    while (!finishedRunningAllProc( wQ )){
			Process* currentRunningProc;
			currentRunningProc = rQ.back();//temporary holding place
			currentRunningProc->incrementPContextSwitch();// since it was in the ready que and is now being processed increment Context switch counter
			int quantum = cpu1.runProcess(currentRunningProc);// run the current process and returns the amount of time spent running that process
			clock.incrementI(quantum);//sets the total running time so far of the simulation
			for (int x = 0; x < quantum; x++) { //for every time quantum spent processing...
				clock.updateRQ(rQ, 1);//increases the time spent waiting in ready queue by 1 for each process not running.
				clock.updateWQ(wQ, 1);//decreases the "time to arrive to RQ" by 1;
				processCreator(wQ, rQ);//if a waiting to be created process reaches zero insert into ready queue
			}
			if (currentRunningProc->isFinished()){//if the current running process has finished...
				cout << endl << endl << "Process: " << currentRunningProc->getPid() << " finished running @ " << clock.getI() << " time units..." << endl << endl; //tells user when process finished
				rQ.pop_back();//removes it from ready queue
			}
			
			else {//otherwise...
				currentRunningProc->incrementPContextSwitch();//increment context switch since it's being placed back in ready queue
				rQ.push_front(currentRunningProc);//return the running process to the front of ready queue
				rQ.pop_back();//remove the last process from the end of the queue since it is now in the front of the queue
	}
	}

}	



void results(vector<Process*>& finishedProcesses, CPU& cpu1, SimClock& clock){
	float avgWaitTime, avgTurnaroundTime;
	int cumWaitTime = 0;
	int cumTurnaroundTime = 0;
	
	for (int i = 0; i < finishedProcesses.size(); i ++) {
		cumWaitTime += finishedProcesses[i]->getPWaitTime();
		cumTurnaroundTime += finishedProcesses[i]->calcTurnAroundTime();
		}
		
		avgWaitTime = cumWaitTime/finishedProcesses.size();
		avgTurnaroundTime = cumTurnaroundTime/finishedProcesses.size();
		cout << "The average wait time was: " << avgWaitTime << endl;
		cout << "The average turn around time was: "<< avgTurnaroundTime << endl;
		cout << "The throughput was: "<< cpu1.getCPC() <<"/"<< clock.getI() << endl << endl;
}

void simulation(vector<Process*>& wQ, int quantum) {
	deque<Process*> rQ;// Double Ended Queue represents the Ready Queue
	SimClock clock;//clock that takes care of timestamping
	CPU cpu1;//cpu object for this simulation
	cpu1.setQuantum( quantum );//sets the time quantum for the cpu to run
	
	processCreator(wQ, rQ);//Goes through all the processes created if any are ready to be placed in the ready queue it does so.
	roundRobin(wQ, rQ, cpu1, clock);// places process in cpu to be run for alloted quantum if process finishes removed form que, otherwise process is placed in the back of que. (also updates wait time for all process in RQ not being processed.)
	results(wQ, cpu1, clock);
	}		
	
int main() {
    
	int n , exitkey, quantum; //number of processes, exitkeypress variable, quantum for cpu;
	
	
	vector<Process*> prcsPtr;//vector of addresses of processes created.
	
	cout << "Please enter number of Processes you wish to create: "; cin >> n;
	cout << endl;
	
	for (int i = 0; i < n; i++) {//for desired amount or processes...
	prcsPtr.push_back(new Process());//create a new process object...
	cout << "Creating Process " << i+1 << "..."<< endl;
	prcsPtr[i]->setAttributes();//set it's attributes...
	prcsPtr[i]->display();//display the attributes of the recently created process...
	}
	cout << "Please enter time quantum for this simulation: "; cin >> quantum;//time quantum for cpu for this simulation
    simulation(prcsPtr, quantum);//run simulation 
    	
		for (int i = 0; i < n; i++) {
            prcsPtr[i]->setTurnAroundTime();
            prcsPtr[i]->display();
            }
	
	cout << "Press any key to exit..." << endl; 
	cin >> exitkey;

return 0;
}
