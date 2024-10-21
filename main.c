#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARS 100
#define MAX_VAR_NAME 50
#define MAX_LINE_LENGTH 256

typedef struct {
    char name[MAX_VAR_NAME];
    double value;
} Variable;

Variable vars[MAX_VARS];
int var_count = 0;

double eval_expression(const char *expr);
double get_variable(const char *name);
void set_variable(const char *name, double value);
void execute_command(const char *command);
void execute_script(const char *filename);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <script_file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    execute_script(filename);

    return 0;
}

void execute_script(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char command[MAX_LINE_LENGTH];
    while (fgets(command, sizeof(command), file)) {
        command[strcspn(command, "\n")] = 0; // Remove newline character
        execute_command(command);
    }

    fclose(file);
}

void execute_command(const char *command) {
    if (strncmp(command, "print(", 6) == 0 && command[strlen(command) - 1] == ')') {
        // Extract variable name from print(x)
        char var_name[] = command + 6;
        var_name[strlen(var_name - 1)] = '\0'; // Remove the closing parenthesis
        printf("%p = %f\n", var_name, get_variable((const char*)var_name));
    } else if (strchr(command, '=')) {
        char var_name[MAX_VAR_NAME];
        char expr[256];

        sscanf(command, "%s = %[^\n]", var_name, expr);
        double value = eval_expression(expr);
        set_variable(var_name, value);
    } else {
        printf("Unknown command: %s\n", command);
    }
}

double eval_expression(const char *expr) {
    // For simplicity, this function only evaluates single numbers or variables
    if (isdigit(expr[0]) || expr[0] == '-') {
        return atof(expr);
    } else {
        return get_variable(expr);
    }
}

double get_variable(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            return vars[i].value;
        }
    }
    printf("Variable not found: %s\n", name);
    return 0;
}

void set_variable(const char *name, double value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            vars[i].value = value;
            return;
        }
    }
    if (var_count < MAX_VARS) {
        strcpy(vars[var_count].name, name);
        vars[var_count].value = value;
        var_count++;
    } else {
        printf("Variable limit reached.\n");
    }
}