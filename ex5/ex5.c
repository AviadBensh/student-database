/******************
* Aviad Benshoshan
* 318528874
* 01
* ex5
*******************/
#define _CRT_SECURE_NO_WARNINGS
#include"ex5.h"

int validateInputAddClass(char* input);
int validateClassNum(char* input);
int validateName(char* input, int currIndex, char endChar, int maxLen);
int existingClass(Class_t classes[], int classNumber, int amount);
void updateClassName(Class_t* class, char* newName);
void newClass(Class_t* class, char* name, int classNumber);
void assignStudent(Class_t classes[], int amount, char* input);
int existingStudent(Class_t classes[], int amount, char* name, int classNum, int newGrade);
void swap(List_t* a, List_t* b);
void bubbleSort(List_t* first);
void createNode(Class_t classes[], int amount, char* name, int classNum, int grade);
void addNodeToList(Class_t* classes, List_t* newNode);
List_t* allocNode(Class_t classes[], int amount);
int validateInputAddStudent(char* input, Class_t classes[], int amount);
int checkNameClassGradeValid(char* input, Class_t classes[], int amount);
int validateGrade(char* tempGrade);
int checkPuncMarks(char* input);
char* removeSpaces(char string[]);
void validateRemovalInput(char* input, Class_t classes[], int amount);
int checkNameAndClassValid(char* input, Class_t classes[], int amount);
int allStudentClasses(Class_t classes[], int amount, char* name);
int isInAllRequestedClasses(Class_t classes[], int amount, int index, int classNum[], char* name);
void removeStudent(Class_t classes[], int amount, int classNum[], int index, char* name);
int CommaCount(char* input, int amount);

/**********************************************************************************************************
* Function Name: printMenu
* Input: None
* Output: None
* Function Operation: prints main menu
***********************************************************************************************************/
void printMenu() {
	puts("Please select an operation:\n\t0. Exit.\n\t1. Insert or update a class.\n\t2. Insert or update a student.\n"
		"\t3. Remove a student from classes.\n\t4. Print a class.\n\t5. Print all classes with their computed scores."
		"\n\t6. Print the menu.");
	return;
}

/*************************************************************************************************************
* Function Name: addClass
* Input:Class_t classes[] (array of class structures, int currentAmount (amount of classes currently in array.
* Output: True/False
* Function Operation:  the function receives input from user.
*                      checks if class exists already and if so, updates the class name.
*                      if class doesn't exist and the Max amount of classes(5) has not been reached yet, the new
                           class is added.
**************************************************************************************************************/
int addClass(Class_t classes[], int currentAmount) {
	//'userInput' is pointer to string that the user input will go to
	char userInput[MAX_USER_INPUT];
	//existingClassIndex is the return value from 'existingClass' function. False/0-4 accordingly.
	int existingClassIndex = 0;
	scanf(" %[^\n]", &userInput);

	/*********************************************************
	if input string is not valid, an error message is printed
	**********************************************************/
	if (validateInputAddClass(userInput) == False) {
		printf("Error: invalid class number or name.\n");
		return False;
	}
	//strtok function breaks input at the first SPACE char and inserts as int (atoi) to 'classNumber'
	int classNumber = atoi(strtok(userInput, " "));
	//the remainig string after initial break is the className
	char* className = strtok(NULL, "\0");
	//removeSpaces cleans any spaces at beginning of string.
	className = removeSpaces(className);

	/************************************************************************************************
	existingClass function returns the index in 'classes' aray of the class number the user inputted. 
	if class exists, function returns int between 0-4.and send new name to updating function.
	if class does not exist, function returns False and continues in current function
	*************************************************************************************************/
	existingClassIndex = existingClass(classes, classNumber, currentAmount);
	if (existingClassIndex != False) {
		updateClassName(&classes[existingClassIndex], className);
		return False;
	}

	/****************************************************************************************
	if class does not exist and array of classes has reached max, an error message is printed
	*****************************************************************************************/
	if (currentAmount == MAX_CLASSES) {
		printf("Unfortunately, there is no room for more classes.\n");
		return False;
	}
	
	/**********************************************************************************************************
	if class does not exist and there is room in array of classes, 'newClass' function, adds new class to array
	***********************************************************************************************************/
	newClass(&classes[currentAmount], className, classNumber);
	return True;
}

/**********************************************************************************************************
* Function Name: validateInputAddClass
* Input: char* input (userInput string)
* Output: True/False if string is valid or not
* Function Operation: send to two seperate function to validate number and name 
***********************************************************************************************************/
int validateInputAddClass(char* input) {
	//index of iteration through string
	int index = 0;
	//function return index it reached at end of number check to 'index'
	index = validateClassNum(input);
	
	/************************************************************************************************
	if function returned False, meaning number is not valid, funtion returns False to caller function
	*************************************************************************************************/
	if (index == False) {
		return False;
	}

	/******************************************************************************************************************
	if number is valid, but validateName func returned False, meaning the name isn't valid, False is returned to caller
	******************************************************************************************************************/
	else if (validateName(input, index, END_OF_STRING, CLASS_NAME_LEN) == False) {
		return False;
	}
	//if number and name validating functions did not return False, they are valid, and True is returned to caller
	return True;
}

/**********************************************************************************************************
* Function Name: validateClassNum
* Input:char* input ('userInput' string)
* Output: False/index of string reached at end of iteration
* Function Operation: validates string if it answers all 'number of class' criterias.
***********************************************************************************************************/
int validateClassNum(char* input) {
	//index of iteration
	int index = 0;
	//input returns without leading SPACE chars if they exist.
	input = removeSpaces(input);

	/***********************************************************************************
	while loop iterates through string until it reaches SPACE char or end of string(\0)
	if through iteration, a non-digit char is reached, False is returned to caller
	***********************************************************************************/
	while (input[index] != SPACE && input[index] != END_OF_STRING) {
		if (isdigit(input[index]) == 0) {
			return False;
		}
		index++;
	}

	/************************************************************************************************************
	if index at the end of while loop is not equal to amount of chars required for class number, False is reurned
	************************************************************************************************************/
	if (index != CLASS_NUM_LEN) {
		return False;
	}
	//if index reached required amount of chars for class number, and they are all digits, the index is returned
	return index;
}

/*****************************************************************************************************************
* Function Name:validateName
* Input:char* input (userInput string), int currIndex (index reached at end of last iteration),
       char endChar (char to stop iteration), int maxLen (max length allowed for name, 30 for class/20 for student
* Output:True/False if valid or not
* Function Operation: iterates through given string and check if it answers all 'name' criterias.
******************************************************************************************************************/
int validateName(char* input, int currIndex, char endChar, int maxLen) {
	//starting index of iteration becomes last index reached in last function 
	int startIndex = currIndex;

	/***************************************************************************
	while loop checks each char if valid until deifned end of iteration reached 
	****************************************************************************/
	while (input[currIndex] != endChar) {

		/***********************************************************************
		if char does not fit name char requirements, False is returned to caller
		***********************************************************************/
		if (input[currIndex] != SPACE && isdigit(input[currIndex]) == 0 && isalpha(input[currIndex]) == 0) {
			return False;
		}
		currIndex++;
	}

	/*****************************************************************************************************************
	if between starting index and index at end of iteration there are more the allowed number of chars, False returned
	*****************************************************************************************************************/
	if ((currIndex - startIndex) > maxLen) {
		return False;
	}
	//if name is valid, True is returned
	return True;
}

/**********************************************************************************************************
* Function Name:existingClass
* Input: Class_t classes[] (array of classes), int classNumber(class number the user inputted), 
         int amount (amount of classes currently in array)
* Output: False/index, false if class does not exist, index of class in array if exists
* Function Operation: runs through classes array to check if inputted class num exists
***********************************************************************************************************/
int existingClass(Class_t classes[], int classNumber, int amount) {

	/**************************
	loop through classes array
	**************************/
	for (int index = 0; index < amount; index++) {

		/***********************************************************************************************
		if class number of input and class number in array, return index of the class in array to caller
		************************************************************************************************/
		if (classNumber == classes[index].classNumber) {
			return index;
		}
	}
	//if class does not exist in array, return False
	return False;
}

/******************************************************************************************************************
* Function Name: updateClassName
* Input:Class_t* class (class at index matching the inputted class number), char* newName (class name in userInput)
* Output: void
* Function Operation: copy new name to 'className' field in the class structure and print update message
******************************************************************************************************************/
void updateClassName(Class_t* class, char* newName) {
	strcpy(class->className, newName);
	printf("Class \"%d %s\" updated.\n", class->classNumber, class->className);
	return;
}

/*************************************************************************************************************
* Function Name: newClass
* Input: Class_t* class (class in first empty index in classes array), char* name (class name from userInput),
         int classNumber (class number from userInput)
* Output:void
* Function Operation: inserts new number and name in empty index in classes array and prints adding message 
**************************************************************************************************************/
void newClass(Class_t* class, char* name, int classNumber) {
	class->classNumber = classNumber;
	//class->className = name;
	strcpy(class->className, name);
	class->head = NULL;
	printf("Class \"%d %s\" added.\n", class->classNumber, class->className);
}

/**********************************************************************************************************
* Function Name: addStudentToClass
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array)
* Output: void
* Function Operation: receives input from user, validates input, and if valid, assigns student to classes
***********************************************************************************************************/
void addStudentToClass(Class_t classes[], int amount) {
	char userInput[MAX_USER_INPUT];
	scanf(" %[^\n]", &userInput);
	char tempInput[MAX_USER_INPUT];
	strcpy(tempInput, userInput);
	//allValid recieves True/False if string os valid or not, if false, end of function
	int allValid= validateInputAddStudent(tempInput, classes, amount);
	if (allValid == False) {
		return;
	}

	/*****************************************************************
	if string is valid, enter 'assignStudent' function to take action
	******************************************************************/
	else if (allValid == True) {
		assignStudent(classes, amount, userInput);
	}
	return;
}

/*********************************************************************************************************************
* Function Name: assignStudent
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array), char* input (userInput)
* Output: void
* Function Operation: breaks input to pieces (name, array of class number, and array of grade), checks if student 
                      exists in class and updates grade. if not, creates new student.
*********************************************************************************************************************/
void assignStudent(Class_t classes[], int amount, char* input) {
	//index variable is for array that holds classNum and grades.
	int index = 0;
	char* studentName = strtok(input, ":");
	//nameCheck recieves True/False from validateName function if student name is valid or not
	int nameCheck = validateName(input, index, END_OF_STRING, STUDENT_NAME_LEN);
	if (nameCheck == False) {
		return;
	}
	//allClassAndGrade is string of all input after the student name
	char* allClassAndGrade = strtok(NULL, "\0");
	//classAndGrade is array of strings that hold each class number and grade in seperate index
	char* classAndGrade[MAX_CLASSES];
	char* token = strtok(allClassAndGrade, ";");

	/************************************************************
	while loop inserting class number and grade into eache index
	*************************************************************/
	while (token != NULL) {
		classAndGrade[index] = token;
		token = strtok(NULL, ";");
		index++;
	}
	/********************************************************************************************
	for loop breaking each index in 'classAndGrade' array to class number string and grade string
	********************************************************************************************/
	for (int j = 0; j < index; j++) {
		char* tempClassNumber = strtok(classAndGrade[j], ",");
		char* tempStudentGrade = strtok(NULL, "\0");
		//converting class number to int
		int classNum = atoi(tempClassNumber);
		//converting grade to int
		int studentGrade = atoi(tempStudentGrade);
		//isStudent receives True/False if student exists already in this class
		int isStudent = existingStudent(classes, amount, studentName, classNum, studentGrade);

		/************************************************************
		if student is not in class, enter 'createStudent' to add him
		*************************************************************/
		if (isStudent == False) {
			createNode(classes, amount, studentName, classNum, studentGrade);
		}
	}
}

/*************************************************************************************************************
* Function Name: existingStudent
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array),
         char* name (name of student), int classNum (class Number), int newGrade (student's grade)
* Output: True/False if student exists in class or not.
* Function Operation: iterates through classes array, when matching class number reached, checks is there 
                      is same student in class. if yes, his grade is updated and a message is printed and True
					  is returned. if he doesn't exist, False is returned
**************************************************************************************************************/
int existingStudent(Class_t classes[], int amount, char* name, int classNum, int newGrade) {
	
	/**************************
	loop through classes array
	**************************/
	for (int i = 0; i < amount; i++) {

		/*******************************************************************************
		if inputted class number is equal to class number in this index in classes array
		********************************************************************************/
		if (classes[i].classNumber == classNum) {

			/********************************************************************************
			if head node in class is not empty, make temp node and iterate through list until
			temp reaches NULL or reaches the student you were searching for.
			********************************************************************************/
			if (classes[i].head != NULL) {
				//temp variable to make iterating through list without making changes to list possible
				List_t* temp = classes[i].head;

				/********************************************************************************
				while loop checking each round if student name of node is same as current student 
				*********************************************************************************/
				while (temp != NULL) {

					/**************************************************************************
					if student exists, update grade and print message, return True to caller
					**************************************************************************/
					if (strcmp(temp->student.Name, name) == True) {
						temp->student.Grade = newGrade;
						printf("Student \"%s\" updated on class \"%d %s\" with grade %d.\n",
							temp->student.Name, classes[i].classNumber, classes[i].className, temp->student.Grade);
						return True;
					}
					temp = temp->next;
				}
			}
		}
	}
	// if through all iteration, student was not found, return False
	return  False;
}

/**********************************************************************************************************
* Function Name: swap
* Input: List_t* a, List_t* b (two List_t nodes)
* Output: void
* Function Operation: swaps the student data between to nodes.
***********************************************************************************************************/
void swap(List_t* a, List_t* b) {
	Student_t temp = a->student;
	a->student = b->student;
	b->student = temp;
}

/**********************************************************************************************************
* Function Name: bubbleSort
* Input: List_t* first (first node in list)
* Output: void
* Function Operation: organizes list in lexographic order.
***********************************************************************************************************/
void bubbleSort(List_t* first) {
	int swapped;
	List_t* temp = first;
	List_t* last = NULL;
	do
	{
		swapped = 0;
		while (temp != last && temp->next != NULL) {
			if (strcmp(temp->student.Name, temp->next->student.Name) > 0) {
				swap(temp, temp->next);
				swapped = 1;
			}
			temp = temp->next;
		}
		last = temp;
	} while (swapped);
}

/************************************************************************************************************
* Function Name: createNode
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array), 
         char* name (name of student), int classNum, int grade
* Output: void
* Function Operation: creates new node of student and inserts it to head of list. then bubble sorts the list
*************************************************************************************************************/
void createNode(Class_t classes[], int amount, char* name, int classNum, int grade) {
	// new node is pointer to new dynamically allocated node
	List_t* newNode = allocNode(classes, amount);
	//copy student name to name field in new node
	strcpy(newNode->student.Name, name);
	//insert grade to grade field in new node
	newNode->student.Grade = grade;
	//initialize pointer to next node field in new node to NULL
	newNode->next = NULL;

	/*******************************************************************************************************
	loop through classes array until reach class number matching inputted class number, then insert new node
	********************************************************************************************************/
	for (int i = 0; i < amount; i++) {

		/**************************************************************************************
		if class number matches inputted class number add node to list and bubble sort the list
		***************************************************************************************/
		if (classNum == classes[i].classNumber) {
			addNodeToList(&classes[i], newNode);
			bubbleSort(classes[i].head);
		}
	}
	return;
}

/**********************************************************************************************************
* Function Name: addNodeToList
* Input: Class_t classes[] (classes array), List_t* newNode (pointer to the new node of student)
* Output: void
* Function Operation: inserts new student node to head of list and prints message
***********************************************************************************************************/
void addNodeToList(Class_t* classes, List_t* newNode) {

	/******************************************************
	if head node is empty, insert new node to head pointer
	*******************************************************/
	if (classes->head == NULL) {
	classes->head = newNode;
	}

	/*************************************************************************
	if there is node already in head pointer, point next field in new node to 
	current head and then head pointer to new node and print message
	**************************************************************************/
	else {
		newNode->next = classes->head;
		classes->head = newNode;
	}
	printf("Student \"%s\" added to class \"%d %s\" with grade %d.\n",
		classes->head->student.Name, classes->classNumber, classes->className, classes->head->student.Grade);
	return;
}

/**********************************************************************************************************
* Function Name: allocNode
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array)
* Output: node (a dynamically allocated node)
* Function Operation: dynamically allocate a node
***********************************************************************************************************/
List_t* allocNode(Class_t classes[], int amount) {
		List_t* node = (List_t*)calloc(SINGLE, sizeof(List_t));
		if (node == NULL) {
			freeAll(classes, amount);
			exit(1);
		}
		return node;
	}

/************************************************************************************************************
* Function Name: validateInputAddStudent
* Input: char* input (userInput), classes[] (classes array), int amount (amount of classes currently in array)
* Output: True/False if input is valid
* Function Operation: checks validation of input, punctuation marks and student name/class num/grade,
                      if invalid, prints error message.
*************************************************************************************************************/
int validateInputAddStudent(char* input, Class_t classes[], int amount) {
	//variable receiving amount of grade from 'checkPuncMarks' func
	int gradeAmount = 0;
	gradeAmount = checkPuncMarks(input);

	/*****************************************************************************************************
	if grade amount is False (meaning no grades inserted) or more grades than classes, print error message
	*****************************************************************************************************/
	if (gradeAmount == False || gradeAmount > MAX_CLASSES) {
		printf("Error: invalid name, class number or grade.\n");
		return False;
	}
	//fieldValid receives True/False if fields of input are valid
	int fieldValid = checkNameClassGradeValid(input, classes, amount);

	/**********************************************
	if fields are not valid, print error message
    ***********************************************/
	if (fieldValid == False) {
		printf("Error: invalid name, class number or grade.\n");
		return False;
	}
	else {
		return True;
	}
}

/**********************************************************************************************************
* Function Name: checkNameClassGradeValid
* Input: char* input, classes[] (classes array), int amount (amount of classes currently in array)
* Output: True/False if valid or not
* Function Operation: checks validity of student name/ class numbers/ grades.
                      if invalid, returns False. if valid, returns True
***********************************************************************************************************/
int checkNameClassGradeValid(char* input, Class_t classes[], int amount) {
	//index for classAndGrade array
	int index = 0;
	char* studentName = strtok(input, ":");
	//student name without leading spaces
	studentName - removeSpaces(studentName);
	//nameCheck receives True/False if name is valid
	int nameCheck = validateName(studentName, index, END_OF_STRING, STUDENT_NAME_LEN);

	/*****************************************
	if name not valid return False to caller
	*****************************************/
	if (nameCheck == False) {
		return False;
    }

	/************************************************
	make array which at each index is class and grade
	*************************************************/
	char* allClassAndGrade = strtok(NULL, "\0");
	char* classAndGrade[MAX_CLASSES];
	char* token = strtok(allClassAndGrade, ";");
	while (token != NULL) {
		classAndGrade[index] = token;
		token = strtok(NULL, ";");
		index++;
	}

	/****************************************************************************************************
	for loop iterating through classAndGrade array and seperating class and grade to different variables
	the sending class num to 'existingClass' function to check if class exists in classes array
	****************************************************************************************************/
	for (int j = 0; j < index; j++) {
		char* tempClassNumber = strtok(classAndGrade[j], ",");
		char* tempStudentGrade = strtok(NULL, "\0");
		// check grade validity
		int gradeValid = validateGrade(tempStudentGrade);
		// check class number validity
		int classNumValid = validateClassNum(tempClassNumber);
		// convert class number to int
		int classNum = atoi(tempClassNumber);
		int isClass = existingClass(classes, classNum, amount);

		/********************************************************************************************
		if class number or grade are not valid, or class does not exist, return False to caller
		********************************************************************************************/
		if (classNumValid == False ||gradeValid == False || isClass == False) {
			return False;
		}
	}
	return True;
}

/**********************************************************************************************************
* Function Name: validateGrade
* Input: char* tempGrade (string of grade)
* Output: True/False if valid or not
* Function Operation: checks if string is right length, al chars are digits and does not exceed 100
***********************************************************************************************************/
int validateGrade(char* tempGrade) {
	//index for chars in string of tempGrade
	int index = 0;
	// cleaning string from leading spaces
	tempGrade = removeSpaces(tempGrade);
	
	/****************************************************
	if string length is smaller than 4, enter while loop 
	*****************************************************/
	if (strlen(tempGrade) < THOUSAND_DEC) {

		/************************************************************************************
		while char of string doesn't reach defined end of string char, check if char is digit
		************************************************************************************/
		while (tempGrade[index] != END_OF_STRING) {

			/********************************************
			if char is not digit, return false to caller
			*********************************************/
			if (isdigit(tempGrade[index]) == 0) {
				return False;
			}
			index++;
		}

		/********************************************************************
		if grade is between 0 to 100, and passed while loop check, return True
		*********************************************************************/
		if (atoi(tempGrade) >= 0 && atoi(tempGrade) <= 100) {
			return True;
		}
	}
	return False;
}

/************************************************************************************************************
* Function Name: checkPuncMarks
* Input: char* input (input string)
* Output: True/False if valid
* Function Operation: counts number of colons, commas and semicolons, counts each comma as a grade,
                      if all punctuation marks adhere to requirments, gradeAmount is returned. if not, False
*************************************************************************************************************/
int checkPuncMarks(char* input) {
	
	/****************************************************************************************
	counter increments by 1 each comma and decrements by 1 each semicolon, making sure no 
	commas and semicolons are repeated one after the other
	*****************************************************************************************/
	int colonCounter = 0, counter=0, gradeAmount=0;

	/**************************************************
	for loop through string counting punctuation marks
	***************************************************/
	for (int i = 0; i < strlen(input); i++) {
		if (input[i] == COLON) {
			colonCounter++;
		}
		else if (input[i] == COMMA) {
			counter++;
			gradeAmount++;
		}
		else if (input[i] == SEMICOLON) {
			counter--;
		}

		/*****************************************************
		if counter is less than 0 or more than 1, return False
		*****************************************************/
		if (counter < 0 || counter>SINGLE) {
			return False;
		}
	}

	/**********************************
	if more than 1 colon, return False
	************************************/
	if (colonCounter != SINGLE) {
		return False;
	}
	else
		return gradeAmount;
}

/**********************************************************************************************************
* Function Name: removeSpaces
* Input: char string[] (any string)
* Output: string (without leading spaces)
* Function Operation: cleans all spaces at beginning of string
***********************************************************************************************************/
char* removeSpaces(char string[]) {
	//start index of string
	int start = 0;
	if (string == NULL) {
		return string;
	}
	/*********************************************
	while loop as long as beginning char is SPACE
	**********************************************/
	while (string[start] == ' ') {

		/**********************************
		make current char same as next char
		***********************************/
		for (int i = 0; i < strlen(string); i++) {
			string[i] = string[i + 1];
		}
	}
	return string;
}

/**********************************************************************************************************
* Function Name: removeStudentFromClass
* Input: classes[] (classes array), int amount (amount of classes currently in array)
* Output: void
* Function Operation: recieves input from user and sends to validation function
***********************************************************************************************************/
void removeStudentFromClass(Class_t classes[], int amount) {
	char userInput[MAX_USER_INPUT]; 
	scanf(" %[^\n]", &userInput);
	//send input to function to check validation and take action
	validateRemovalInput(userInput, classes, amount);
	return;
}

/**********************************************************************************************************
* Function Name: validateRemovalInput
* Input: char* input, Class_t classes[] (classes array), int amount (amount of classes currently in array)
* Output: void
* Function Operation: send input to validation function. if False is recieved an error mesage is printed
***********************************************************************************************************/
void validateRemovalInput(char* input, Class_t classes[], int amount) {
	
	/***********************************************************************************************
	gradeAmount receives amount of commas, which is amount of grades. if receives False, print error
	***********************************************************************************************/
	int gradeAmount = CommaCount(input, amount);
	if (gradeAmount == False) {
		printf("Error: invalid name or class number.\n");
		return;
	}

	/*****************************************************************************
	fieldValid receives True\False if fields are valid. if not valid, print error
	******************************************************************************/
	int fieldValid = checkNameAndClassValid(input, classes, amount);
	if (fieldValid == False) {
		printf("Error: invalid name or class number.\n");
		return;
	}
	return;
}

/**********************************************************************************************************
* Function Name: checkNameAndClassValid
* Input: char* input, Class_t classes[] (classes array), int amount (amount of classes currently in array)
* Output: True/False if valid or not
* Function Operation:
***********************************************************************************************************/
int checkNameAndClassValid(char* input, Class_t classes[], int amount) {
	//index for classAndGrade array
	int index = 0;
	char* studentName = strtok(input, ":");
	//nameCheck receives True/False if name is valid
	int nameCheck = validateName(input, index, END_OF_STRING, STUDENT_NAME_LEN);

	/*****************************************
if name not valid return False to caller
*****************************************/
	if (nameCheck == False) {
		return False;
	}

	/************************************************
	 make array which at each index is class number
	*************************************************/
	char* classNumCharArr[MAX_CLASSES];
	char* token = strtok(NULL, ",");
	while (token != NULL) {
		classNumCharArr[index] = token;
		token = strtok(NULL, ",");
		index++;
	}
	/************************************************************************************
	initialize int array of class number and convert each string of class number to int 
	*************************************************************************************/
	int classNum[MAX_CLASSES];
	for (int i = 0; i < index; i++) {
		classNum[i] = atoi(classNumCharArr[i]);
	}
	//check if for all classes inputted the student is actually in them
	int inAllClasses = isInAllRequestedClasses(classes, amount, index, classNum, studentName);

	/*******************************************************
	if student is not in all classes return false to caller
	********************************************************/
	if (inAllClasses == False) {
		return True;
	}

	/***********************************************************************************************
	check what are all the classes the student is in. if number of classes wanted to remove him from
	equals number of classes he is in, return false. print message, not allowed to remove from all
	************************************************************************************************/
	int totalStudentClasses = allStudentClasses(classes, amount, studentName);
	if (index == totalStudentClasses) {
		printf("Error: student cannot be removed from all classes.\n");
		return True;
	}
	//if passed all previous test, enter removal function
	removeStudent(classes, amount, classNum,index, studentName);
	return True;
}

/**********************************************************************************************************
* Function Name: allStudentClasses
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array), char* name
* Output:
* Function Operation:
***********************************************************************************************************/
int allStudentClasses(Class_t classes[], int amount, char* name) {
	
	/*************************************************************************************
	counter for amount of classes student is in. increments in for loop iterating through 
	classes array, and each class' list of students, if student is in there
	*************************************************************************************/
	int counter = 0;
	for (int i = 0; i < amount; i++) {
		List_t* temp = classes[i].head;
		while (temp != NULL) {
			if (strcmp(temp->student.Name, name) == True) {
				counter++;
				break;
			}
			temp = temp->next;
		}
	}
	return counter;
}

/**********************************************************************************************************
* Function Name:  isInAllRequestedClasses
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array), int index
         int classNum[], char* name
* Output: True/False if student is in all requested classes to remove him from
* Function Operation:
***********************************************************************************************************/
int isInAllRequestedClasses(Class_t classes[], int amount, int index, int classNum[], char* name) {

	/**************************************************************
	for loop through class number array and through classes array
	***************************************************************/
	for (int i = 0; i < index; i++) {
		for (int j = 0; j < amount; j++) {

			/*******************************************************************************************
			if class num in both arrays are same, check if student name is in student list of same class
			*******************************************************************************************/
			if (classNum[i] == classes[j].classNumber) {
				List_t* temp = classes[j].head;
				if (temp != NULL && strcmp(temp->student.Name, name) == 0) {
					break;
				}
				while (temp != NULL && strcmp(temp->student.Name, name) != 0) {
					temp = temp->next;
				}

				/***************************************************************************************
				if ater iterating through all list, student was not there, error message is printed
				****************************************************************************************/
				if (temp == NULL) {
					printf("Error: student is not a member of class \"%d %s\"\n", classes[j].classNumber, classes[j].className);
					return False;
				}
			}
		}
	}
	return True;
}

/**********************************************************************************************************
* Function Name: removeStudent
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array), int clasNum[],
         int index, char* name
* Output: void
* Function Operation:
***********************************************************************************************************/
void removeStudent(Class_t classes[], int amount, int classNum[],int index, char* name) {

	/*******************************************************
	for loops through classes array and class number array
	********************************************************/
	for (int i = 0; i < amount; i++) {
		for (int j = 0; j < index; j++) {

			/************************************************************************************************
			if class numbers from both arrays match, create temp node equal to head node in class, and pointer
			to previous node when student is reached previous pointer to node points to the student's next
			node, then student data is released and message is printed
			************************************************************************************************/
			if (classes[i].classNumber == classNum[j]) {
				List_t* temp = classes[i].head, * prev=NULL;
				if (temp == NULL) {
					return;
				}
				if (temp != NULL && strcmp(temp->student.Name, name) == 0) {
					printf("Student \"%s\" removed from class \"%d %s\".\n",
						temp->student.Name, classes[i].classNumber, classes[i].className);
					classes[i].head = temp->next;
					free(temp);
					continue;
				}
				while (temp != NULL && strcmp(temp->student.Name, name) != 0) {
					prev = temp;
					temp = temp->next;
				}
				printf("Student \"%s\" removed from class \"%d %s\".\n",
					temp->student.Name, classes[i].classNumber, classes[i].className);
				prev->next = temp->next;
				free(temp);
			}
		}
	}
	return;
}

/**********************************************************************************************************
* Function Name: CommaCount
* Input: char* input (string), int amount (amount of classes in classes array)
* Output: commaCount (equal to amount of grades), or False if punctuation marks are not valid
* Function Operation: iterate through string, and count necessary punctuation marks. check validity.
***********************************************************************************************************/
int CommaCount(char* input, int amount) {
	//index of string char
	int index = 0, commaCount=0, colonCount = 0;

	/***********************************
	while loop iterating through string
	************************************/
	while (input[index] != END_OF_STRING) {
		if (input[index] == COMMA) {
			commaCount++;
		}
		if (input[index] == COLON) {
			colonCount++;
		}
		index++;
	}

	/******************************************************************************************************
	if punctuation marks are not valid, or not enough (at least 1) or too many(more than existing classes),
	return False. else- return commaCount (amount of grades)
	******************************************************************************************************/
	if (colonCount != SINGLE || commaCount<SINGLE || commaCount>amount) {
		return False;
	}
	return commaCount;
}

/**********************************************************************************************************
* Function Name: printClass
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array)
* Output: void
* Function Operation: receive class number and print al of its' students with their grades
***********************************************************************************************************/
void printClass(Class_t classes[], int amount) {
	//res variable is result of existingClass function, numValid is positive number/False if class number is valid
	int res, numValid;
	char userInput[MAX_USER_INPUT];
	scanf(" %[^\n]", &userInput);
	numValid = validateClassNum(userInput);

	/********************************************************************
	if class number is valid, convert to int, and check if class exists
	********************************************************************/
	if (numValid != False) {
		int classNum = atoi(userInput);
		res = existingClass(classes, classNum, amount);

		/************************************************************************************
		if class exists and pointer to head of student list is NULL- print matching message,
		if class exists and head is not NULL, make temp pointer equal to head pointer and 
		print current temp, then move temp to be temp->next, and so on, until temp is NULL
		*************************************************************************************/
		if (res != False) {
			if (classes[res].head == NULL) {
				printf("Class \"%d %s\" has no students.\n", classes[res].classNumber, classes[res].className);
				return;
			}
			else {
				List_t* temp = classes[res].head;
				printf("Class \"%d %s\" students:\n", classes[res].classNumber, classes[res].className);
				while (temp != NULL) {
					printf("%s, %d\n", temp->student.Name, temp->student.Grade);
					temp = temp->next;
				}
				return;
			}
		}
	}

	/*******************************************
	if class does not exist print error message
	*******************************************/
	else {
		printf("Error: invalid class number.\n");
		return;
	}
}

/**********************************************************************************************************
* Function Name: avgGrade
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array)
* Output: void
* Function Operation: calculate average grade in each class and print
***********************************************************************************************************/
void avgGrade(Class_t classes[], int amount) {
	//sum of all grades in class, and count of amount of grades
	int sum, count;

	/*************************************************************
	loop through classes array, for each class calculating average
	*************************************************************/
	for (int i = 0; i < amount; i++) {
		//reinitialize sum and count for each class
		sum = 0;
		count = 0;

		/**********************************************
		if there are no students in class print message
		***********************************************/
		if (classes[i].head == NULL) {
			printf("Class \"%d %s\" has no students.\n", classes[i].classNumber, classes[i].className);
		}

		/****************************************************************
		sum all grades of student and count each grade, and print average
		****************************************************************/
		else {
			List_t* temp = classes[i].head;
			while (temp != NULL) {
				sum += temp->student.Grade;
				count++;
				temp = temp->next;
			}
			printf("%d %s, %d\n", classes[i].classNumber, classes[i].className, (sum / count));
			
		}
	}
}

/**********************************************************************************************************
* Function Name: maxGrade
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array)
* Output: void
* Function Operation: find max grade in each class and print
***********************************************************************************************************/
void maxGrade(Class_t classes[], int amount) {
	//max variable will be equal to max grade
	int max = 0;

	/*************************************************************
	loop through classes array, for each class finding max grade
	*************************************************************/
	for (int i = 0; i < amount; i++) {
		if (classes[i].head == NULL) {

			/**********************************************
		     if there are no students in class print message
		    ***********************************************/
			printf("Class \"%d %s\" has no students.\n", classes[i].classNumber, classes[i].className);
		}

		/****************************************************************
		find max grade between all student in class, and print max grade
		****************************************************************/
		else {
			List_t* temp = classes[i].head;
			max = temp->student.Grade;
			while (temp->next != NULL) {
				if (max < temp->next->student.Grade) {
					max = temp->next->student.Grade;
				}
				temp = temp->next;
			}
			printf("%d %s, %d\n", classes[i].classNumber, classes[i].className, max);
		}
	}
}

/**********************************************************************************************************
* Function Name: minGrade
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array)
* Output: void
* Function Operation: find minimal grade in each class and print
***********************************************************************************************************/
void minGrade(Class_t classes[], int amount) {
	int min = 0;

	/***************************************************************
	loop through classes array, for each class finding minimum grade
	***************************************************************/
	for (int i = 0; i < amount; i++) {
		if (classes[i].head == NULL) {

			/**********************************************
			 if there are no students in class print message
			***********************************************/
			printf("Class \"%d %s\" has no students.\n", classes[i].classNumber, classes[i].className);
		}

		/****************************************************************
		find min grade between all student in class, and print min grade
		****************************************************************/
		else {
			List_t* temp = classes[i].head;
			min = temp->student.Grade;
			while (temp->next != NULL) {
				if (min > temp->next->student.Grade) {
					min = temp->next->student.Grade;
				}
				temp = temp->next;
			}
			printf("%d %s, %d\n", classes[i].classNumber, classes[i].className, min);
		}
	}
}

/**********************************************************************************************************
* Function Name: sumOfGrades
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array)
* Output: void
* Function Operation: calculate sum of grades in a class and print for all classes
***********************************************************************************************************/
void sumOfGrades(Class_t classes[], int amount) {
	int sum;

	/*******************************************************************
	loop through classes array, for each class calculating sum of grades
	*******************************************************************/
	for (int i = 0; i < amount; i++) {
		sum = 0;
		if (classes[i].head == NULL) {

			/**********************************************
			 if there are no students in class print message
			***********************************************/
			printf("Class \"%d %s\" has no students.\n", classes[i].classNumber, classes[i].className);
		}
		else {

			/********************************************************
			add up grades of all students in class and print the sum
			*********************************************************/
			List_t* temp = classes[i].head;
			while (temp != NULL) {
				sum += temp->student.Grade;
				temp = temp->next;
			}
			printf("%d %s, %d\n", classes[i].classNumber, classes[i].className, sum);
		}
	}
}

/**********************************************************************************************************
* Function Name: countOfGrades
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array)
* Output: void
* Function Operation: count number of grade in each class and print. 
***********************************************************************************************************/
void countOfGrades(Class_t classes[], int amount) {
	int count;

	/*******************************************************************
	loop through classes array, for each class counting amount of grades
	*******************************************************************/
	for (int i = 0; i < amount; i++) {
		count = 0;
		if (classes[i].head == NULL) {

			/**********************************************
			 if there are no students in class print message
			***********************************************/
			printf("Class \"%d %s\" has no students.\n", classes[i].classNumber, classes[i].className);
		}
		else {

			/*********************************************************
			count grades of all students in class and print the amount
			**********************************************************/
			List_t* temp = classes[i].head;
			while (temp != NULL) {
				count++;
				temp = temp->next;
			}
			printf("%d %s, %d\n", classes[i].classNumber, classes[i].className, count);
		}
	}
}

/**********************************************************************************************************
* Function Name: freeAll
* Input: Class_t classes[] (classes array), int amount (amount of classes currently in array)
* Output: void
* Function Operation: free all dynamically allocoted memory in heap
***********************************************************************************************************/
void freeAll(Class_t classes[], int amount) {

	/***************************************************************
	loop through classes array, for each class freeing student list
	***************************************************************/
	for (int i = 0; i < amount; i++) {
		//initialize temp node 
		List_t* temp = NULL;

		/********************************************************************************************
		while there are still students in list, make temp equal to head, then make head equal to next
		then free temp. do that until temp is last student and head equals NULL. then free the class.
		********************************************************************************************/
		while (classes[i].head != NULL) {
			temp = classes[i].head;
			classes[i].head = classes[i].head->next;
			free(temp);
		}
	}
		free(classes);
}