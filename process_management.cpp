#include <iostream>             // BASIC INPUT/OUTPUT STREAM
#include <fstream>              // FOR FILE OPERATIONS
#include <cstdlib>              // FOR GENERAL PURPOSE FUNCTIONS
#include <string>               // TO USE A COMPLETE STRING
#include <stdio.h>              // STAND INPUT/OUTPUT FOR FILE OPERATIONS
#include <unistd.h>             // FOR POSIX OPERATING SYSTEM API  
#include <sys/types.h>          // FOR SYSTEM CALLS
#include <sys/wait.h>           // TO MAKE A PROCESS ON WAIT
#include <chrono>               // FOR HIGH RESOLUTINO CLOCK

using namespace std;            // NAEMSPACE FOR STANDARD LIBRARY
using namespace std::chrono;    // NAMESPACE FOR HIGH RESOLUTION CLOCK


/* ----------START OF CONSTANT VARIABLES-----------  */

const int SIZE_OF_THE_ARRAY = 10000;        
const int MAX_RANGE_OF_NUMBERS = 1000000;
const string RANDOM_NUMBER_FILE_PATH = "/home/faqeeh/Desktop/numbers.txt"; // Absolute path
const string PRIME_NUMBER_FILE_PATH = "/home/faqeeh/Desktop/prime.txt"; // Absolute path 
const string PRIME_NUMBER_FILE_PATH_FOR_CHILD_PROCESSES = "/home//Desktop/prime2.txt"; // Absolute path
int ARRAY_OF_RANDOM_NUMBER[SIZE_OF_THE_ARRAY];

/* -----------END OF CONSTANT VARIABLES-----------  */

/* -------------------------------------------------------------------------------------------------------------------------------------------------------- */

/* -----------START OF GENERATING RANDOM NUMBER FUNCTION-----------  */

void generateRandomNumber () {
    
    // Initializing random number generator in C++
    srand ((unsigned int) time(0));

    for (int i = 0; i < SIZE_OF_THE_ARRAY; i++) {
        
        ARRAY_OF_RANDOM_NUMBER[i] = rand() % MAX_RANGE_OF_NUMBERS; 
    }
}

/* -----------END OF GENERATING RANDOM NUMBER FUNCTION-----------  */

/* -------------------------------------------------------------------------------------------------------------------------------------------------------- */

/* ----fir commi-------START OF PRINTING RANDOM NUMBER FUNCTION-----------  */

void printRandomNumber () {
    
    int length = sizeof(ARRAY_OF_RANDOM_NUMBER) / sizeof(int);
    
    if (length > 0) {
        
        for (int i = 0; i < length; i++) {
            
            cout<< "Index => " << i << ", Value  => " << ARRAY_OF_RANDOM_NUMBER[i]<<"\n";
        }
        
    } else {
        
        cout<<"We are sorry, kindly generate random numbers again\n";
    }

}

/* -----------END OF PRINTING RANDOM NUMBER FUNCTION-----------  */

/* -------------------------------------------------------------------------------------------------------------------------------------------------------- */

/* -----------START OF PRINTING PRIME NUMBER FUNCTION-----------  */

void printPrimeNumber () {
    
    fstream FILE; // PRIME NUMBER FILE, PROVIDES CAPABILITY OF READING, WRITING AND CREATING FOR FILE
    FILE.open(PRIME_NUMBER_FILE_PATH,ios::in);
    string lineHavingNumber;
    int j = 0;

    if (FILE.is_open()){   //checking whether the file is open

        while(getline(FILE, lineHavingNumber)){ //read data from file object and put it into string.

            cout<< "Index => " << j << ", Value => " << lineHavingNumber<<endl;
            j++;
        }
    }

    FILE.close(); //close the file object.
}

/* -----------END OF PRINTING PRIME NUMBER FUNCTION-----------  */

/* -------------------------------------------------------------------------------------------------------------------------------------------------------- */

/* -----------START OF SAVING RANDOM NUMBER INTO TEXT FILE FUNCTION-----------  */

void saveRandomNumbersIntoTextFile () {
    
    ofstream FILE; // RANDOM NUMBER FILE, PROVIDES CAPABILITY OF READING, WRITING AND CREATING FOR FILE
    
    FILE.open(RANDOM_NUMBER_FILE_PATH); 

    for (int i = 0;i < SIZE_OF_THE_ARRAY; i++){

    	FILE<<ARRAY_OF_RANDOM_NUMBER[i]<<endl;
    }
    
    FILE.close();
    
}

/* -----------END OF SAVING RANDOM NUMBER INTO TEXT FILE FUNCTION-----------  */

/* -------------------------------------------------------------------------------------------------------------------------------------------------------- */

/* -----------START OF PRIME NUMBER CHECKER FUNCTION-----------  */

bool checkPrimeNumber (int number) {
    
    bool isPrime = true;
    
    if (number == 0 || number == 1) {
        
        isPrime = false;
        
    } else {
        
        for (int i = 2; i <= number / 2; ++i) {
            
            if (number % i == 0) {
                
              isPrime = false;
              break;
            }
        }
    }
    
    return isPrime;
}

/* -----------END OF PRIME NUMBER CHECKER FUNCTION-----------  */

/* -------------------------------------------------------------------------------------------------------------------------------------------------------- */

/* -----------START OF PRIME NUMBER FINDER FROM TXT FILE AND SAVING PRIME NUMBER INTO NEW TEXT FILE FUNCTION-----------  */

void findAndSavePrimeNumberFromTextFile () {

    fstream FILE; // random NUMBER FILE
    FILE.open(RANDOM_NUMBER_FILE_PATH,ios::in);
    
    if (FILE.is_open()){   //checking whether the file is open

        string lineHavingNumber;
        ofstream P_FILE; // PRIME NUMBER FILE
        P_FILE.open(PRIME_NUMBER_FILE_PATH); 

        while(getline(FILE, lineHavingNumber)){ //read data from file object and put it into string.

            int isPrime = checkPrimeNumber(stoi(lineHavingNumber)); //STOI, CONVERTS STRING INTO AN INTEGER
            
            if (isPrime == 1) {
                
                P_FILE<<stoi(lineHavingNumber)<<endl;
            }
        }

        P_FILE.close(); //close the file object.
        FILE.close(); //close the file object.
   }
}

/* -----------END OF PRIME NUMBER FINDER FROM TXT FILE AND SAVING PRIME NUMBER INTO NEW TEXT FILE FUNCTION-----------  */

/* -------------------------------------------------------------------------------------------------------------------------------------------------------- */

/* -----------START OF GENERATING CHILD PROCESSESS AND FINDIND PRIME NUMBER FUNCTION-----------  */

void primeNumberFinderThroughChildProcesses (int num_jobs) {
    
    double totalStartTime = 0, totalEndTime = 0;
    double time_taken;
    
    long timeTakenByFileReading = 0;
    long timeTakenByFileWriting = 0;
    long timeTakenByFindingPrimeNumber = 0;

    long totalTimeTakenByFileReading = 0;
    long totalTimeTakenByFileWriting = 0;
    long totalTimeTakenByFindingPrimeNumber = 0;

    int sizeOfInputPerProcess = SIZE_OF_THE_ARRAY / num_jobs;
    int currentLine, nextLine;
    currentLine = 0;
    nextLine = sizeOfInputPerProcess;
    string lineHavingNumber;

    fstream R_FILE;
    R_FILE.open(RANDOM_NUMBER_FILE_PATH,ios::in);
    
    ofstream P2_FILE;
    P2_FILE.open(PRIME_NUMBER_FILE_PATH_FOR_CHILD_PROCESSES); 

    if (R_FILE.is_open() && P2_FILE.is_open()){
        // cout<<"My PID is: "<<getpid()<<endl;

        // FILE READING TIME START
        auto startTimeOfFileReading = high_resolution_clock::now();
        for (int i = 1; i <= num_jobs; i++) {

            int pid = fork();
            if (pid == 0) {
                // cout<<"Chilld process "<<i<<": "<<getpid()<<", "<<getppid()<<endl;
                for (int j = currentLine; getline(R_FILE, lineHavingNumber) && j < nextLine; j++) {

                    // cout<<"Line No: "<<j<<", String: "<<lineHavingNumber<<endl;

                    // PRIME NUMBER CHECKER TIMER START
                        auto startTimeOfCheckingPrimeNumber = high_resolution_clock::now();
                        int isPrime = checkPrimeNumber(stoi(lineHavingNumber));
                        if (isPrime == 1) {
                        auto endTimeOfCheckingPrimeNumber = high_resolution_clock::now();
                    // PRIME NUMBER CHECKER TIME END

                    // START OF MAKING TOTAL TIME TAKEN BY PRIME NUMBER CHEKCER
                        auto durationTimeOfCheckingPrimeNumber = duration_cast<microseconds>(endTimeOfCheckingPrimeNumber - startTimeOfCheckingPrimeNumber);
                        timeTakenByFindingPrimeNumber += durationTimeOfCheckingPrimeNumber.count();
                    // END OF MAKING TOTAL TIME TAKEN BY PRIME NUMBER CHEKCER

                    // FILE WRITING TIME START
                        auto startTimeOfFileWriting = high_resolution_clock::now();
                        P2_FILE<<stoi(lineHavingNumber)<<endl;
                        auto endTimeOfFileWriting = high_resolution_clock::now();
                    // FILE WRITING TIME END
                         
                    // START OF MAKING TOTAL TIME TAKEN BY FILE WRITING FUNCTION
                        auto durationTimeOfFileWriting = duration_cast<microseconds>(endTimeOfFileWriting - startTimeOfFileWriting);
                        timeTakenByFileWriting += durationTimeOfFileWriting.count();
                    // START OF MAKING TOTAL TIME TAKEN BY FILE WRITING FUNCTION                    
                    }
                }

                exit(0);
            } 

            auto endTimeOfFileReading = high_resolution_clock::now();
            // FILE READING TIME END

            // START OF MAKING TOTAL TIME TAKEN BY FILE READING FUNCTION
                auto durationTimeOfFileReading = duration_cast<microseconds>(endTimeOfFileReading - startTimeOfFileReading);
                timeTakenByFileReading += durationTimeOfFileReading.count();
            // END OF MAKING TOTAL TIME TAKEN BY FILE READING FUNCTION

            cout << "Reading function time for process " << i << " : " << timeTakenByFileReading << " microseconds" << endl;
            cout << "Writing function time for process " << i << " : " << timeTakenByFileWriting << " microseconds" << endl;
            cout << "Prime Number Function time for process " << i << " : " << timeTakenByFindingPrimeNumber << " microseconds" << endl << endl;

            totalTimeTakenByFileReading += timeTakenByFileReading;
            totalTimeTakenByFileWriting += timeTakenByFileWriting;
            totalTimeTakenByFindingPrimeNumber += timeTakenByFindingPrimeNumber;
        }

        cout << "\n\nTotal time for Reading function: " << totalTimeTakenByFileReading << " microseconds" << endl;
        cout << "Total time for Writing function: " << totalTimeTakenByFileWriting << " microseconds" << endl;
        cout << "Total time for Prime Number Function: " << totalTimeTakenByFindingPrimeNumber << " microseconds" << endl;

    } else {

        cout<<"Error in opening file\n";
    }

    // cout << "Reading function: " << timeTakenByFileReading << " microseconds" << endl;      
    // cout << "Writing function: " << timeTakenByFileWriting << " microseconds" << endl;    
    // cout << "Prime Number Function: " << timeTakenByFindingPrimeNumber << " microseconds" << endl;
  

    R_FILE.close();
    P2_FILE.close();
}

/* -----------END OF GENERATING CHILD PROCESSESS AND FINDIND PRIME NUMBER FUNCTION-----------  */

/* -------------------------------------------------------------------------------------------------------------------------------------------------------- */

/* -----------START OF MAIN FUNCTION-----------  */

int main(){
    
    int i = 0;
    char a;
    int z;
    
    cout<<"\n\nWelcome to Process creation and management Assignment # 1 of AOS\n\n";
    cout<<"\t\t Name: Muhammad Akbar\n\t\t Class: MSCS-2k22-A\n\t\t Reg#: 400552\n\t\t Email: makbar.mscs22seecs@seecs.edu.pk\n\t\t Subject: Advance Operating System\n\t\t Professor: Honourable Dr. Hassan Ali Khattak\n\t\t\t    Associate Professor, Department of Computing\n\t\t\t    School of Electrical Engineering & Computer Science (SEECS)\n\t\t\t    National University of Sciences & Technology (NUST), H12, Islamabad 44000, Pakistan\n\t\t\t    hasan.alikhattak@seecs.edu.pk\n\n ";
    cout<<"To Generate Random Numbers, please click any button:";
    cin>>a;

    generateRandomNumber();

    cout<<"\n\nRandom numbers have been generated, do you want to view them[y/n]:";
    cin>>a;

    if ( a == 'y' || a == 'Y' ) {

        printRandomNumber();

    }

    cout<<"\n\nRandom numbers have been generated already, do you want to save them[y/n]:";
    cin>>a;

    if ( a == 'y' || a == 'Y' ) {

        saveRandomNumbersIntoTextFile();

        cout<<"\n\nRandom numbers have been saved into the txt file, do you want to save prime numbers also[y/n]:";
        cin>>a;

        if (a == 'y' || a == 'Y') {

            findAndSavePrimeNumberFromTextFile();

            cout<<"\n\nPrime numbers have been saved into the txt file, do you want to view them[y/n]:";
            cin>>a; 

            if (a == 'y' || a == 'Y') {

                printPrimeNumber();
            }

            cout<<"\n\nPrime numbers are already saved into the txt file, do you want to apply multiprocessig to find prime numbers[y/n]:";
            cin>>a;

            if (a == 'y' || a == 'Y') {

                cout<<"\n\nEnter the number of processes which you want to run from (1 - 8):";
                cin>>z;
                cout<<"\n\n";

                if ( z > 0 && z < 9 ) {

                    primeNumberFinderThroughChildProcesses(z);

                } else {

                    cout<<"\n\nPlease re run the program to test its functionality, you entered value out of the boundry!";
                    cin>>a; 
                }
            }

        } else {

            cout<<"\n\nPlease re run the program to test its functionality by saving prime numbers!";
            cin>>a; 
        }

    } else {

        cout<<"\n\nPlease re run the program to test its functionality by saving random numbers!";
        cin>>a;
    }

    cout<<"\n\nThank you for using the software!\n";
    
    // auto stop = high_resolution_clock::now();
    // auto duration = duration_cast<microseconds>(stop - start);
 
    // cout << "Time taken by function: "
    //      << duration.count() / 1000000 << " microseconds" << endl;


    // generateARRAY_OF_RANDOM_NUMBER();
    // printARRAY_OF_RANDOM_NUMBER();
    // saveRandomumbersIntoTextFile();
    // primeNumberFinderFromFile();
    // primeNumberFinderThroughChildProcesses();
    
    // -----------------------------------------------------------------------------------------
    
    // time(&start);
    // time(&end);
    // double time_taken = double(end - start);
    // cout << "Time taken by program is : " << fixed
    //      << time_taken;
    // cout << " sec " << endl;
    
    // -----------------------------------------------------------------------------------------
    
    // Data Type: pid_t
    //      The pid_t data type is a signed integer type which is capable of representing a process ID. In the GNU library, this is an int.
    // Function: pid_t getpid (void)
    //      The getpid function returns the process ID of the current process.
    // Function: pid_t getppid (void)
    //      The getppid function returns the process ID of the parent of the current process.

    // -----------------------------------------------------------------------------------------

    // cout<<"My PID is: "<<getpid()<<endl;
    // int pid = fork();
    // if (pid == 0) {
    //     cout<<"Chilld process 1: "<<getpid()<<", "<<getppid()<<endl;
    // } else{
    //     pid = fork();
    //     if (pid==0) {
    //         cout<<"Chilld process 2: "<<getpid()<<", "<<getppid()<<endl;
    //     } else {
    //         pid = fork();
    //         if (pid == 0) {
    //           cout<<"Chilld process 3: "<<getpid()<<", "<<getppid()<<endl; 
    //         }
    //     }
    // }
    
    // -----------------------------------------------------------------------------------------
    
}

/* -----------END OF MAIN FUNCTION-----------  */

