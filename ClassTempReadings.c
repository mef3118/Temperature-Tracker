#include <stdio.h>
#include <wiringPi.h>

#define LED_PIN 0

int main() {
    int temp;
    int sum = 0;
    int count = 0;
    int minTemp;
    int maxTemp;

    if (wiringPiSetup() == -1) {
        printf("Error initializing WiringPi.\n");
        return 1;
    }
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    FILE *file = fopen("classroom_temps.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    printf("Enter minimum acceptable temperature (F): ");
    scanf("%d", &minTemp);

    printf("Enter maximum acceptable temperature (F): ");
    scanf("%d", &maxTemp);
    
    fprintf(file, "Acceptable Temperature Range: %dF - %dF\n", minTemp, maxTemp);
    fprintf(file, "--------------------------------------------\n");

    printf("Enter Classroom Temperatures in Fahrenheit (enter -999 to stop):\n");

    while (1) {
        if (scanf("%d", &temp) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            while (getchar() != '\n');
            continue;
        }

        if (temp == -999) {
            break;
        }

        if (temp >= minTemp && temp <= maxTemp) {
            printf("Status: Normal\n\n");
            fprintf(file, "Temperature: %dF | Status: Normal\n", temp);
            digitalWrite(LED_PIN, LOW);
        } else {
            printf("Status: Warning\n\n");
            fprintf(file, "Temperature: %dF | Status: Warning\n", temp);
            digitalWrite(LED_PIN, HIGH);
        }

        sum += temp;
        count++;
    }

    digitalWrite(LED_PIN, LOW);

    if (count > 0) {
        double average = (double)sum / count;
        printf("\nTotal temperatures entered: %d\n", count);
        printf("Average classroom temperature: %.2lfF\n", average);

        fprintf(file, "--------------------------------------------\n");
        fprintf(file, "Total temperatures entered: %d\n", count);
        fprintf(file, "Average classroom temperature: %.2lfF\n", average);
    }
    else {
        printf("\nNo temperatures were entered.\n");
        fprintf(file, "No temperatures were entered.\n");
    }

    fclose(file);
    printf("Results saved to classroom_temps.txt.\n");

    return 0;
}