#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 2147483647
#define INT_MIN -2147483648

typedef struct bank {
    char name[50];
    int netAmount;
    char types[50][50];
    int numTypes;
} Bank;

int getMinIndex(Bank listOfNetAmounts[], int numBanks) {
    int min = INT_MAX, minIndex = -1;
    for (int i = 0; i < numBanks; i++) {
        if (listOfNetAmounts[i].netAmount == 0) continue;

        if (listOfNetAmounts[i].netAmount < min) {
            minIndex = i;
            min = listOfNetAmounts[i].netAmount;
        }
    }
    return minIndex;
}

int getSimpleMaxIndex(Bank listOfNetAmounts[], int numBanks) {
    int max = INT_MIN, maxIndex = -1;
    for (int i = 0; i < numBanks; i++) {
        if (listOfNetAmounts[i].netAmount == 0) continue;

        if (listOfNetAmounts[i].netAmount > max) {
            maxIndex = i;
            max = listOfNetAmounts[i].netAmount;
        }
    }
    return maxIndex;
}

int commonTypeExists(Bank bank1, Bank bank2, char matchingType[]) {
    for (int i = 0; i < bank1.numTypes; i++) {
        for (int j = 0; j < bank2.numTypes; j++) {
            if (strcmp(bank1.types[i], bank2.types[j]) == 0) {
                strcpy(matchingType, bank1.types[i]);
                return 1;
            }
        }
    }
    return 0;
}

int getMaxIndex(Bank listOfNetAmounts[], int numBanks, int minIndex, Bank input[], char matchingType[]) {
    int max = INT_MIN, maxIndex = -1;
    for (int i = 0; i < numBanks; i++) {
        if (listOfNetAmounts[i].netAmount == 0 || listOfNetAmounts[i].netAmount < 0) continue;

        if (commonTypeExists(listOfNetAmounts[minIndex], listOfNetAmounts[i], matchingType) && max < listOfNetAmounts[i].netAmount) {
            max = listOfNetAmounts[i].netAmount;
            maxIndex = i;
        }
    }
    return maxIndex;
}

void printAns(int ansGraph[][50], char ansGraphType[][50][50], int numBanks, Bank input[]) {
    printf("\nThe transactions for minimum cash flow are as follows:\n\n");
    for (int i = 0; i < numBanks; i++) {
        for (int j = 0; j < numBanks; j++) {
            if (i == j) continue;

            if (ansGraph[i][j] != 0) {
                printf("%s pays Rs %d to %s via %s\n", input[i].name, ansGraph[i][j], input[j].name, ansGraphType[i][j]);
                ansGraph[i][j] = 0;
            }
        }
    }
}

void minimizeCashFlow(int numBanks, Bank input[], int numTransactions, int graph[][50], int maxNumTypes) {
    Bank listOfNetAmounts[numBanks];
    int ansGraph[50][50] = {0};
    char ansGraphType[50][50][50] = {0};

    for (int b = 0; b < numBanks; b++) {
        strcpy(listOfNetAmounts[b].name, input[b].name);
        memcpy(listOfNetAmounts[b].types, input[b].types, sizeof(input[b].types));
        listOfNetAmounts[b].numTypes = input[b].numTypes;

        int amount = 0;
        for (int i = 0; i < numBanks; i++) {
            amount += graph[i][b];
        }
        for (int j = 0; j < numBanks; j++) {
            amount -= graph[b][j];
        }
        listOfNetAmounts[b].netAmount = amount;
    }

    int numZeroNetAmounts = 0;
    for (int i = 0; i < numBanks; i++) {
        if (listOfNetAmounts[i].netAmount == 0) numZeroNetAmounts++;
    }

    while (numZeroNetAmounts != numBanks) {
        int minIndex = getMinIndex(listOfNetAmounts, numBanks);
        char matchingType[50];
        int maxIndex = getMaxIndex(listOfNetAmounts, numBanks, minIndex, input, matchingType);

        if (maxIndex == -1) {
            int simpleMaxIndex = getSimpleMaxIndex(listOfNetAmounts, numBanks);
            strcpy(matchingType, input[simpleMaxIndex].types[0]);
            ansGraph[minIndex][simpleMaxIndex] += abs(listOfNetAmounts[minIndex].netAmount);
            strcpy(ansGraphType[minIndex][simpleMaxIndex], matchingType);

            listOfNetAmounts[simpleMaxIndex].netAmount += listOfNetAmounts[minIndex].netAmount;
            listOfNetAmounts[minIndex].netAmount = 0;

            if (listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            if (listOfNetAmounts[simpleMaxIndex].netAmount == 0) numZeroNetAmounts++;
        } else {
            int transactionAmount = (abs(listOfNetAmounts[minIndex].netAmount) < listOfNetAmounts[maxIndex].netAmount) ? abs(listOfNetAmounts[minIndex].netAmount) : listOfNetAmounts[maxIndex].netAmount;
            ansGraph[minIndex][maxIndex] += transactionAmount;
            strcpy(ansGraphType[minIndex][maxIndex], matchingType);

            listOfNetAmounts[minIndex].netAmount += transactionAmount;
            listOfNetAmounts[maxIndex].netAmount -= transactionAmount;

            if (listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            if (listOfNetAmounts[maxIndex].netAmount == 0) numZeroNetAmounts++;
        }
    }
    printAns(ansGraph, ansGraphType, numBanks, input);
}

int main() {
    printf("\n********************* Welcome to CASH FLOW MINIMIZER SYSTEM ***********************\n\n\n");
    printf("This system minimizes the number of transactions among multiple banks in the different corners of the world that use different modes of payment. There is one world bank (with all payment modes) to act as an intermediary between banks that have no common mode of payment.\n\n");
    printf("Enter the number of banks participating in the transactions.\n");
    int numBanks;
    scanf("%d", &numBanks);

    Bank input[numBanks];
    int graph[50][50] = {0};

    printf("Enter the details of the banks and transactions as stated:\n");
    printf("Bank name, number of payment modes it has and the payment modes.\n");
    printf("Bank name and payment modes should not contain spaces.\n");

    int maxNumTypes;
    for (int i = 0; i < numBanks; i++) {
        if (i == 0) {
            printf("World Bank: ");
        } else {
            printf("Bank %d: ", i);
        }
        scanf("%s", input[i].name);
        int numTypes;
        scanf("%d", &numTypes);

        if (i == 0) maxNumTypes = numTypes;

        for (int j = 0; j < numTypes; j++) {
            scanf("%s", input[i].types[j]);
        }
        input[i].numTypes = numTypes;
    }

    printf("Enter the number of transactions.\n");
    int numTransactions;
    scanf("%d", &numTransactions);

    printf("Enter the details of each transaction as stated:\n");
    printf("Debtor Bank, creditor Bank, and amount\n");
    for (int i = 0; i < numTransactions; i++) {
        printf("%d th transaction: ", i);
        char debtor[50], creditor[50];
        int amount;
        scanf("%s %s %d", debtor, creditor, &amount);
        
        int debtorIndex = -1, creditorIndex = -1;
        for (int k = 0; k < numBanks; k++) {
            if (strcmp(input[k].name, debtor) == 0) {
                debtorIndex = k;
            }
            if (strcmp(input[k].name, creditor) == 0) {
                creditorIndex = k;
            }
        }
        if (debtorIndex != -1 && creditorIndex != -1) {
            graph[debtorIndex][creditorIndex] = amount;
        }
    }

    minimizeCashFlow(numBanks, input, numTransactions, graph, maxNumTypes);
    return 0;
}
