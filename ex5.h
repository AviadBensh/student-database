/******************
* Aviad Benshoshan
* 318528874
* 01
* ex5
*******************/
#ifndef EX5_H
#define EX5_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define MAX_USER_INPUT 200
#define MAX_CLASSES 5
#define CLASS_NUM_LEN 5
#define CLASS_NAME_LEN 30
#define STUDENT_NAME_LEN 20
#define THOUSAND_DEC 4
#define SINGLE 1
#define END_OF_STRING '\0'
#define SPACE ' '
#define COLON ':'
#define COMMA ','
#define SEMICOLON ';'

enum Success { True = 0, False = -1 };
typedef enum Success Success;

struct SClass {
	int classNumber;
	char className[CLASS_NAME_LEN];
	struct SList* head;
};
typedef struct SClass Class_t;

struct SStudent {
	char Name[STUDENT_NAME_LEN+1];
	int Grade;
	int classIndex;
};
typedef struct SStudent Student_t;

struct SList {
	struct SStudent student;
	struct SList* next;
};
typedef struct SList List_t;

void printMenu();
int addClass(Class_t classes[], int currentAmount);   
void addStudentToClass(Class_t classes[], int amount);        
void removeStudentFromClass(Class_t classes[], int amount);   
void printClass(Class_t classes[], int amount);               
void avgGrade(Class_t classes[], int amount);                 
void maxGrade(Class_t classes[], int amount);                 
void minGrade(Class_t classes[], int amount);                 
void sumOfGrades(Class_t classes[], int amount);              
void countOfGrades(Class_t classes[], int amount);            
void freeAll(Class_t classes[], int amount);

#endif // EX5_H