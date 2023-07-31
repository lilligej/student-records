# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>

# define YEAR_BUFF_SIZE 16
# define NAME_BUFF_SIZE 20
# define STUDENT_ID_BUFF_SIZE 10
# define GPA_BUFF_SIZE 5

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
    int year, studentID;
    double gpa;
} student;

// Dynamically allocates memory for student struct and returns a student struct pointer
student* createStudent (char* name, int year, double gpa, int studentID) {
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
            //displayStudent();

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
    fprintf(db,"\n%s,%d,%f,%d", stu->name, stu->year, stu->gpa, stu->studentID);
    fclose(db);
}

// TODO shorten to be less than 60 lines, ideally 35
// Prompts user and receives data for required student information and stores data as student struct
void addStudent () {
    char freshman[] = "freshman";
    char sophmore[] = "sophomore";
    char junior[] = "junior";
    char senior[] = "senior";
    char *freshmanPtr = freshman;
    char *sophmorePtr = sophmore;
    char *juniorPtr = junior;
    char *seniorPtr = senior;

    char *namePtr = (char *)calloc(NAME_BUFF_SIZE, sizeof(char));
    char *yearPtr = (char *)calloc(YEAR_BUFF_SIZE, sizeof(char));
    int year, studentID = -1;
    char gpaStr[GPA_BUFF_SIZE];
    char idStr[STUDENT_ID_BUFF_SIZE];
    double gpa = -1;

    memset(gpaStr, '\0', sizeof(gpaStr));
    memset(idStr, '\0', sizeof(idStr));
    errno = 0;
    printf("Name: ");
    // TODO -- Error Handling when input exceeds length of buffer
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

    char *strEnd;
    gpa = strtof(gpaStr, &strEnd);
    studentID = strtol(idStr, &strEnd, STUDENT_ID_BUFF_SIZE);
    if (!strcmp(yearPtr, freshmanPtr)) {
        year = 0;
    }
    else if (!strcmp(yearPtr, sophmorePtr)) {
        year = 1;
    }
    else if (!strcmp(yearPtr, juniorPtr)) {
        year = 2;
    }
    else if (!strcmp(yearPtr, seniorPtr)) {
        year = 3;
    }
    else {
        printf("Please Enter a Valid year (freshman, sophomore, junior, senior)\n");
        free(namePtr);
        free(yearPtr);
        addStudent();
        return;
    }

    student* stu = (student*)malloc(sizeof(student));
    stu = createStudent(namePtr, year, gpa, studentID);
    printf("Inserting student: %s, %d, %f, %d\n", stu->name, stu->year, stu->gpa, stu->studentID);
    insertStudent(stu);

    free(namePtr);
    free(yearPtr);
    free(stu);
    return;
}

// Displays the data for the given student by name or ID
void displayStudent () {
    // TODO
}

// Quits the program and exits
void quit () {
    printf("Exiting...\n");
    exit(0);
}