
/*
Pealkiri: IAS0090 Praktikumiülesannete lahendused
Nimi: Aleksander Laasmägi 213453IACB
Kuupäev: 02.11.2023
Töö variant: Struct3 ja Object1
*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "DateTime.h"
#include "Objects.h"
#include "Headers.h"
#include "Structs.h"
#include "time.h"
#include "malloc.h"
#include "ctype.h"
#pragma warning ( disable : 4996 )

// Funktsioonide deklaratsioonid
void PrintObjects(HeaderB *pStruct3);
int InsertNewObject(HeaderB **pStruct3, char *pNewID, int newCode);
Object1* CreateNewObject(char *identificator, int code);
int InputCompatibility(char *identificator);
char HeaderAFirstChar(char *identificator);
void GetUserInput(char **identificator, int *code);
Object1* RemoveExistingObject(HeaderB **pStruct3, char *pExistingID);
Node* CreateBinaryTree(HeaderB* pStruct3);
Stack* Push(Stack* ptrStack, void* ptrObj);
Stack* Pop(Stack* ptrStack, void** ptrObj);
void TreeTraversal(Node* pTree);
bool DoesCodeExist(HeaderB* pStruct3, unsigned long int c);
Node* DeleteTreeNode(Node* pTree, unsigned long int Code);
int MenuOptions();

int main()
{
	// Vajalike lokaalsete muutujate deklareerimine
	int option = 0;	
	int r_code, i;
	char *userInputCode = NULL;
	int mockUserInput = -1;
	Object1* deletedObject; 
	const char* demo_input[] = 
	{ "Dx Gz", "Dx Ga", "Db Aa", "Dk Za", "Dr Wa", "Aa Aa", "Ab Ba", "Za Aa", "Za Ab", "Za Ba", "Wx Xa", "Wx Aa", "zb Kk",
	"Zc ca", "Dr Wa", "ZB kk", "Fa", "Fa_Fa"};

	HeaderB *pStruct3 = GetStruct3(1, 35);
	while (1)
	{
		option = MenuOptions();	// Menüü sisestuse küsimine
		switch (option)
		{
			case 0:
				return 0;
			case 1:
				PrintObjects(pStruct3);
				break;
			case 2:
				userInputCode = NULL;
				int userInputID;
				GetUserInput(&userInputCode, &userInputID);
				if (InsertNewObject(&pStruct3, userInputCode, userInputID) == 1)
				{
					printf("Uue objekti lisamine 6nnestus!\n\n\n\n");
				}
				else
				{
					printf("Uue objekti lisamine eba6nnestus!\n\n\n\n");
				}
				break;
			case 3:
				userInputCode = NULL;
				GetUserInput(&userInputCode, &mockUserInput);
				deletedObject = RemoveExistingObject(&pStruct3, userInputCode);
				if (deletedObject != NULL)
				{
					printf("Objekti eemaldamine 6nnestus!\n");
					printf("Eemaldatud objekt: %s\n\n\n\n", *deletedObject);
				}
				else
				{
					printf("Objekti eemaldamine eba6nnestus!\n\n\n\n");
				}
				free(deletedObject);
				break;
			case 4:
				printf("Objektid enne DEMO algust:\n");
				PrintObjects(pStruct3);
				for (int i = 0; i < 18; i++)
				{
					if (InsertNewObject(&pStruct3, (char*)demo_input[i], 1000000 + i) == 1)
					{
						printf("Objekti %s lisamine 6nnestus!\n", demo_input[i]);
					}
					else
					{
						printf("Objekti %s lisamine eba6nnestus!\n", demo_input[i]);
					}
				}
				printf("\n\n\n");
				printf("Objektid peale kirjete lisamist:\n");
				PrintObjects(pStruct3);
				for (int i = 0; i < 18; i++)
				{
					deletedObject = RemoveExistingObject(&pStruct3, (char*)demo_input[i]);
					if (deletedObject != NULL)
					{
						printf("Objekti %s eemaldamine 6nnestus!\n", *deletedObject);
					}
					else
					{
						printf("Objekti %s eemaldamine eba6nnestus!\n", demo_input[i]);
					}
				}
				printf("\n\n\n");
				PrintObjects(pStruct3);
				printf("\n\n\n\n");
				break;
			case 5:
				pStruct3 = GetStruct3(1, 35);
				printf("Lähtestruktuur, kui N on 35:\n");
				PrintObjects(pStruct3);
				printf("\nKahendpuu moodustamine:\n");
				Node *root1 = CreateBinaryTree(pStruct3);
				TreeTraversal(root1);
				root1 = DeleteTreeNode(root1, 316985719);
				printf("\nPeale juure eemaldamist:\n");
				TreeTraversal(root1);
				printf("\n\n\nLähtestruktuur, kui N on 10:\n");
				pStruct3 = GetStruct3(1, 10);
				PrintObjects(pStruct3);
				printf("\nKahendpuu moodustamine:\n");
				Node *root2 = CreateBinaryTree(pStruct3);
				TreeTraversal(root2);
				while(1)
				{
					printf("\nSisestage tipp, mida eemaldada (-1, kui pole rohkem eemaldamissoovi): ");
					scanf("%d", &r_code);
					if (DoesCodeExist(pStruct3, r_code) == 1)
					{
						printf("Eemaldati objekt koodiga %lu\n\n", r_code);
						root2 = DeleteTreeNode(root2, r_code);
						TreeTraversal(root2);
					}
					else if (r_code == -1)
					{
						break;
					}
					else
					{
						printf("Kood %lu ei eksisteeri selles puus\n\n", r_code);
					}
				}
				break;
		}
		if (option < 0 || option > 5)
		{
			printf("Sellist tegevust ei eksisteeri!\n\n\n");
		}
	}
	free(deletedObject);
	return 0;
}

/*
Kirjeldus: Kuvab kõik struktuurisisesed objektid.
Parameetrid: *pStruct3 - lähtestruktuuri esimene siduja.
Tagastus: puudub
*/
void PrintObjects(HeaderB *pStruct3) 
{
	int objCnt = 0;	// Nummerdamise eesmärgil loodud muutuja

	if (pStruct3 != NULL)	// Sisend ei või tühi olla
	{
		HeaderB *ptrB = pStruct3;	// HeaderA viit, mis võrdsustatakse sisendiga
		for (ptrB = pStruct3; ptrB; ptrB = ptrB->pNext)	// Kammib kõik HeaderB-d läbi
		{
			HeaderA *ptrA = ptrB->pHeaderA;	// HeaderA viit, mis võrdsustatakse HeaderB sees asuva HeaderA-ga
			for (ptrA; ptrA; ptrA = ptrA->pNext) // Kammib HeaderB sees olevad HeaderA-d läbi
			{
				Object1 *ptrObj = (Object1*)ptrA->pObject;
				for (ptrObj = ptrObj; ptrObj; ptrObj = ptrObj->pNext) // Kammib läbi kõik objektid
				{
					printf("%d) %s %lu %s\n", objCnt + 1, ptrObj->pID, ptrObj->Code, ptrObj->pTime);
					objCnt++;
				}
			}
		}
	}
	printf("\n\n\n\n");
}

/*
Kirjeldus: Kasutajale menüüvalikute kuvamine ja valiku sisestuse küsimine.
Parameetrid: puuduvad
Tagastus: option - kasutaja poolt tehtud valik.
*/
int MenuOptions()
{
	int option = 0;
	printf("*** TEGEVUSTE MENYY ***\n\n");
	printf("1 - Kuva k6ik objektid\n");
	printf("2 - Lisa uus objekt\n");
	printf("3 - Eemalda olemasolev objekt\n");
	printf("4 - Esimese osa DEMO\n");
	printf("5 - Teise osa DEMO\n");
	printf("0 - Sulge programm\n\n");
	printf("Vali tegevus, mida soovid teha: \n");
	printf("> ");
	scanf("%d", &option);
	printf("\n");

	return option;
}

/*
Kirjeldus: Kasutajalt objekti identifikaatori ja objekti koodi küsimine.
Parameetrid: **identificator - topeltviit objekti identifikaatori sõnele.
		     *code - viit objekti koodi muutujale.
Tagastus: puudub
*/
void GetUserInput(char **identificator, int *code)
{
	char character[100];	// Puhver kuhu salvestatakse sisestus
	srand((unsigned int)time(NULL));	// Juhusliku arvu jaoks vajalik "seed"

	printf("Sisestage objekti identifikaator: ");
	scanf(" %99[^\n]", character);	// Sisestust küsitakse 99 tähemärgini või kuni 'Enter' klahvi vajutatakse

	(*identificator) = (char*)malloc(sizeof(char) * strlen(character));

	if (identificator == NULL)
	{
		printf("Mälu küsimine eba6nnestus!\n");
		exit(EXIT_FAILURE);
	}

	strcpy(*identificator, character);

	// Kui on antud kaasa väärtus -1, siis ei küsita objekti koodi (Võimaldab sama funktsiooni mitmeks otstarbeks kasutada)
	if (*code != -1)
	{
		printf("Sisestage objekti kood (Juhusliku koodi genereerimine - 0): ");
		scanf("%d", code);
		if (*code == 0)
		{
			unsigned int random_nr = (rand() % 999999999) + 111111111;
			*code = (int)random_nr;
		}
	}
	
}

/*
Kirjeldus: Objekti identifikaatori teise sõna esimese tähe leidmine
Parameetrid: *identificator - viit objekti identifikaatori sõnele.
Tagastus: firstChar - objekti identifikaatori teise sõna esimene täht.
*/
char HeaderAFirstChar(char *identificator)
{
	char* firstChar = strchr(identificator, ' ');
	firstChar++;
	return  *firstChar;
}

/*
Kirjeldus: Objekti identifikaatori nõuetele vastavuse kontroll.
Parameetrid: *identificator - viit objekti identifikaatori sõnele.
Tagastus: 0 - identifikaator pole nõuetele vastav.
		  1 - identifikaator on nõuetele vastav.
*/
int InputCompatibility(char *identificator)
{
	// Kontrollib, kas identifikaatori kahe sõna vahel on tühik
	if (strchr(identificator, ' ') == NULL)
	{
		return 0;
	}
	for (int i = 0; i < strlen(identificator); i++)
	{
		if (identificator[i] == ' ')
		{
			i++;
		}
		else
		{
			if (!isalpha(identificator[i]))
			{
				return 0;
			}
		}
	}

	// Kontrollib, kas identifikaatori mõlemad sõnad algavad suure esitähega
	if (!isupper(identificator[0]) || !isupper(HeaderAFirstChar(identificator)))
	{
		return 0;
	}
	return 1;
}

/*
Kirjeldus: Uue objekti loomine.
Parameetrid: *identificator - viit loodava objekti identifikaatori sõnele.
			 code - loodava objekti kood.
Tagastus: ptrObj - loodud objekt.
		  NULL - objekti ei suudetud luua.
*/
Object1* CreateNewObject(char *identificator, int code)
{
	time_t tm;
	int resBuf = 9;
	Object1 *ptrObj = (Object1*)malloc(sizeof(Object1));
	ptrObj->pID = (char*)malloc(strlen(identificator) + 1);
	strcpy(ptrObj->pID, identificator);
	ptrObj->Code = code;
	ptrObj->pNext = NULL;
	ptrObj->pTime = (char*)malloc(resBuf);
	int err = GetTimeString(time(&tm), resBuf, ptrObj->pTime);
	if (err == 0)
	{
		return NULL;
	}

	return ptrObj;
}

/*
Kirjeldus: Objekti sisestamine andmestruktuuri.
Parameetrid: **pStruct3 - topeltviit lähtestruktuuri esimesele sidujale.
			 *pNewID - viit uue objekti identifikaatori sõnele.
			 newCode - uue objekti kood.
Tagastus: 0 - objekti ei suudetud andmestruktuuri sisestada.
		  1 - objekti sisestamine andmestruktuuri oli edukas.
*/
int InsertNewObject(HeaderB **pStruct3, char *pNewID, int newCode)
{
	// Sisend ei või tühi olla
	if (pStruct3 == NULL || pNewID == NULL || newCode <= 0)
	{
		return 0;
	}
	// Sisend peab olema nõuetele vastav
	if (InputCompatibility(pNewID) == 0)
	{
		return 0;
	}
	HeaderB *tempStruct = *pStruct3;	// Loob HeaderB viida ja võrdsustab sisendiga 
	int cnt1 = 0, cnt2 = 0, pos = 0;	// Vajalikud loendurid ja muutujad

	// Leiab identifikaatori teise sõna esimese tähe positsiooni
	for (int i = 0; i < strlen(pNewID); i++)
	{
		if (pNewID[i] == ' ')
		{
			pos = i + 1;
		}
	}
	while (tempStruct != NULL)	// Otsib kuni HeaderB viit ei anna NULLi
	{
		if (tempStruct->cBegin == pNewID[0])	// Kui sisendi esimene täht kattub tempStructi esimese tähega, alustatakse teise esitähe otsimist
		{
			HeaderA *ptrA = tempStruct->pHeaderA;
			while (ptrA != NULL)	// Sel juhul otsitakse, kuni HeaderA ei anna nulli
			{
				if (ptrA->cBegin == pNewID[pos])	// Kui sisendi teise sõna esimene täht kattub HeaderA esimese tähega, minnakse sinna alamstruktuuri
				{
					Object1 *ptrObj1 = (Object1*)ptrA->pObject;
					while (ptrObj1 != NULL)		// Otsitakse seni, kuni kõik objektid läbi kammitud
					{
						if (strcmp(ptrObj1->pID, pNewID) == 0) // Kui objekti identifikaator kattub sisendiga, siis ei saa seda lisada ja tagastatakse 0
						{
							return 0;
						}
						ptrObj1 = ptrObj1->pNext;	// Järgmine objekt
					}
					Object1 *nObj = CreateNewObject(pNewID, newCode);	// Uue objekti loomine
					if (nObj == NULL)	// Kui loomine ei õnnestu, väljastatakse 0
					{
						return 0;
					}
					nObj->pNext = (Object1*)ptrA->pObject;	// Pannakse viimase olemaoleva objekti viidaks viit uuele objektile
					ptrA->pObject = nObj;	// Lisatakse loodud objekt eelmise viidale
					return 1;	// Lisamine oli edukas
				}
				else if ((ptrA->pNext == NULL) || (ptrA->pNext->cBegin > HeaderAFirstChar(pNewID)))	// Kui HeaderA-d ei leitud, siis tuleb see luua
				{
					HeaderA *insertNewCharacter = (HeaderA*)malloc(sizeof(HeaderA));
					insertNewCharacter->cBegin = HeaderAFirstChar(pNewID);
					insertNewCharacter->pObject = NULL;
					if (cnt1 == 0)	// Esimene HeaderA
					{
						HeaderA *tempA = tempStruct->pHeaderA;
						tempStruct->pHeaderA = insertNewCharacter;
						insertNewCharacter->pNext = tempA;
					}
					else	// Teine või rohkem HeaderA
					{
						insertNewCharacter->pNext = ptrA->pNext;
						ptrA->pNext = insertNewCharacter;
					}
					ptrA = insertNewCharacter;
				}
				else
				{
					ptrA = ptrA->pNext; // Kui selline täht on olemas, siis läheb lihtsalt järgmiseks
				}
				cnt1++; // Järgmine HeaderA
			}
		}
		else if ((tempStruct->pNext == NULL) || (tempStruct->pNext->cBegin > pNewID[0])) // Kui ei ole sellele sisendile HeaderB-d, siis tuleb luua see kõigepealt
		{
			HeaderB *insertB = (HeaderB*)malloc(sizeof(HeaderB));
			insertB->cBegin = pNewID[0];
			insertB->pHeaderA = (HeaderA*)malloc(sizeof(HeaderA));
			insertB->pHeaderA->cBegin = HeaderAFirstChar(pNewID);
			insertB->pHeaderA->pObject = NULL;
			insertB->pHeaderA->pNext = NULL;
			if (cnt2 == 0) // Esimene HeaderB
			{
				HeaderB *tempB = *pStruct3;
				*pStruct3 = insertB;
				insertB->pNext = tempB;
			}
			else	// Teine või enam HeaderB
			{
				insertB->pNext = tempStruct->pNext;
				tempStruct->pNext = insertB;
			}
			tempStruct = insertB;
		}
		else
		{
			tempStruct = tempStruct->pNext;
		}
		cnt2++; // Järgmine HeaderB
	}
	return 0;
}

/*
Kirjeldus: Olemasoleva objekti eemaldamine andmestruktuurist (ilma objekti ennast kustutamata).
Parameetrid: **pStruct3 - topeltviit lähtestruktuuri esimesele sidujale.
			 *pExistingID - viit olemasoleva objekti identifikaatori sõnele.
Tagastus: ptrReturn - viit eemaldatud objektile.
		  NULL - objekti ei õnnestunud eemaldada.
*/
Object1* RemoveExistingObject(HeaderB** pStruct3, char* pExistingID)
{
	Object1* ptrReturn = NULL; // Funktsiooni tagastuse deklaratsioon

	// Sisendid ei tohi tühjad olla
	if (pStruct3 == NULL || pExistingID == NULL)
	{
		return ptrReturn;
	}
	// Sisestus peab vastama nõuetele
	if (InputCompatibility(pExistingID) == 0)
	{
		return ptrReturn;
	}

	// Viitade deklareerimine
	HeaderB* ptrB = *pStruct3;
	HeaderB* lastB = NULL;
	HeaderA* ptrA = NULL;
	Object1* ptrObj = NULL;
	//Object1* lastObject = NULL;

	while (ptrB != NULL)	// HeaderB-sid kammitakse läbi, kuni jõutakse viimaseni
	{
		if (ptrB->cBegin == pExistingID[0])
		{
			HeaderA* lastA = (HeaderA*)ptrB->pHeaderA;
			lastA =NULL;
			ptrA = (HeaderA*)ptrB->pHeaderA;
			while (ptrA != NULL)	// HeaderA-sid kammitakse läbi, kuni jõutakse viimaseni
			{
				if (ptrA->cBegin == HeaderAFirstChar(pExistingID))
				{
					Object1* lastObject = (Object1*)ptrA->pObject;
					lastObject = NULL;
					ptrObj = (Object1*)ptrA->pObject;
					while (ptrObj != NULL)	// Objekte kammitakse läbi, kuni jõutakse viimaseni
					{
						if (strcmp(pExistingID, ptrObj->pID) == 0)
						{
							ptrReturn = ptrObj;	// Tagastuseks on see objekt, mis sai eemaldatud
							if (lastObject != NULL) 
							{
								lastObject->pNext = ptrObj->pNext;
							}
							else 
							{
								ptrA->pObject = ptrObj->pNext;
							}
							return ptrReturn;
						}
						lastObject = ptrObj;
						ptrObj = ptrObj->pNext;	// Järgmine objekt
					}
					return ptrReturn;
				}
				lastA = ptrA;
				ptrA = ptrA->pNext;	//Jägmine HeaderA
			}
		}
		lastB = ptrB;
		ptrB = (HeaderB*)ptrB->pNext;	// Järgmine HeaderB
	}
	return ptrReturn;	// Kui ei leitud midagi, siis tagastatakse NULL
}

/*
Kirjeldus: Luuakse uue kahendpuu.
Parameetrid: pStruct3 - peamine struktuur, mis sisaldab objekte.
Tagastus: Loodud kahendpuu juur.
*/ 
Node* CreateBinaryTree(HeaderB* pStruct3)
{
	// Algväärtustan juure esmalt NULLiga
	Node* root = NULL;
	HeaderB* ptrB = pStruct3;
	for (ptrB; ptrB; ptrB = ptrB->pNext)
	{
		HeaderA* ptrA = (HeaderA*)ptrB->pHeaderA;
		for (ptrA; ptrA; ptrA = ptrA->pNext)
		{
			Object1* ptrObj = (Object1*)ptrA->pObject;
			for (Object1* d = ptrObj; d; d = d->pNext)
			{
				// Allocate memory for a new node
				Node* temp = (Node*)malloc(sizeof(Node));
				if (!temp)
				{
					printf("Memory allocation failed.\n");
					exit(EXIT_FAILURE); // Mälu eraldamise tõrge
				}

				temp->pObject = d;
				temp->pLeft = NULL;
				temp->pRight = NULL;

				unsigned long int key = d->Code;

				Node* ptr = root;
				Node* parent = NULL;

				// Leian objektile õige koha kahendpuus
				while (ptr != NULL)
				{
					parent = ptr;
					if (key < ((Object5*)ptr->pObject)->Code)
						ptr = ptr->pLeft;
					else if (key > ((Object5*)ptr->pObject)->Code)
						ptr = ptr->pRight;
					else
					{
						printf("Object with this code is already added to the tree.\n");
						free(temp); // Vabastan mälu
						return root;
					}
				}

				// Uuendan vanema viida
				if (parent == NULL)
					root = temp;
				else if (key < ((Object5*)parent->pObject)->Code)
					parent->pLeft = temp;
				else
					parent->pRight = temp;
			}
		}
	}
	return root;
}

/*
Kirjeldus: Lisab objekti pStacki otsa.
Parameetrid: ptrStack - olemasolev kuhi, ptrObj - lisatav objekt.
Tagastus: Uus kuhi koos lisatud objektiga.
*/
Stack* Push(Stack* ptrStack, void* ptrObj)
{
	errno = 0;
	Stack* pNew;

	if (!ptrObj)
	{
		errno = EINVAL;
		return ptrStack;
	}

	pNew = (Stack*)malloc(sizeof(Stack));
	if (!pNew)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE); // Malloc ebaõnnestus
	}

	pNew->pObject = ptrObj;
	pNew->pNext = ptrStack;

	return pNew;
}

/*
Kirjeldus: Eemaldab objekti kuhjast.
Parameetrid: ptrStack - olemasolev kuhi, ptrObj - eemaldatav objekt.
Tagastus: Uus kuhi ilma eemaldatud objektita.
*/
Stack* Pop(Stack* ptrStack, void** ptrObj)
{
	Stack* ptr;

	if (!ptrStack)
	{
		*ptrObj = NULL;
		return ptrStack;
	}

	*ptrObj = ptrStack->pObject;
	ptr = ptrStack->pNext;
	free(ptrStack);

	return ptr;
}

/*
Kirjeldus: Puu sümmeetriline läbikäik.
Parameetrid: pTree - kahendpuu juur.
Tagastus: puudub.
*/
void TreeTraversal(Node* pTree)
{
	int objCnt = 0;
	Stack* pStack = NULL;
	Node* ptr = pTree;

	if (ptr == NULL)
	{
		printf("Puu on tühi\n");
		return;
	}

	while (ptr != NULL || pStack != NULL)
	{
		while (ptr != NULL)
		{
			pStack = Push(pStack, ptr);
			ptr = ptr->pLeft;
		}

		if (pStack != NULL)
		{
			ptr = (Node*)pStack->pObject;
			pStack = Pop(pStack, (void**)&ptr);
			Object1* Obj = (Object1*)ptr->pObject;

			printf("%d) %s %lu %s\n", objCnt + 1, Obj->pID, Obj->Code, Obj->pTime);
			objCnt++;

			ptr = ptr->pRight;
		}
	}
	printf("\n");
}

/*
Kirjeldus: Kontrollib, kas antud koodiga objekt eksisteerib struktuuris pStruct3.
Parameetrid: pStruct3 - peamine struktuur, c - kontrollitav kood.
Tagastus: True, kui objekt eksisteerib; False, kui objekt puudub.
*/
bool DoesCodeExist(HeaderB* pStruct3, unsigned long int c)
{
	HeaderB* ptrB = pStruct3;
	for (ptrB; ptrB; ptrB = ptrB->pNext)
	{
		HeaderA* ptrA = (HeaderA*)ptrB->pHeaderA;
		for (ptrA; ptrA; ptrA = ptrA->pNext)
		{
			Object1* ptrObj1 = (Object1*)ptrA->pObject;

			for (Object1* d = ptrObj1; d; d = d->pNext)
			{
				if (d->Code == c)
				{
					return true;
				}
					
			}
		}
	}
	
	return false;
}

/*
Kirjeldus: Eemaldab kahendpuust objekti, millel on antud kood.
Parameetrid: pTree - kahendpuu juur, Code - kustutatava objekti kood.
Tagastus: Uuendatud kahendpuu juur.
*/
Node* DeleteTreeNode(Node* pTree, unsigned long int Code) {
	Node* current = pTree;
	Node* parent = NULL;
	Node* child = pTree;

	// Leian kustutatava lehe
	while (current != NULL && ((Object1*)current->pObject)->Code != Code) {
		parent = current;
		if (Code < ((Object1*)current->pObject)->Code)
			current = current->pLeft;
		else
			current = current->pRight;
	}

	// Lehel on kaks tütart
	if (current->pLeft != NULL && current->pRight != NULL) {
		Node* successorParent = current;
		Node* successor = current->pRight;

		// Kõige vasakpoolsem puu leht
		while (successor->pLeft != NULL) {
			successorParent = successor;
			successor = successor->pLeft;
		}

		// Asendan praeguse lehe andmed tütre andmetega
		((Object1*)current->pObject)->Code = ((Object1*)successor->pObject)->Code;
		((Object1*)current->pObject)->pID = ((Object1*)successor->pObject)->pID;

		current = successor;
		parent = successorParent;
	}
	// Vanemal on kõige rohkem 1 laps
	child = (current->pLeft != NULL) ? current->pLeft : current->pRight;

	if (parent == NULL)
	{
		pTree = child;
	}
	else if (current == parent->pLeft)
	{
		parent->pLeft = child;
	}
	else
	{
		parent->pRight = child;
	}
	free(current);
	return pTree;
}