#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ncurses.h>

typedef struct punct
{
    int x,y;
}punct;

/*
Functie care afisaeza tabla de joc a jucatorului
si tabla de joc a calculatorului.
*/

void afisare(int tabla_joc[14][14],int tabla_player[14][14])
{
    int i,j,k;
    printf("   ");
    for(i=1;i<=10;i++)
        printf("%d ",i);
    printf("          ");
    printf("  ");
    for(i=1;i<=10;i++)
        printf("%d ",i);
    printf("\n");
    printf("  ");
    for(i=0;i<=20;i++)
        printf("-");
    printf("          ");
    printf("  ");
    for(i=0;i<=20;i++)
        printf("-");
    printf("\n");
    for(i=1;i<=10;i++)
    {
        for(j=1;j<=10;j++)
        {
            if(j==1 && i!=10)
                printf("%d |",i);
            if(j==1 && i==10)
                printf("%d|",i);
            if(tabla_joc[i][j]==0)
                printf("-|");
            if(tabla_joc[i][j]==-1)
                printf("@|");
            if(tabla_joc[i][j]==2)
                printf("x|");
        }
        printf("          ");
        for(k=1;k<=10;k++)
        {
            if(k==1 && i!=10)
                printf("%d |",i);
            if(k==1 && i==10)
                printf("%d|",i);
            if(tabla_player[i][k]==0)
                printf("-|");
            if(tabla_player[i][k]==-1)
                printf("@|");
            if(tabla_player[i][k]==2)
                printf("x|");
            if(tabla_player[i][k]==1)
                printf("#|");
        }
        printf("\n");
    }
}

void init_2(int tabla_player[14][14],char fisier[])
{
    int i=1,j=1,k,n=0,bec;
    char linie[100];			
    FILE *f;
    f=fopen(fisier,"r");
    while(fgets(linie,100,f)!=NULL)
    {
        n=strlen(linie);
        for(k=0;k<n;k++)
        {
            if(linie[k]=='-')
            {
                bec=1;
                if(j>10)
                    bec=0;
                if(bec==0)
                {
                    i++;
                    j=1;
                }
                tabla_player[i][j]=0;
                j++;
            }
            if(linie[k]=='#')
            {
                bec=1;
                if(j>10)
                    bec=0;
                if(bec==0)
                {
                    i++;
                    j=1;
                }
                tabla_player[i][j]=1;
                j++;
            }
        }
    }
    fclose(f);
}

/*
Functie care initializeaza matricea cu 0.
*/
void init(int tabla_joc[14][14])  // functie de initializare matrice
{
    int n=10,i,j;
    for(i=1;i<=n;i++)
        for(j=1;j<=n;j++)
            tabla_joc[i][j]=0;
}

/*
Functie care bordeza o matrice cu -1.
*/
void bordare_matrice(int tabla_joc[14][14]) // bordam matricea cu -1
{
    int i;
    for(i=0;i<=11;i++)
    {
        tabla_joc[0][i]=-1;
        tabla_joc[11][i]=-1;
    }

    for(i=0;i<=11;i++)
    {
        tabla_joc[i][0]=-1;
        tabla_joc[i][11]=-1;
    }
}

/*
Generam intai o valoare din multimea {0,1}.
In cazul in care valoarea este 0 vom considera pozitie orizontala.
In cazul in care valoarea este 1 vom considera pozitie verticala.
Generam un punct random pe harta.
Verificam in functie de tipul pozitionarii navei(orizontala sau verticala) daca
putem plasa acea nava in acel loc(chenarul exterior al acelei nave(matrice) nu trebuie sa
contina decat 0 si -1(din bordare). In cazul in care contine si altceva inseamna ca mai exista o nava,
motiv pentru care vom genera un alt punct,pana cand vor fi satisfacute conditiile.
Pozitionare navelor se incepe de la nava de lungime maxima(4) pentru a evita cazurile in care
nu mai este loc in matrice pentru pozitionarea unei nave;
*/
void generare_1234(int tabla_joc[14][14],int dim) // functie care genereaza o nava pt o dim data
{
    int random,bec=0,bec2=0,k,p,i;
    punct a;
    random=rand()%2;
    while(!bec)
    {
        a.x=rand()%10+1;
        a.y=rand()%10+1;
        if(random==0)
        {
            bec=1;
            bec2=1;
            for(k=a.y;k<=a.y+dim-1;k++)
                if(tabla_joc[a.x][k]!=0)
                {
                    bec2=0;
                    bec=0;
                    break;
                }
            if(bec2==1)
            {
                for(k=a.x-1;k<=a.x+1;k++)
                    for(p=a.y-1;p<=a.y+dim;p++)
                        if(tabla_joc[k][p]>0)
                        {
                            bec=0;
                            break;
                        }
                if(bec==1)
                    for(i=a.y;i<=a.y+dim-1;i++)
                        tabla_joc[a.x][i]=1;
            }
        }
        else
        {
            bec=1;
            bec2=1;
            for(k=a.x;k<=a.x+dim-1;k++)
                if(tabla_joc[k][a.y]!=0)
                {
                    bec2=0;
                    bec=0;
                    break;
                }
            if(bec2==1)
            {
                for(k=a.x-1;k<=a.x+dim;k++)
                    for(p=a.y-1;p<=a.y+1;p++)
                        if(tabla_joc[k][p]>0)
                        {
                            bec=0;
                            break;
                        }
                if(bec==1)
                    for(i=a.x;i<=a.x+dim-1;i++)
                        tabla_joc[i][a.y]=1;
            }
        }
    }
}

/*
Aceasta functie genereaza navele prin apeluri succesive ale functie generare_1234
*/
void generare_nave(int tabla_joc[14][14])
{
    int i;
    generare_1234(tabla_joc,4);
    for(i=1;i<=2;i++)
        generare_1234(tabla_joc,3);
    for(i=1;i<=3;i++)
        generare_1234(tabla_joc,2);
    for(i=1;i<=4;i++)
        generare_1234(tabla_joc,1);
}
/*
Verifica daca mai exista vreo nava(mai exista vreun element a matricei cu valoarea 1).
*/
int exista_nava(int tabla_joc[14][14])
{
    int i,j;
    for(i=1;i<=10;i++)
        for(j=1;j<=10;j++)
            if(tabla_joc[i][j]==1)
                return 1;
    return 0;
}

/*
Aceasta functie cauta toate punctele lovite de invins(toate valorile de 2 din matrice).
In momentul in care gaseste un punct cautam in toate cele patru directii
(stanga,dreapta,sus,jos). Daca nu gasim vreun punct cu valoarea 1 inseamna ca nava a fost distrusa complet.
Sa presupunem ca avem o nava de 3 elemnte (linia 3 si coloanele 4,5,6) complet distrusa(numai valori de 2)
atunci algoritmul ar putea considera ca exista 3 nave distruse.Pentru a evita aceasta problema fiecare loc prin
care am trecut il vom reinitializa cu -1(doar daca are valoarea 2, orice alt punct va ramane neschimbat).
Pentru a nu modifica matricea initiala vom face o copie a acesteia.
*/
int nave_distruse(int tabla[14][14])
{
    int i,j,x,y;
    int cnt=0,bec;
    int copie[14][14];
    for(i=0;i<=11;i++)                  //copiem matricea cu tot cu bordarea de -1
        for(j=0;j<=11;j++)
            copie[i][j]=tabla[i][j];
    for(i=1;i<=10;i++)
        for(j=1;j<=10;j++)
        {
            bec=1;
            if(copie[i][j]==2)
            {
                x=i;
                y=j+1;
                while(copie[x][y]!=-1 && copie[x][y]!=0)  //verificam la dreapta
                {
                    if(copie[x][y]==1)
                    {
                        bec=0;
                        break;
                    }
                    copie[x][y]=-1;
                    y++;
                }
                x=i;
                y=j-1;
                while(copie[x][y]!=-1 && bec==1 && copie[x][y]!=0)  //verificam la stanga
                {
                    if(copie[x][y]==1)
                    {
                        bec=0;
                        break;
                    }
                    copie[x][y]=-1;
                    y--;
                }
                x=i-1;
                y=j;
                while(copie[x][y]!=-1 && bec==1 && copie[x][y]!=0) //verificam in sus
                {
                    if(copie[x][y]==1)
                    {
                        bec=0;
                        break;
                    }
                    copie[x][y]=-1;
                    x--;
                }
                x=i+1;
                y=j;
                while(copie[x][y]!=-1 && bec==1 && copie[x][y]!=0)  //verificam in jos
                {
                    if(copie[x][y]==1)
                    {
                        bec=0;
                        break;
                    }
                    copie[x][y]=-1;
                    x++;
                }
                if(bec==1)
                    cnt++;
            }
        }
    return cnt;
}

/*
Functia play se ocupa de gamplay ul jocului.
Aceasta modifica matricea in momentu in care lovim un punct care apartine si nu apartine navei.
Ne atentioneaza atunci cand am introdus inca o data un punct deja lovit.
*/

void play(int tabla_joc[14][14],int tabla_player[14][14],int tabla_lovire[14][14])
{
    int terminat=0,player=0,computer=0,bec,x,y;
    punct a;
    while(!terminat)
    {
        if(player==0)
        {
            bec=1;
            scanf("%d%d",&x,&y);
            while(bec)
            {
                if(tabla_joc[x][y]==1)
                {
                    tabla_joc[x][y]=2;
                    tabla_lovire[x][y]=2;
                    afisare(tabla_lovire,tabla_player);
                    printf("Ai lovit un punct din nava\n\n");
                    if(exista_nava(tabla_joc))
                        scanf("%d%d",&x,&y);
                    else
                    {
                        bec=0;
                        break;
                    }
                }
                if(tabla_joc[x][y]==0)
                {
                    bec=0;
                    tabla_joc[x][y]=-1;
                    tabla_lovire[x][y]=-1;
                    afisare(tabla_lovire,tabla_player);
                    printf("Nu ai lovit un punct din nava\n\n");
                    break;
                }
                if(tabla_joc[x][y]==-1 || tabla_joc[x][y]==2)
                {
                    printf("Ai lovit deja acest punct\n\n");
                    if(exista_nava(tabla_joc))
                        scanf("%d%d",&x,&y);
                    else
                    {
                        bec=0;
                        break;
                    }
                }
            }
        }

        if(!exista_nava(tabla_joc))
        {
            printf("\n\n\n\n\nAi castigat\n");
            printf("Calcuatorul a distrus %d nave\n",nave_distruse(tabla_player));
            terminat=1;
            break;
        }


        if(computer==0)
        {
            bec=1;
            a.x=rand()%10+1;
            a.y=rand()%10+1;
            while(bec)
            {
                if(tabla_player[a.x][a.y]==1)
                {
                    tabla_player[a.x][a.y]=2;
                    afisare(tabla_lovire,tabla_player);
                    printf("Calculatorul a lovit un punct din nava\n\n");
                    if(exista_nava(tabla_player))
                    {
                        a.x=rand()%10+1;
                        a.y=rand()%10+1;
                    }
                    else
                    {
                        bec=0;
                        break;
                    }
                }
                if(tabla_player[a.x][a.y]==0)
                {
                    bec=0;
                    tabla_player[a.x][a.y]=-1;
                    afisare(tabla_lovire,tabla_player);
                    printf("Calculatorul nu a lovit nava\n\n");
                    break;
                }
                if(tabla_player[a.x][a.y]==-1 || tabla_player[a.x][a.y]==2)
                {
                    if(exista_nava(tabla_player))
                    {
                        a.x=rand()%10+1;
                        a.y=rand()%10+1;
                    }
                    else
                    {
                        bec=0;
                        break;
                    }
                }
            }
        }
        if(!exista_nava(tabla_player))
        {
            printf("\n\n\n\n\n\n\nCalculatorul a castigat\n");
            printf("Jucatorul i-a distrus calculatorului %d nave.\n",nave_distruse(tabla_joc));
            terminat=1;
            break;
        }
    }
}

int main(int argc,char **argv)
{
    int tabla_joc[14][14],tabla_player[14][14],tabla_lovire[14][14],bec,opt;
    bec=1;
    if(argc-1==0)
        printf("[Eroare]:Nu s-au dat argumente de comanda\n");
    else
    {
        while(bec)
        {
            printf("1.START\n");
            printf("2.QUIT\n");
            printf("3.INFO\n");
            printf("Selectati optiunea:");
            scanf("%d",&opt);
            switch(opt)
            {
                case 1:
                    {
                        srand(time(NULL));
                        init(tabla_joc);
                        init(tabla_lovire);
                        init_2(tabla_player,argv[1]);
                        bordare_matrice(tabla_joc);
                        bordare_matrice(tabla_player);
                        bordare_matrice(tabla_lovire);
                        generare_nave(tabla_joc);
                        afisare(tabla_lovire,tabla_player);
                        play(tabla_joc,tabla_player,tabla_lovire);
                        break;
                    }
                case 2:
                    {
                        bec=0;
                        break;
                    }
                case 3:
                    {
                        printf("\nLegenda semne joc:\n");
                        printf("@-punctul atacat nu este parte dintr-o nava.\n");
                        printf("x-punctul atacat face parte dintr-o nava.\n");
                        printf("Navele din tabla jucatorului sunt reprezentate prin #.\n");
                        printf("Alegerea unui punct din matrice se realizeaza prin citirea acestuia(linie si coloana).\n");
                        printf("- reprezinta un spatiu gol\n\n");
                        break;
                    }
                default:
                    {
                        printf("Ati introdus o optiune inexistenta.\n\n");
                        break;
                    }
            }
        }
    }
    return 1;
}