#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
struct medication{
  int quantity;
  int price;
  char manufacturing_date[11];
  char expiry_date[11]; 
  char name[32];
  char brand[32];
  
};
/* FILE *fptr;
	 // Create a file
	fptr = fopen("ph.txt", "a");
	// Write some text to the file
	fprintf(fptr, "123456\n");
	// Close the file
	fclose(fptr);*/
int isExpiryDateValid(const char *expiry_date,const char *manufacturing_date) {
	time_t current_time = time(NULL);
    

    // Parse theexpiry date
    int day, month, year;
    sscanf(expiry_date, "%d/%d/%d", &day, &month, &year);

    struct tm expiry_tm = {0};
    expiry_tm.tm_mday = day;
    expiry_tm.tm_mon = month - 1;
    expiry_tm.tm_year = year - 1900;

    // Check if the expiry date is in the future
    return difftime(mktime(&expiry_tm), current_time) > 0;
}
char* ReadMedicationInfo(){
	
	
	struct medication med;
	static char OneLine[100];
	printf("\nEnter the medication name: ");
	//fgets(med.name, sizeof(med.name), stdin); 
	scanf("%[^\n]%*c", med.name);
	printf("Enter the available quantity: ");
	scanf("%d" , &med.quantity);
    getchar(); // Consume the newline character

    printf("Enter the unit price: ");
	scanf("%d" , &med.price);
    getchar(); // Consume the newline character
	
	
	
	
	printf("Enter the manufacturing date (DD/MM/YYYY): ");
	scanf("%[^\n]%*c", med.manufacturing_date);    

    
    printf("Enter the expiry date (DD/MM/YYYY): ");
	scanf("%[^\n]%*c", med.expiry_date);  

    printf("Enter the medication brand: ");
	scanf("%[^\n]%*c", med.brand);

   if (!isExpiryDateValid(med.expiry_date,med.manufacturing_date)) {
	   printf("Warning: Expiry date is near or exceeded. Adding the medication anyway...\n");
   }
    
	char str[10];
	
	strcpy(OneLine, med.name);
	
	strcat(OneLine, " ");
	
	sprintf(str, "%d", med.quantity);
	strcat(OneLine, str);
	
	strcat(OneLine, " ");
	
	sprintf(str, "%d", med.price);
	strcat(OneLine, str);
	
	strcat(OneLine, " ");
	
	strcat(OneLine, med.manufacturing_date);
	
	strcat(OneLine, " ");
	strcat(OneLine, med.expiry_date);
	
	strcat(OneLine, " ");
	strcat(OneLine, med.brand);
	
	return OneLine;
	
	

}
void AddNewMed(){
	system("clear");
	printf("########  Add Medication  ########");
	
	// Open the file in write mode (creates the file if it doesn't exist)
    FILE *file = fopen("ph.txt", "a");
    
    // Check if the file was opened successfully
    if (file == NULL) {
        perror("Error opening file");
    }
    
    // Write a line to the file
    fprintf(file, ReadMedicationInfo());
    fprintf(file, "\n");
    
    // Close the file
    fclose(file);
	// Create a file
	
 
}
void loadMedicationsFromFile(struct medication meds[], int *count) {
    FILE *file = fopen("ph.txt", "r");
    if (file == NULL) {
        printf("Warning: Unable to open file for reading. Starting with an empty stock.\n");
        return;
    }

    int current_index = 0;
    while (current_index < 100) {
        if (fscanf(file, "%s %d %d %s %s %s\n", meds[current_index].name, &meds[current_index].quantity,
                   &meds[current_index].price, meds[current_index].manufacturing_date,
                   meds[current_index].expiry_date, meds[current_index].brand) != EOF) {
            current_index++;
        } else {
            break;
	}
    }

    *count = current_index;
    fclose(file);
}

void DisplayListOfMed(){
	system("clear");
	struct medication medications[100];
	
    int medication_count = 0;
	loadMedicationsFromFile(medications, &medication_count);
	
	
	printf("########  Medications in stock  ########\n\n");
    printf("%-15s %-14s %-15s %-13s %-15s %s\n", "# Name", "# Quantity", "# Unit Price", "# Manu_Date", "# Expiry Date", "# Brand");
    printf("####################################################################################\n");
	for (int i = 0; i < medication_count; i++) {
        printf("# %-15s %-14d %-15d %-13s %-14s %s\n", medications[i].name, medications[i].quantity, medications[i].price, medications[i].manufacturing_date, medications[i].expiry_date, medications[i].brand); 
        
    }
   
    
    printf("\nPress Any Key to Continue...");
    getchar();
    
}

void searchMedication(struct medication medications[], int count, const char *searchStr) {
    int matches = 0;

    printf("Matching medications:\n");
    printf("%-15s %-14s %-15s %-13s %-15s %s\n", "# Name", "# Quantity", "# Unit Price", "# Manu_Date", "# Expiry Date", "# Brand");
    printf("####################################################################################\n");
    for (int i = 0; i < count; i++) {
        if (strncmp(medications[i].name, searchStr, strlen(searchStr)) == 0) {
			
            printf("# %-15s %-14d %-15d %-13s %-14s %s\n", medications[i].name, medications[i].quantity, medications[i].price, medications[i].manufacturing_date, medications[i].expiry_date, medications[i].brand); 

            matches++;
        }
    }

    if (matches == 0) {
        printf("No medications found matching the search criteria.\n");
    }
    getchar();
    printf("\nPress Any Key to Continue...");
    getchar();
    
}

void SearchForMed(){
	system("clear");
	struct medication medications[100];
	
    int medication_count = 0;
	loadMedicationsFromFile(medications, &medication_count);
	  
	char searchStr[30];

	printf("Enter the first characters of the medication name: ");
	scanf("%s", searchStr);
	searchMedication(medications, medication_count, searchStr);
	
}
void UpdateTheQuantityOfMed() {
    system("clear");
    struct medication medications[100];
    char name[30];

    int medication_count = 0;
    loadMedicationsFromFile(medications, &medication_count);
    printf("Enter the medication name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    // Loop through medications to find the one with the matching name
    int found = 0;
    for (int i = 0; i < medication_count; i++) {
        if (strcmp(name, medications[i].name) == 0) {
			printf("Enter the new available quantity: ");
			scanf("%d", &medications[i].quantity);
            found = 1;
            break;
        }
    }
	
    
    if (found) {
        FILE *file = fopen("ph.txt", "w");
    
		// Check if the file was opened successfully
		if (file == NULL) {
        perror("Error opening file");
		}
    
		// Write a line to the file
		for(int i=0; i< medication_count; i++){
			static char OneLine[100];
			char str[10];
			
			strcpy(OneLine, medications[i].name);
	
			strcat(OneLine, " ");
	
			sprintf(str, "%d", medications[i].quantity);
			strcat(OneLine, str);
	
			strcat(OneLine, " ");
			
			sprintf(str, "%d", medications[i].price);
			strcat(OneLine, str);
	
			strcat(OneLine, " ");
	
			strcat(OneLine, medications[i].manufacturing_date);
			
			strcat(OneLine, " ");
			strcat(OneLine, medications[i].expiry_date);
	
			strcat(OneLine, " ");
			strcat(OneLine, medications[i].brand);
	
			fprintf(file, OneLine);
			fprintf(file, "\n");
		}
    
		// Close the file
		fclose(file);
    } 
    else {
		printf("Medication not found!\n");
    }

	printf("\nPress Any Key to Continue...");
    getchar(); // Consume newline character from previous input
}
void DeleteMed(){
	system("clear");
    struct medication medications[100];
    char name[30];

    int medication_count = 0;
    loadMedicationsFromFile(medications, &medication_count);
    printf("Enter the medication name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    // Loop through medications to find the one with the matching name
    int found = 0;
    int ccc;
    for (int i = 0; i < medication_count; i++) {
        if (strcmp(name, medications[i].name) == 0) {
			ccc=i;
            found = 1;
            break;
        }
    }
	
    
    if (found) {
        FILE *file = fopen("ph.txt", "w");
    
		// Check if the file was opened successfully
		if (file == NULL) {
        perror("Error opening file");
		}
    
		// Write a line to the file
		for(int i=0; i< medication_count; i++){
			if (i != ccc){
				static char OneLine[100];
				char str[10];
			
				strcpy(OneLine, medications[i].name);
		
				strcat(OneLine, " ");
	
				sprintf(str, "%d", medications[i].quantity);
				strcat(OneLine, str);
			
				strcat(OneLine, " ");
			
				sprintf(str, "%d", medications[i].price);
				strcat(OneLine, str);
	
				strcat(OneLine, " ");
	
				strcat(OneLine, medications[i].manufacturing_date);
			
				strcat(OneLine, " ");
				strcat(OneLine, medications[i].expiry_date);
		
				strcat(OneLine, " ");
				strcat(OneLine, medications[i].brand);
	
				fprintf(file, OneLine);
				fprintf(file, "\n");
			}
		}
    
		// Close the file
		fclose(file);
    } 
    else {
		printf("Medication not found!\n");
    }
	printf("\nPress Any Key to Continue...");
    getchar();
}
void start(){
	
	int x;
	do
	{
		printf("1. Add a new medication to the stock\n");
		printf("2. Display the list of medications in stock\n");
		printf("3. Search for a medication by the first characters of its name\n");
		printf("4. Update the available quantity of a medication in stock\n");
		printf("5. Delete a medication from the stock\n");
		printf("99 to Exit\n");
		printf("Enter Your Choice: "); 
		scanf("%d",&x);
		getchar();
		if (x==1) AddNewMed();
		else if (x==2) DisplayListOfMed();
		else if (x==3) SearchForMed();
		else if (x==4) UpdateTheQuantityOfMed();
		else if (x==5) DeleteMed();
		else printf("Enter between 1-5\n");
		system("clear");
	}while (x != 99);
}
int main(){
	
	start();
	
	return 0;
}

