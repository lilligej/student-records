# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <ctype.h>
# include "sqlite/sqlite3.h"

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
char * sql_getElement(sqlite3 *db, sqlite3_stmt *q, char * stmt, long id);

/* stores data about each student */
struct Student {
    char name[NAME_BUFF_SIZE];
    int year;
    long studentID;
    double gpa;
};
typedef struct Student Student;

/* Main Function */
int main () {
    driver();
}

/* Dynamically allocates memory for student struct and returns a student struct pointer */
Student* createStudent (char* name, int year, double gpa, long studentID) {
    Student* stu = (Student*)malloc(sizeof(Student));
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

/* Clears Input Buffer by finding next "\n" character */
static void clearInput () {
    char arb;
    do {
        arb = getchar();
    } while (arb != '\n' && arb != EOF && arb != 0);
}

/* main driver of the main menu for the program. */
void driver () {
    int inputNum = 0;
    char numString[1];
    memset(&numString, 0, 1 * sizeof(char));

    while (1) {
        printf("Welcome to Grade Management System!\n1. Add Student\n2. Modify Student\n3. Display All Grades\n4. Display Student Grades\n5. Remove Student\n6. Exit\n");
        scanf("%1s", &numString);
        if (!isdigit(numString[0])) {
            printf("Please Input Valid Number\n");
            continue;
        }
        inputNum = atoi(numString);
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

/* Callback from web */
static int callback(void *data, int argc, char **argv, char **azColName){
    int i;

    fprintf(stderr, "%s: ", (const char*)data);
    for(i=0; i<argc; i++){
      printf("%s = %s, ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

/* Inserts student data into database. */
void insertStudent(Student *stu) {
    // SQL Schema: name, grade, gpa, studentID
    sqlite3 *db;
    sqlite3_stmt *q;
    int rc = 0;
    char *sqlErr = 0;
    char *statement = "INSERT INTO records VALUES (?1,?2,?3,?4)";

    // Opens database 'records'
    rc = sqlite3_open("student_records.db", &db);
    if (rc) {
        printf("ERROR: Unable to Open DataBase");
        exit(1);
    }

    // Prepare SQL statement to insert student data into statement
    rc = sqlite3_prepare_v2(db, statement, -1, &q, NULL);
    if (rc != SQLITE_OK) {
        printf("ERROR: Failed to prepare SQL statement\n");
        sqlite3_close(db);
        exit(1);
    }

    // Binds student data to SQL statement
    rc = sqlite3_bind_text(q, 1, stu->name, 20, SQLITE_STATIC);
    if(rc != SQLITE_OK) {
		printf("ERROR: Failed to bind name\n");
		sqlite3_close(db);
		exit(1);
	}
    sqlite3_bind_int(q, 2, stu->year);
    sqlite3_bind_double(q, 3, stu->gpa);
    sqlite3_bind_int(q, 4, stu->studentID);

    // Exectes SQL statement
    rc = sqlite3_step(q);
    if (rc != SQLITE_OK && rc != SQLITE_DONE) {
        fprintf(stderr, "SQL ERROR: %s\n", sqlite3_errmsg(db));
        printf("SQL ERRROR NUMBER: %d", rc);
        sqlite3_free(sqlErr);
    }

    sqlite3_finalize(q);
    sqlite3_close(db);
    return;
}

/* Returns corresponding int for the string representation of grade level. */
int gradeToInt (char *yearPtr) {
    char freshman[] = "freshman";
    char sophomore[] = "sophomore";
    char junior[] = "junior";
    char senior[] = "senior";
    int year = -1;

    if (!strcmp(yearPtr, freshman)) {
        year = 0;
    }
    else if (!strcmp(yearPtr, sophomore)) {
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

/* Converts integer (0-3) to coresponding string */
char* intToGrade (int num) {
    char freshman[] = "freshman\0";
    char sophomore[] = "sophomore\0";
    char junior[] = "junior\0";
    char senior[] = "senior\0";
    char *year = NULL;

    switch (num) {
        case 0:
            year = freshman;
            break;
        case 1:
            year = sophomore;
            break;
        case 2:
            year = junior;
            break;
        case 3:
            year = senior;
            break;
        default:
            year = "UNDEFINED";
            exit(0);
    }
    return year;
}

/* TODO: shorten to be ideally 35
   Prompts user and receives data for required student information and stores data as student struct */
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
    if (strlen(namePtr) > NAME_BUFF_SIZE - 2) {
        printf("ERROR: Name length too long\n");
        exit(1);
    }
    namePtr[strcspn(namePtr, "\n")] = 0;
    fflush(stdin);
    printf("Year: ");
    scanf("%16s", yearPtr);
    if (strlen(yearPtr) > YEAR_BUFF_SIZE - 2) {
        printf("ERROR: Year length too long\n");
        exit(1);
    }

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
    Student* stu = (Student*)malloc(sizeof(Student));
    stu = createStudent(namePtr, year, gpa, studentID);
    printf("Inserting student: %s, %d, %f, %ld\n", stu->name, stu->year, stu->gpa, stu->studentID);
    insertStudent(stu);

    free(namePtr);
    free(yearPtr);
    free(stu);
    return;
}

/* TODO: Returns student struct with student information by querying database by given name */
Student* getStudentByName (char *name) {
    // TODO
    sqlite3 *db;
    sqlite3_stmt *q;
    int rc = 0;
    char *sqlErr = 0;

    char *statement = "SELECT grade FROM records WHERE name=?1";

    printf("Search By Name Not Currently Supported\n");
    exit(0);
    return NULL;
}

char * sql_getElement(sqlite3 *db, sqlite3_stmt *q, char * stmt, long id) {
    int rc = 0;
    const char* retTemp = (char*)calloc(NAME_BUFF_SIZE + 1, sizeof(char));
    char* ret = (char*)calloc(NAME_BUFF_SIZE + 1, sizeof(char));
    
    // Opens database 'records'
    rc = sqlite3_open("student_records.db", &db);
    if (rc) {
        printf("ERROR: Unable to Open DataBase");
        exit(1);
    }

    // Prepare SQL statement to insert student data into statement
    rc = sqlite3_prepare_v2(db, stmt, -1, &q, NULL);
    if (rc != SQLITE_OK) {
        printf("ERROR: Failed to prepare SQL statement\n");
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_bind_int(q, 1, id);
    rc = sqlite3_step(q);
    
    if (rc == SQLITE_ROW) {
        retTemp = sqlite3_column_text(q, 0);
    }
    else {
        return NULL;
    }

    strncpy_s(ret, NAME_BUFF_SIZE, retTemp, NAME_BUFF_SIZE * sizeof(char));
    ret[NAME_BUFF_SIZE] = '\0';

    return ret;
}

/* Returns student struct with student information by querying database by given studentID. */
Student* getStudentByID (long id) {
    // TODO - Shorten Function
    Student *stu = NULL;
    sqlite3 *db;
    sqlite3_stmt *q;
    int rc = 0;
    char *sqlErr = 0;
    char *nameSTMT = "SELECT name FROM records where studentID IS ?1";
    char *gradeSTMT = "SELECT grade FROM records where studentID IS ?1";
    char *gpaSTMT = "SELECT gpa FROM records where studentID IS ?1";
    char *studentIDSTMT = "SELECT studentID FROM records WHERE studentID is ?1";

    // change retName to a char array or figure out what to do with that
    char* retName = (char*)calloc(NAME_BUFF_SIZE, sizeof(char));
    int retYear = -1;
    long retStudentID = -1;
    double retGPA = -1;

    char* tmpName = (char*)calloc(NAME_BUFF_SIZE, sizeof(char));

    // Opens database 'records'
    rc = sqlite3_open("student_records.db", &db);
    if (rc) {
        printf("ERROR: Unable to Open DataBase");
        exit(1);
    }

    retName = sql_getElement(db, q, nameSTMT, id);
    retYear = atoi(sql_getElement(db, q, gradeSTMT, id));
    retGPA = atof(sql_getElement(db, q, gpaSTMT, id));
    retStudentID = atol(sql_getElement(db, q, studentIDSTMT, id));

    // Checks if student was found
    if (retName == NULL) return stu;

    stu = createStudent(retName, retYear, retGPA, retStudentID);

    // Clean-up database
    sqlite3_finalize(q);
    sqlite3_close(db);
    return stu;
}

/* TODO: Displays all students and their information */
void displayAll() {
    
}

/* Displays the data for the given student by name or ID */
void displayStudent () {
    char *input = calloc(NAME_BUFF_SIZE, sizeof(char));
    char *gradeString;
    Student *stu;

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
    if (stu == NULL) {
        printf("================\nStudent Not Found!\n================\n");
        return;
    }
    gradeString = intToGrade(stu->year);
    printf("================\nName: %s\nGrade: %s\nGPA: %f\nStudent ID: %ld\n================\n", stu->name, intToGrade(stu->year), stu->gpa, stu->studentID);
    free(stu);
    return;
}

/* Quits the program. */
void quit () {
    printf("Exiting...\n");
    exit(0);
}