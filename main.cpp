#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

/***********************************************************************************************
 *	Solution to the impossible version of the cigarette smokers problem (without conditional   *
 *  statements or arrays of semaphores as stated in "The Little Book Of Semaphores (2nd Ed.)". *
 *  Well, actually this solution doesn't use much of anything, not even arithmetic.            *
 *	If you think "while()" violates the rules replace them with gotos.												 *
 *  Environment: Visual Studio Community 2015																									 *
 *  Copyright 2015 Karl Szameitat																															 *
 ***********************************************************************************************/

using namespace std;

HANDLE agentSem;
HANDLE tobaccoSem;
HANDLE paperSem;
HANDLE matchSem;
HANDLE flagTobaccoSem;
HANDLE flagPaperSem;
HANDLE flagMatchSem;
HANDLE tobaccoSmokerSem;
HANDLE paperSmokerSem;
HANDLE matchSmokerSem;
HANDLE collect2Sem;
HANDLE let3runSem;
HANDLE noWrapRoundSem1;
HANDLE noWrapRoundSem2;
HANDLE noWrapRoundSem3;
HANDLE nowtSem1;
HANDLE nowtSem2;
HANDLE nowtSem3;
HANDLE oortoSem1;
HANDLE oortoSem2;
HANDLE oortoSem3;
HANDLE waitFor3Sem;

vector<HANDLE> threads;

/*
 * The next three functions are agents that according to the rules are not allowed to be tampered with.
 */
DWORD WINAPI agentA(LPVOID param){
	while(true){
		WaitForSingleObject(agentSem, INFINITE);
		cout << "Tobacco and paper offered." << endl;
		ReleaseSemaphore(tobaccoSem, 1, NULL);
		ReleaseSemaphore(paperSem, 1, NULL);
	}
	return 0;
}

DWORD WINAPI agentB(LPVOID param){
	while(true){
		WaitForSingleObject(agentSem, INFINITE);
		cout << "Paper and a match offered." << endl;
		ReleaseSemaphore(paperSem, 1, NULL);
		ReleaseSemaphore(matchSem, 1, NULL);
	}
	return 0;
}

DWORD WINAPI agentC(LPVOID param){
	while(true){
		WaitForSingleObject(agentSem, INFINITE);
		cout << "Tobacco and a match offered." << endl;
		ReleaseSemaphore(tobaccoSem, 1, NULL);
		ReleaseSemaphore(matchSem, 1, NULL);
	}
	return 0;
}

/*
 *All the functions that follow are smokers or their helpers.
 */
DWORD WINAPI checkTobacco(LPVOID param){
	while(true){
		WaitForSingleObject(tobaccoSem, INFINITE);
		WaitForSingleObject(flagTobaccoSem, INFINITE);
		ReleaseSemaphore(collect2Sem, 1, NULL);
	}
	return 0;
}

DWORD WINAPI checkPaper(LPVOID param){
	while(true){
		WaitForSingleObject(paperSem, INFINITE);
		WaitForSingleObject(flagPaperSem, INFINITE);
		ReleaseSemaphore(collect2Sem, 1, NULL);
	}
	return 0;
}

DWORD WINAPI checkMatch(LPVOID param){
	while(true){
		WaitForSingleObject(matchSem, INFINITE);
		WaitForSingleObject(flagMatchSem, INFINITE);
		ReleaseSemaphore(collect2Sem, 1, NULL);
	}
	return 0;
}

DWORD WINAPI collect2events(LPVOID param){
	while(true){
		WaitForSingleObject(collect2Sem, INFINITE);
		WaitForSingleObject(collect2Sem, INFINITE);
		ReleaseSemaphore(let3runSem, 3, NULL);
	}
	return 0;
}

DWORD WINAPI letSmokerWithTobaccoFree(LPVOID param){
	while(true){
		WaitForSingleObject(let3runSem, INFINITE);
		WaitForSingleObject(flagTobaccoSem, INFINITE);
		ReleaseSemaphore(oortoSem1, 1, NULL);
		oortoSem1 = tobaccoSmokerSem;
		ReleaseSemaphore(flagTobaccoSem, 1, NULL);
		ReleaseSemaphore(waitFor3Sem, 1, NULL);
		WaitForSingleObject(noWrapRoundSem1, INFINITE);
	}
	return 0;
}

DWORD WINAPI letSmokerWithPaperFree(LPVOID param){
	while(true){
		WaitForSingleObject(let3runSem, INFINITE);
		WaitForSingleObject(flagPaperSem, INFINITE);
		ReleaseSemaphore(oortoSem2, 1, NULL);
		oortoSem2 = paperSmokerSem;
		ReleaseSemaphore(flagPaperSem, 1, NULL);
		ReleaseSemaphore(waitFor3Sem, 1, NULL);
		WaitForSingleObject(noWrapRoundSem2, INFINITE);
	}
	return 0;
}

DWORD WINAPI letSmokerWithMatchFree(LPVOID param){
	while(true){
		WaitForSingleObject(let3runSem, INFINITE);
		WaitForSingleObject(flagMatchSem, INFINITE);
		ReleaseSemaphore(oortoSem3, 1, NULL);
		oortoSem3 = matchSmokerSem;
		ReleaseSemaphore(flagMatchSem, 1, NULL);
		ReleaseSemaphore(waitFor3Sem, 1, NULL);
		WaitForSingleObject(noWrapRoundSem3, INFINITE);
	}
	return 0;
}

void makeCigarette(){
}

void smoke(string smoker){
	cout << smoker << " is smoking." << endl;
}

DWORD WINAPI goSmokerWithTobacco(LPVOID param){
	while(true){
		WaitForSingleObject(tobaccoSmokerSem, INFINITE);
		makeCigarette();
		smoke("Smoker with tobacco");
		oortoSem2 = nowtSem2;
		oortoSem3 = nowtSem3;
		ReleaseSemaphore(flagPaperSem, 1, NULL);
		ReleaseSemaphore(flagMatchSem, 1, NULL);
		WaitForSingleObject(nowtSem2, INFINITE);
		WaitForSingleObject(nowtSem3, INFINITE);
		cout << "===================================================" << endl;
		cout << "======== new round ================================" << endl;
		cout << "===================================================" << endl;
		WaitForSingleObject(waitFor3Sem, INFINITE);
		WaitForSingleObject(waitFor3Sem, INFINITE);
		WaitForSingleObject(waitFor3Sem, INFINITE);
		ReleaseSemaphore(noWrapRoundSem1, 1, NULL);
		ReleaseSemaphore(noWrapRoundSem2, 1, NULL);
		ReleaseSemaphore(noWrapRoundSem3, 1, NULL);
		ReleaseSemaphore(agentSem, 1, NULL);
	}
	return 0;
}

DWORD WINAPI goSmokerWithPaper(LPVOID param){
	while(true){
		WaitForSingleObject(paperSmokerSem, INFINITE);
		makeCigarette();
		smoke("Smoker with papers");
		oortoSem1 = nowtSem1;
		oortoSem3 = nowtSem3;
		ReleaseSemaphore(flagTobaccoSem, 1, NULL);
		ReleaseSemaphore(flagMatchSem, 1, NULL);
		WaitForSingleObject(nowtSem1, INFINITE);
		WaitForSingleObject(nowtSem3, INFINITE);
		cout << "===================================================" << endl;
		cout << "======== new round ================================" << endl;
		cout << "===================================================" << endl;
		WaitForSingleObject(waitFor3Sem, INFINITE);
		WaitForSingleObject(waitFor3Sem, INFINITE);
		WaitForSingleObject(waitFor3Sem, INFINITE);
		ReleaseSemaphore(noWrapRoundSem1, 1, NULL);
		ReleaseSemaphore(noWrapRoundSem2, 1, NULL);
		ReleaseSemaphore(noWrapRoundSem3, 1, NULL);
		ReleaseSemaphore(agentSem, 1, NULL);
	}
	return 0;
}

DWORD WINAPI goSmokerWithMatch(LPVOID param){
	while(true){
		WaitForSingleObject(matchSmokerSem, INFINITE);
		makeCigarette();
		smoke("Smoker with matches");
		oortoSem1 = nowtSem1;
		oortoSem2 = nowtSem2;
		ReleaseSemaphore(flagPaperSem, 1, NULL);
		ReleaseSemaphore(flagTobaccoSem, 1, NULL);
		WaitForSingleObject(nowtSem1, INFINITE);
		WaitForSingleObject(nowtSem2, INFINITE);
		cout << "===================================================" << endl;
		cout << "======== new round ================================" << endl;
		cout << "===================================================" << endl;
		WaitForSingleObject(waitFor3Sem, INFINITE);
		WaitForSingleObject(waitFor3Sem, INFINITE);
		WaitForSingleObject(waitFor3Sem, INFINITE);
		ReleaseSemaphore(noWrapRoundSem1, 1, NULL);
		ReleaseSemaphore(noWrapRoundSem2, 1, NULL);
		ReleaseSemaphore(noWrapRoundSem3, 1, NULL);
		ReleaseSemaphore(agentSem, 1, NULL);
	}
	return 0;
}

int main(){
	agentSem = CreateSemaphore(NULL, 0, 3, NULL);
	tobaccoSem = CreateSemaphore(NULL, 0, 3, NULL);
	paperSem = CreateSemaphore(NULL, 0, 3, NULL);
	matchSem = CreateSemaphore(NULL, 0, 3, NULL);
	flagTobaccoSem = CreateSemaphore(NULL, 0, 3, NULL);
	flagPaperSem = CreateSemaphore(NULL, 0, 3, NULL);
	flagMatchSem = CreateSemaphore(NULL, 0, 3, NULL);
	tobaccoSmokerSem = CreateSemaphore(NULL, 0, 3, NULL);
	paperSmokerSem = CreateSemaphore(NULL, 0, 3, NULL);
	matchSmokerSem = CreateSemaphore(NULL, 0, 3, NULL);
	collect2Sem = CreateSemaphore(NULL, 0, 3, NULL);
	let3runSem = CreateSemaphore(NULL, 0, 6, NULL);
	noWrapRoundSem1 = CreateSemaphore(NULL, 0, 1, NULL);
	noWrapRoundSem2 = CreateSemaphore(NULL, 0, 1, NULL);
	noWrapRoundSem3 = CreateSemaphore(NULL, 0, 1, NULL);
	nowtSem1 = CreateSemaphore(NULL, 0, 3, NULL);
	nowtSem2 = CreateSemaphore(NULL, 0, 3, NULL);
	nowtSem3 = CreateSemaphore(NULL, 0, 3, NULL);
	waitFor3Sem = CreateSemaphore(NULL, 0, 3, NULL);
	oortoSem1 = tobaccoSmokerSem;
	oortoSem2 = paperSmokerSem;
	oortoSem3 = matchSmokerSem;
	threads.push_back(CreateThread(NULL, 0, agentA, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, agentB, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, agentC, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, checkTobacco, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, checkPaper, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, checkMatch, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, letSmokerWithTobaccoFree, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, letSmokerWithPaperFree, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, letSmokerWithMatchFree, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, goSmokerWithTobacco, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, goSmokerWithPaper, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, goSmokerWithMatch, NULL, 0, NULL));
	threads.push_back(CreateThread(NULL, 0, collect2events, NULL, 0, NULL));
	ReleaseSemaphore(flagTobaccoSem, 1, NULL);
	ReleaseSemaphore(flagPaperSem, 1, NULL);
	ReleaseSemaphore(flagMatchSem, 1, NULL);
	ReleaseSemaphore(agentSem, 1, NULL); // 
	WaitForMultipleObjects(13, &threads[0], true, INFINITE);
	cout << "Oops, infinity has arrived!" << endl;
	return 0;
}
