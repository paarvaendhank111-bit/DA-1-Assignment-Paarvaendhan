#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OT_RATE 100

struct Employee {
    int empID;
    char name[50];
    float basicPay;
    int otHours;
};

// Function declarations
void addEmployee();
void displayAll();
void generatePayslip();
void searchEmployee();
void deleteEmployee();

float calculateGross(float basic, int ot);
float calculateTax(float gross);
int validateInt(int value);
float validateFloat(float value);

int main() {
    struct Employee e = {101, "Clement", 25000, 5};

    FILE *fp = fopen("employee.dat", "wb");
    fwrite(&e, sizeof(e), 1, fp);
    fclose(fp);

    printf("===== Employee Payroll System =====\n");

    float gross = calculateGross(e.basicPay, e.otHours);
    float tax = calculateTax(gross);
    float net = gross - tax;

    printf("\n===== PAYSLIP =====\n");
    printf("EmpID: %d\n", e.empID);
    printf("Name: %s\n", e.name);
    printf("Basic Pay: %.2f\n", e.basicPay);
    printf("OT Pay: %.2f\n", (float)e.otHours * OT_RATE);
    printf("Gross Pay: %.2f\n", gross);
    printf("Tax: %.2f\n", tax);
    printf("Net Pay: %.2f\n", net);
    printf("===================\n");

    return 0;
}

// Validation
int validateInt(int value) {
    if (value < 0) {
        printf("Invalid input!\n");
        return 0;
    }
    return 1;
}

float validateFloat(float value) {
    if (value < 0) {
        printf("Invalid input!\n");
        return 0;
    }
    return 1;
}

// Add Employee
void addEmployee() {
    struct Employee e;
    FILE *fp = fopen("employee.dat", "ab");

    if (!fp) {
        printf("File error!\n");
        return;
    }

    printf("Enter EmpID: ");
    fflush(stdout);
    scanf("%d", &e.empID);
    if (!validateInt(e.empID)) return;

    printf("Enter Name: ");
    fflush(stdout);
    scanf(" %[^\n]", e.name);

    printf("Enter Basic Pay: ");
    fflush(stdout);
    scanf("%f", &e.basicPay);
    if (!validateFloat(e.basicPay)) return;

    printf("Enter OT Hours: ");
    fflush(stdout);
    scanf("%d", &e.otHours);
    if (!validateInt(e.otHours)) return;

    fwrite(&e, sizeof(e), 1, fp);
    fclose(fp);

    printf("Employee added successfully!\n");
}

// Display all
void displayAll() {
    struct Employee e;
    FILE *fp = fopen("employee.dat", "rb");

    if (!fp) {
        printf("No records found!\n");
        return;
    }

    printf("\n--- Employee Records ---\n");

    while (fread(&e, sizeof(e), 1, fp)) {
        printf("ID: %d | Name: %s | Basic: %.2f | OT: %d\n",
               e.empID, e.name, e.basicPay, e.otHours);
    }

    fclose(fp);
}

// Search employee
void searchEmployee() {
    int id, found = 0;
    struct Employee e;
    FILE *fp = fopen("employee.dat", "rb");

    if (!fp) {
        printf("No records found!\n");
        return;
    }

    printf("Enter EmpID: ");
    fflush(stdout);
    scanf("%d", &id);

    while (fread(&e, sizeof(e), 1, fp)) {
        if (e.empID == id) {
            printf("Found: %d | %s | %.2f | %d\n",
                   e.empID, e.name, e.basicPay, e.otHours);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Employee not found!\n");

    fclose(fp);
}

// Delete employee
void deleteEmployee() {
    int id;
    struct Employee e;
    FILE *fp = fopen("employee.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!fp || !temp) {
        printf("File error!\n");
        return;
    }

    printf("Enter EmpID to delete: ");
    fflush(stdout);
    scanf("%d", &id);

    int found = 0;

    while (fread(&e, sizeof(e), 1, fp)) {
        if (e.empID != id) {
            fwrite(&e, sizeof(e), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("employee.dat");
    rename("temp.dat", "employee.dat");

    if (found)
        printf("Employee deleted successfully!\n");
    else
        printf("Employee not found!\n");
}

// Calculations
float calculateGross(float basic, int ot) {
    return basic + (ot * OT_RATE);
}

float calculateTax(float gross) {
    if (gross < 20000)
        return 0;
    else if (gross < 50000)
        return gross * 0.1;
    else
        return gross * 0.2;
}

// Payslip
void generatePayslip() {
    int id, found = 0;
    struct Employee e;
    FILE *fp = fopen("employee.dat", "rb");

    if (!fp) {
        printf("No records found!\n");
        return;
    }

    printf("Enter EmpID: ");
    fflush(stdout);
    scanf("%d", &id);

    while (fread(&e, sizeof(e), 1, fp)) {
        if (e.empID == id) {
            float gross = calculateGross(e.basicPay, e.otHours);
            float tax = calculateTax(gross);
            float net = gross - tax;

            printf("\n===== PAYSLIP =====\n");
            printf("EmpID: %d\n", e.empID);
            printf("Name: %s\n", e.name);
            printf("Basic Pay: %.2f\n", e.basicPay);
            printf("OT Pay: %.2f\n", (float)e.otHours * OT_RATE);
            printf("Gross Pay: %.2f\n", gross);
            printf("Tax: %.2f\n", tax);
            printf("Net Pay: %.2f\n", net);
            printf("===================\n");

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Employee not found!\n");

    fclose(fp);
}
