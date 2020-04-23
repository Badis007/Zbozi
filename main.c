#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SOUBOR "zbozi.txt"
#define VYSTUP "index.html"
#define VELIKOST 100
#define ODDELOVAC ";\n"
void vymena (char * nazev){
    int i;
    for(i=0;i<strlen(nazev);i++){//Funkce nahradí pomlčky mezerama
        if(nazev[i]=='_')
            nazev[i]=' ';
    }
    return;
}
int main(int argc, char** argv) {
    typedef struct{	//STRUKTURA 
        char dodavatel[20];
        char nazev_zbozi[30];
        int hmotnost;
        int pocet;
        float cena;
    }ZBOZI;
  
  
    FILE * pFile; //NAČTENÍ SOUBORA
    ZBOZI *zbozi=NULL; //ADRESA NA STRUKTURU -> MUSÍM POUŽÍVAT ZBOZI S MALYM
    int pocet=0;
    int pocetSloupcu;
    char *pch;
    float cenaCelkem=0;
    int hmotnostCelkem=0;
    int max=0;
    int index=0;
    char mystring [VELIKOST];
    int i;
  
    if ((pFile = fopen(SOUBOR,"r")) == NULL){ //OTEVŘENÍ SOUBOROSE
        printf("Nebyl otevren vas soubor %s. \n",SOUBOR);
        return EXIT_FAILURE;
    }
  
    while(fgets (mystring , VELIKOST , pFile) != NULL ){ //FGETS -> ZÍSKÁ ZNAKY ZE SOUBORU, ULOŽÍ DO PROMĚNNÉ mystring->pole, VELIKOST->velikost pole, pfile->z jakého souboru, bude to načítat dokud tam nic nebude
        if(pocet >0){ //Skipnutí prvního řádku, nechem tam názvy.
            zbozi = (ZBOZI*) realloc (zbozi, pocet * sizeof(ZBOZI)); //Vytvoření pole do proměnné zbozi
            pocetSloupcu=1; 
            pch = strtok (mystring,ODDELOVAC); //Ze stringu odstraní oddělovače a načte do proměnné pch
            while (pch != NULL){ //Dokud tam něco je tak se opakuje cyklus
                switch(pocetSloupcu){ //PocetSloupcu -> ridici promenna 
                    case 1: //První sloupec -> nactu do neho udaje v prvnim sloupci jde o dodavatele
                        strcpy(zbozi[pocet-1].dodavatel,pch);//strcpy -> pro kopírování stringových hodnot, zbozi-> do neho kopiruju hodnoty z pch-> zaznamy bez oddelovacu.
                        break;
                    case 2:
                        strcpy(zbozi[pocet-1].nazev_zbozi,pch);
                        break;
                    case 3:
                        zbozi[pocet-1].hmotnost=atoi(pch);//ATOI dělá ze stringového čísla napr. "12" normální intové číslo 12.
                        break;
                    case 4:
                        zbozi[pocet-1].pocet=atoi(pch);
                        break;
                    case 5:
                        zbozi[pocet-1].cena=atof(pch);//ATOF dělá ze stringového čísla napr. "12.5" normální float číslo 12.5
                        break;
                }
                pch = strtok (NULL,ODDELOVAC); //IDK MUSÍ TO TAM BÝT
                pocetSloupcu++;
            }
        }
        pocet++; //Posunutí na další řádek
    }
    
    if(fclose(pFile)==EOF){ //ZAVŘENÍ SOUBORU BO UŽ JE K PIČI
        printf("Nebyl zavren vas soubor %s. \n",SOUBOR);
    }
  
    printf("U C T E N K A\n");//VÁPIS DO TERMINÁLU
    printf("dodavatel     nazev zbozi    hmotnost    pocet   cena     cena\n");
    printf("(zkratka)                     (1 kusu)    kusu  (1 kusu)  celkem\n");
    printf("--------------------------------------------------------------------\n");
    pocet--; //Skipli jsme prvni radek, musime jit o krok zpet
  
    for(i=0;i<pocet;i++){ //Pocet -> pocet radku -> cyklus pojede dokud tam budou radky
        cenaCelkem += zbozi[i].pocet * zbozi[i].cena; //Násobení počtu a ceny na každém řádku a sečtení dohromady
        hmotnostCelkem += zbozi[i].pocet * zbozi[i].hmotnost;//To samé 
        vymena(zbozi[i].nazev_zbozi);//Vyvolání funkce, která nahradí pomlčky mezerama. zbozi s indexem i je pole se všemy názvy zboží.
      
        printf("   %s %20s %10d %6d %8.2f Kc %8.2f Kc \n",zbozi[i].dodavatel, zbozi[i].nazev_zbozi, 
                zbozi[i].hmotnost, zbozi[i].pocet, zbozi[i].cena, cenaCelkem);//CELKOVÝ VÝPIS 
        if(zbozi[i].cena > max){ //Do max nacitam zbozi a porovnavam s kazdym ktere prijde dal, aby naslo nejdrazsi 
            max=zbozi[i].cena;
            index=i; //aby jsme vedeli jake zbozi je nejdrazsi 
        }
    }
  
    printf("\nCena celkem: %.2f Kc \n", cenaCelkem); //Výpisy
    printf("Pocet polozek: %d \n",pocet);
    printf("Celkova hmotnost zbozi: %d kg %d g \n", hmotnostCelkem/1000, hmotnostCelkem%1000);
    printf("Nejdrazsi nakoupene zbozi: %s za %.2f Kc \n",zbozi[index].nazev_zbozi,zbozi[index].cena);
    
    if ((pFile = fopen(VYSTUP,"w")) == NULL){ //Otevření souboru na zápis
        printf("Nebyl otevren vas soubor %s. \n",VYSTUP);
        return EXIT_FAILURE;
    }
  
    fprintf(pFile,"<h1>Zbozi od externich dodavatelu</h1>");
    fprintf(pFile,"<table><tr>");
    fprintf(pFile,"<th>dodavatel</th><th>nazev</th><th>hmotnost v gramech</th><th>cena 1 kusu v Kc</th>");
    for(i=0;i<pocet;i++){ //Načtení do HTML tabluky
        if(strcmp(zbozi[i].dodavatel,"OP")!=0){ //Nevypise dodavatele s "OP"
            fprintf(pFile,"</tr><tr>");
            fprintf(pFile,"<td> %s </td> <td> %s </td><td>%d</td><td>%.2f</td>",zbozi[i].dodavatel, 
                    zbozi[i].nazev_zbozi, zbozi[i].hmotnost,zbozi[i].cena);
            fprintf(pFile,"</tr>");
        }
    }
    
    if(fclose(pFile)==EOF){ //Zavření souboru do kterého jsme zapsali
        printf("Nebyl zavren vas soubor %s. \n",VYSTUP);
    }else{
        printf("\nByl vytvoren soubor %s \n",VYSTUP);
    }
    
    return (EXIT_SUCCESS);
}

