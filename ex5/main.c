/******************
* Aviad Benshoshan
* 318528874
* 01
* ex5
*******************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "ex5.h"

int main (){
	char choice;
	int currentAmount = 0;
	Class_t* classes= (Class_t*)calloc(MAX_CLASSES, sizeof(Class_t));
	if (classes == NULL) {
		exit(1);
	}
	printMenu();
	do {
		scanf(" %c", &choice);
			switch (choice){
			case '0':{
			freeAll(classes, currentAmount);
			break;
		}	case '1':{
				//addClass function returns True/False if addition of new class was made
			int addClassRes = addClass(classes, currentAmount);

			/*****************************************************************
			if class was added, amount of classes in array is incremented by 1
			******************************************************************/
			if (addClassRes == True) {
				currentAmount++;
			}
			break;
		}case '2':{
			addStudentToClass(classes, currentAmount);
			break;
		}case '3':{
			removeStudentFromClass(classes, currentAmount);
			break;
		}case '4':{
			printClass(classes, currentAmount);
			break;
		}case '5':{
			if (currentAmount == 0) {
				printf("Error: there are no classes.\n");
			}else {
				char secondChoice;
				printf("Please select the aggregation method:\n\ta. Average Grade.\n\tb. Maximal Grade.\n\t"
					"c. Minimal Grade.\n\td. Sum.\n\te. Count.\n\t0. Return to the main menu.\n");
				scanf(" %c", &secondChoice);
				switch (secondChoice) {
				case '0':{
					break;
				}case 'a':{
					avgGrade(classes, currentAmount);
					break;
				}case 'b':{
					maxGrade(classes, currentAmount); 
					break;
				}case 'c':{
					minGrade(classes, currentAmount);
					break;
				}case 'd':{
					sumOfGrades(classes, currentAmount);
					break;
				}case 'e':{
					countOfGrades(classes, currentAmount);
					break;
				}
				}
			}
			break;
		}case '6':{
			printMenu();
			break;
		}default:{
			printf("Error: unrecognized operation.\n");
		}
		}if (choice != '6' && choice != '0') {
			printf("Select the next operation (insert 6 for the entire menu):\n");
		}
	} while (choice != '0');

		return 0;
}