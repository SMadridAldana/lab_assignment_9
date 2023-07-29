#include <stdio.h>
#include <stdlib.h>
#define TABLE_SIZE 15

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType data;
	struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
	return x%TABLE_SIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		if(pHashArray[i].data.id == -1 && pHashArray[i].data.order == 0) continue;
		else{
			printf("Index %2i \t", i);
			struct HashType* pTemp = &pHashArray[i];

			do{
				printf("-> %i %c %i \t", pTemp->data.id, pTemp->data.name, pTemp->data.order);
				pTemp = pTemp->next;  
			} while(pTemp != NULL);

			printf("\n");
		}
	}
}

void InitializeHash(struct HashType* myTable){
	for(int i = 0; i < TABLE_SIZE; i++){
		myTable[i].data.id = -1;
		myTable[i].data.name = ' ';
		myTable[i].data.order = 0;
		myTable[i].next = NULL;
	}
}

void FillHash(struct HashType* myTable, struct RecordType *myData, int recordSz){

	int index;
	for(int i = 0; i < recordSz; i ++){
		index = hash(myData[i].order);
		if(myTable[index].data.id == -1 && myTable[index].data.order == 0){

			myTable[index].data.id = myData[i].id;
			myTable[index].data.order = myData[i].order;
			myTable[index].data.name = myData[i].name;
		}
		else{

			struct HashType* newNode = (struct HashType*) malloc(sizeof(struct HashType));
			newNode->data.id = myData[i].id;
			newNode->data.order = myData[i].order;
			newNode->data.name = myData[i].name;
			newNode->next = NULL;

			if(myTable[index].next == NULL) myTable[index].next = newNode;
			else{
				struct HashType* pTemp = myTable[index].next;
				while(pTemp->next != NULL) pTemp = pTemp->next;
				pTemp->next = newNode;
			}
		}
	}

}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

	struct HashType myTable[TABLE_SIZE];
	InitializeHash(myTable);
	FillHash(myTable, pRecords, recordSz);
	displayRecordsInHash(myTable, TABLE_SIZE);

}