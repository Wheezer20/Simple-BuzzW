#include <stdlib.h>
#include <stdio.h>
#include "score_logic.h"
#include "display.h"
#include "buzzer.h"
#include "millis.h"

#define MAX_CONTACTS 50 //subject to change if needed

static uint16_t *contactTimes = NULL;
static uint16_t contactCount = 0;
static unsigned long startTime = 0;
static uint8_t failed = 0; // NEW: failure flag
static unsigned long contactDisplayTime = 0; // Time when contact count display started
static uint8_t showContactCount = 0; // Flag: 1=show contact count, 0=show timer

void startGame(uint8_t mode) {
    contactCount = 0;
    failed = 0;
    
    // Free existing memory if it was already allocated
    if (contactTimes) {
        free(contactTimes);
        contactTimes = NULL;
    }
    
    // Allocate memory for contact times array
    contactTimes = malloc(MAX_CONTACTS * sizeof(uint16_t));
    if (!contactTimes) {
        // if memory allocation fails:
        printString("ERROR: Memory allocation failed!\r\n");
        return;
    }
    
    startTime = millis();
    contactDisplayTime = 0;
    showContactCount = 0; // Start by showing timer, not contact count
}


void handleContact(uint8_t touched) {
    if (touched && contactCount < MAX_CONTACTS && contactTimes) {
        // Record the contact time and increment count (only if memory is allocated)
        contactTimes[contactCount++] = (uint16_t)(millis() - startTime);
        failed = 1;
        
        // Trigger contact count display for 1 second
        showContactCount = 1;
        contactDisplayTime = millis(); // Record when display started
        
        printString("Contact count now: ");
        printWord(contactCount);
        printString("\r\n");
    }
}

void displayTimer() {
    // Check if 1 second has passed since contact count display started
    if (showContactCount && (millis() - contactDisplayTime >= 1000)) {
        showContactCount = 0; // Switch back to timer display
    }
    
    // Get reference to the global display buffer from main.c
    extern char display_buffer[5];
    
    if (showContactCount) {
        // Display contact count in format "C###" (e.g., "C001", "C002", etc.)
        snprintf(display_buffer, 5, "C%03u", contactCount);
    } else {
        // Display timer in format "MMSS" (minutes and seconds)
        unsigned long current = millis();
        uint16_t elapsed = (uint16_t)(current - startTime);
        uint8_t seconds = (elapsed / 1000) % 60;
        uint8_t minutes = (elapsed / 60000);
        snprintf(display_buffer, 5, "%02u%02u", minutes, seconds);
    }
    
    // No direct display writing - let display_tick() handle the refresh
}


void stopGame(void) {

    unsigned long endTime = millis();
    unsigned long totalTime = endTime - startTime;
    uint16_t penalty = contactCount * 10;

    int32_t finalScore = (int32_t)(totalTime / 1000) - penalty;
    //number of seconds it took you to complete the course minus 10 * the number of contacts

    if (finalScore < 0) finalScore = 0; // weird score bug, if final score is negative, set to 0

    // S.M report
    printString("\n=== Game Statistics: ==\r\n");
    printString("Total time: ");
    printWord((uint16_t)(totalTime / 1000));
    printString(" s\r\n");

    printString("Contacts: ");
    // Print contact count as a regular decimal number (no leading zeros)
    char buf[8];
    snprintf(buf, sizeof(buf), "%u", contactCount);
    printString(buf);
    printString("\r\n");

    printString("Penalty: ");
    printWord(penalty);
    printString(" (10s per contact)\r\n");

    printString("Final score: ");
    printWord((uint16_t)finalScore);
    printString("\r\n");

    if (contactCount > 1) {
        uint16_t min = 0xFFFF, max = 0;
        uint32_t sum = 0;

        for (uint16_t i = 1; i < contactCount; i++) {
            uint16_t delta = contactTimes[i] - contactTimes[i - 1];
            if (delta < min) min = delta;
            if (delta > max) max = delta;
            sum += delta;
        }

        uint16_t avg = (uint16_t)(sum / (contactCount - 1));

    printString("Min interval: ");
    printWord(min / 1000);
    printString(" s\r\n");

    printString("Max interval: ");
    printWord(max / 1000);
    printString(" s\r\n");

    printString("Avg interval: ");
    printWord(avg / 1000);
    printString(" s\r\n");
    }

    for (uint16_t i = 0; i < contactCount; ++i) {
        printString("Touch ");
        // Print touch number as a regular decimal number (no leading zeros)
    
        snprintf(buf, sizeof(buf), "%u", i + 1); // should increment the touch # for each contact thingy
        printString(buf);
        printString(": "); //*idk somehow this works better

        // Print touch time in seconds as a regular decimal number (no leading zeros)
        snprintf(buf, sizeof(buf), "%u", contactTimes[i] / 1000);
        printString(buf);
        printString(" s\r\n");
    }

    free(contactTimes);
    contactTimes = NULL;
}

void resetScoreLogic(void) {
    if (contactTimes) free(contactTimes);
    contactTimes = NULL;
    contactCount = 0;
    failed = 0;
    startTime = 0;
    // Reset contact count display variables
    contactDisplayTime = 0;
    showContactCount = 0;
}

uint8_t gameFailedCondition(void) {
    return failed;
}

uint32_t getElapsedGameTime(void) {
    if (startTime == 0) {
        return 0; // Game hasn't started yet
    }
    return (uint32_t)((millis() - startTime) / 1000); // Return elapsed seconds
}
