# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <ctype.h>

# define YEAR_BUFF_SIZE 16
# define NAME_BUFF_SIZE 20
# define STUDENT_ID_BUFF_SIZE 10
# define GPA_BUFF_SIZE 10

void driver();
void addStudent();
void modStudent();
void displayAll();
void removeStudent();
void displayStudent();
void quit();

// stores data about each student
typedef struct student {
    char name[NAME_BUFF_SIZE];
    int year;
    long studentID;
    double gpa;
} student;

// Dynamically allocates memory for student struct and returns a student struct pointer
student* createStudent (char* name, int year, double gpa, long studentID) {
    student* stu = (student*)malloc(sizeof(student));
    int index = 0;
    while (*name != '\0') {
        stu->name[index] = *name;
        index++;
        name++;
    }
    stu->name[index] = '\0';
    stu->year = year;
    stu->gpa = gpa;
    stu->studentID = studentID;
    return stu;
}

int main () {
    driver();
}

// Clears Input Buffer by finding next "\n" character
static void clearInput () {
    char arb;
    do {
        arb = getchar();
    } while (arb != '\n' && arb != EOF && arb != 0);
}

// main driver of the main menu for the program.
void driver () {
    int inputNum = 0;
    while (1) {
        printf("Welcome to Grade Management System!\n1. Add Student\n2. Modify Student\n3. Display All Grades\n4. Display Student Grades\n5. Remove Student\n6. Exit\n");
        scanf("%d", &inputNum);
        
        switch (inputNum) {
        case 1:
            addStudent();
            break;
        
        case 2:
            // TODO
            //modStudent();
            break;

        case 3:
            // TODO
            //displayAll();
            break;

        case 4:
            // TODO
            displayStudent();

        case 5:
            // TODO
            //removeStudent();
            break;

        case 6:
            quit();
            break;

        default:
            printf("Unkown Command\n");
            break;
        }
    }
}

// Inserts student data into database
void insertStudent (student* stu) {
    FILE *db = fopen("records.csv", "a");
    if (db == NULL) printf("Error Opening File");
    fprintf(db,"\n%s,%d,%f,%ld", stu->name, stu->year, stu->gpa, stu->studentID);
    fclose(db);
}

// Returns corresponding int for the string representation of grade level
int gradeToInt (char *yearPtr) {
    char freshman[] = "freshman";
    char sophmore[] = "sophomore";
    char junior[] = "junior";
    char senior[] = "senior";
    int year = -1;

    if (!strcmp(yearPtr, freshman)) {
        year = 0;
    }
    else if (!strcmp(yearPtr, sophmore)) {
        year = 1;
    }
    else if (!strcmp(yearPtr, junior)) {
        year = 2;
    }
    else if (!strcmp(yearPtr, senior)) {
        year = 3;
    }
    return year;
}

char* intToGrade (int num) {
    // TODO
    return NULL;
}

// TODO shorten to be ideally 35
// Prompts user and receives data for required student information and stores data as student struct
void addStudent () {
    char *namePtr = (char *)calloc(NAME_BUFF_SIZE, sizeof(char));
    char *yearPtr = (char *)calloc(YEAR_BUFF_SIZE, sizeof(char));
    int year, studentID = -1;
    char gpaStr[GPA_BUFF_SIZE];
    char idStr[STUDENT_ID_BUFF_SIZE];
    double gpa = -1;
    char *strEnd;

    memset(gpaStr, '\0', sizeof(gpaStr));
    memset(idStr, '\0', sizeof(idStr));
    errno = 0;

    // TODO -- Error Handling when input exceeds length of buffer
    // Scans in input for Name, Year, GPA, and Student ID
    printf("Name: ");
    clearInput();
    if (fgets(namePtr, NAME_BUFF_SIZE, stdin) == NULL) {
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error Inputing Name");
    }
    namePtr[strcspn(namePtr, "\n")] = 0;
    fflush(stdin);
    printf("Year: ");
    scanf("%16s", yearPtr);
    printf("GPA: ");
    clearInput();
    scanf("%5s", gpaStr);
    printf("Student ID: ");
    scanf("%10s", idStr);

    // converts gpaStr and idStr to float and int respectively
    gpa = strtof(gpaStr, &strEnd);
    studentID = strtol(idStr, &strEnd, STUDENT_ID_BUFF_SIZE);

    year = gradeToInt(yearPtr);
    if (year == -1) {
        printf("Please Enter a Valid year (freshman, sophomore, junior, senior)\n");
        free(namePtr);
        free(yearPtr);
        addStudent();
        return;
    }

    // Creates student struct and inserts struct data into database
    student* stu = (student*)malloc(sizeof(student));
    stu = createStudent(namePtr, year, gpa, studentID);
    printf("Inserting student: %s, %d, %f, %ld\n", stu->name, stu->year, stu->gpa, stu->studentID);
    insertStudent(stu);

    free(namePtr);
    free(yearPtr);
    free(stu);
    return;
}

student* getStudentByName (char *name) {
    // TODO
    printf("Search By Name Not Currently Supported\n");
    exit(0);
    return NULL;
}

// TODO
student* getStudentByID (long ID) {
    // TODO
    const char delim[2] = ",";
    char *token;
    char *line = NULL;
    size_t len = 0;
    char *name = calloc(NAME_BUFF_SIZE, sizeof(char));
    int grade;
    double gpa;
    long studentID;
    int read = 0;
    int foundFlag = 0;
    student *stu = NULL;

    //printf("Into Student ID: %ld", ID);
    fflush(stdout);
    FILE *db = fopen("records.csv", "r");
    if (db == NULL) {
        printf("Error Opening File");
        exit(EXIT_FAILURE);
    }

    while (read = getline(&line, &len, db) != -1) {
        //printf("Scanning Line: %s", line);
        token = strtok(line, delim);
        for (int i = 0; i < 4 && token != NULL; i++) {
            if (i == 0) name = token;
            if (i == 1) grade = atoi(token);
            if (i == 2) gpa = atof(token);
            if (i == 3) studentID = atol(token);
            token = strtok(NULL, delim);
        }
        if (studentID == ID) {
            foundFlag = 1;
            break;
        }
    }
    if (foundFlag != 1) {
        printf("Error: Could not find student\n");
        exit(1);
        return stu;
    }
    stu = createStudent(name, grade, gpa, studentID);
    return stu; // stu will need to freed in displayStudent()
}

// Displays the data for the given student by name or ID
void displayStudent () {
    // TODO
    char *input = calloc(NAME_BUFF_SIZE, sizeof(char));
    char *gradeString;
    student *stu;

    printf("Name or ID: ");
    clearInput();
    if (fgets(input, NAME_BUFF_SIZE, stdin) == NULL) {
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error Inputing Name or ID");
    }
    
    // TODO Bounds Checking on input
    if (isdigit(*input)) {
        stu = getStudentByID(atol(input));
    }

    else {
        stu = getStudentByName(input);
    }

    gradeString = intToGrade(stu->year);
    printf("Name: %s\nGrade: %d\nGPA: %f\nStudent ID: %ld\n", stu->name, stu->year, stu->gpa, stu->studentID);
    return;
}

// Quits the program
void quit () {
    printf("Exiting...\n");
    exit(0);
}