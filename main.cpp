#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <windows.h>
using namespace std;

/// Secvente aduse pentru calcularea timpului
/// Timpul in microsecunde
double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
    cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart)/1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}

void radix_sort(int v[],int nr)
{
    vector <int> bucket[2][10]; // Avem o "dimensiune" deja ca vetor
                                // Folosim un vector auxiliar pt a introduce
                                // numerele sortate
    int maxi=0,cifre_max=0;
    for(int i=0;i<nr;i++) // Pas 1: introducem numerele intr-un vector si cautam si nr maxim
        {
            bucket[0][v[i]%10].push_back(v[i]); // Prima introducere
            if(v[i]>maxi)
                maxi=v[i];
        }

    while(maxi!=0) // Pas 2: gasim numarul de cifre ale numarului maxim
    {
        cifre_max++;
        maxi/=10;
    }

    int p=10;
    int pas=0; // Pas 3: pt vector auxiliar care se interschimba cu vector principal
    for(int k=0;k<cifre_max;k++) // Trebuie sa facem maxim cifre_max sortari
    {
        for(int i=0;i<10;i++) // Bucket-ul e de 10
            for(int j=0;j<bucket[pas][i].size();j++)
                bucket[1-pas][bucket[pas][i][j]/p%10].push_back(bucket[pas][i][j]);
        for(int i=0;i<10;i++)
            bucket[pas][i].clear();
        pas=1-pas;
        p*=10;
    }
    int poz=0;
    for(int i=0;i<10;i++)
        for(int j=0;j<bucket[pas][i].size();j++)
            v[poz++]=bucket[pas][i][j];

}

void countingSort(int v[],int n)
{   int maxim=v[0],minim=v[0];
    for(int i=0;i<n;i++)
        if (maxim<v[i])
        maxim=v[i];
        else if (minim>v[i])
        minim=v[i];

    int v_fr[maxim-minim+1]={0}; // dimensiunea vectorului e corecta
    for(int i=0;i<n;i++)
        v_fr[v[i]-minim]++; // vectorul de frecventa aloca poz[0] fiind minimul corect
    int poz=0;
    for(int i=0;i<maxim-minim+1;i++)
    for(int j=0;j<v_fr[i];j++)
        v[poz++]=minim+i;
}

void bubble_sort(int v[],int nr)
{
int sem=0,aux;
while(sem==0)
{
    sem=1;
    for (int i=0;i<nr-1;i++)
        if (v[i]>v[i+1])
    {
     sem=0;
     aux=v[i];
     v[i]=v[i+1];
     v[i+1]=aux;
    }

}

}


void interclasare(int v[],int st,int dr)
{
int i,j,k,mij=(st+dr)/2,l1=mij-st+1,l2=dr-mij;
int arr1[l1],arr2[l2];
for(i=0;i<l1;i++)
    arr1[i]=v[st+i];
for(j=0;j<l2;j++)
    arr2[j]=v[mij+j+1];

i=0;j=0;k=st;
while(i<l1 && j<l2)
    if (arr1[i]>arr2[j])
        v[k++]=arr2[j++];
    else
        v[k++]=arr1[i++];

while(i<l1)
    v[k++]=arr1[i++];
while(j<l2)
    v[k++]=arr2[j++];

}

void merge_sort(int v[],int st,int dr)
{
    if (st<dr)
    {
        merge_sort(v,st,(st+dr)/2);
        merge_sort(v,(st+dr)/2+1,dr);
        interclasare(v,st,dr);
    }
}

void mrg(int v[],int nr)
{
    merge_sort(v,0,nr-1);
}

int partitie(int v[],int inceput,int sfarsit)
{
    int pivot=v[inceput],retine=inceput;
    while(inceput<sfarsit)
    {
        while(v[inceput]<=pivot)
            inceput++;
        while(v[sfarsit]>pivot)
            sfarsit--;
        if (inceput<sfarsit)
                swap(v[inceput],v[sfarsit]);
    }
    swap(v[retine],v[sfarsit]);
    return sfarsit;
}

void quick_sort(int v[],int st,int dr)
{
    if (st<dr)
    {
        int poz_pivot=partitie(v,st,dr); //trebuie sa aleg pivotul altfel
        quick_sort(v,st,poz_pivot-1);
        quick_sort(v,poz_pivot+1,dr);
    }
}

void qck(int v[],int nr)
{
    quick_sort(v,0,nr-1);
}

void analyze(void (*func)(int v[],int nr),int ve[],int nre)
{
    int vAux[100000],nrAux=0; // Luam un vector provizoriu astfel incat sa nu modificam vectorul initial
    while(nrAux<nre)
    {
        vAux[nrAux]=ve[nrAux];
        nrAux++;
    }
    StartCounter();
    func(ve,nre);
    double timp=GetCounter();
    sort(vAux,(vAux+nre));  // Sortam cu sortare nativa sa ne asiguram ca este bine sortat
     int ok=1;
     for(int i=0;i<=nre;i++)
       if (ve[i]!=vAux[i]) // Pasul de verificare daca vectorul e bine sortat
            ok=0;
    if (ok==1)
    std::cout << "Succes in " << timp << " microsecunde."<<'\n';
    else
    cout << "!!! Ceva nu a mers bine !!!"<<'\n';
}

void special_analyze()
{

}

int main()
{
    int v[100000];
    int nr=0,aux,mx=0,nrFisiere=4;
    vector<string> numeFisiere;
    numeFisiere.push_back("PutineMici.txt");
    numeFisiere.push_back("PutineMari.txt");
    numeFisiere.push_back("MulteMici.txt");
    numeFisiere.push_back("MulteMari.txt");
    for(int i=0;i<nrFisiere;i++)
    {
    cout<<numeFisiere[i]<<'\n';
    ifstream f(numeFisiere[i]);
    while(f>>aux)
    {
        v[nr]=aux;
        // Retinem maximul si numarul de numere pentru a nu sorta in cazuri speciale, precum bubble sau counting
        if (aux>mx)
            mx=aux;
        nr++;
    }

    if (nr<25000) {cout<<"Bubble: "; analyze(bubble_sort,v,nr);}
    else cout<<"Fara bubbleSort"<<'\n';
    if (mx<1000000) {cout<<"Counting: "; analyze(countingSort,v,nr);}
    else cout<<"Fara countingSort"<<'\n';
    cout<<"Radix: "; analyze(radix_sort,v,nr);
    cout<<"Quick: "; analyze(qck,v,nr);
    cout<<"Merge: "; analyze(mrg,v,nr);

    // Acum putem sorta direct cu sortarea nativa vectorul citit
    double timp_sortare_nativa;
    StartCounter();
    sort(v,v+nr);
    timp_sortare_nativa=GetCounter();

    cout<<"SortareNativa: Succes in " << timp_sortare_nativa << " microsecunde."<<'\n';
    cout<<'\n';
    f.close();
    }
// MulteMici 60.000 de nr cuprinse intre 1 si 100
// MulteMari 30.000 de nr cuprinse intre 1 si 100.000.000

    return 0;
}
