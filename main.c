// Operacoes heap de Fibonacci
// @author J Seabra - 20220616

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"

//Cria e retorna um heap de Fibonacci
//MAKE-FIB-HEAP (CORMEM)
FIB_HEAP *criaHeap() 
{
	FIB_HEAP *heap;
	heap = (FIB_HEAP *)malloc(sizeof(FIB_HEAP)); 
	heap->num = 0;
	heap->min = NULL;
	
	return heap;
}

//Insere um novo noh no heap de Fibonacci
//FIB-HEAP-INSERT (CORMEM)
void insercao(FIB_HEAP *heap, NOH *novoNo, int valor) 
{
	//Verifica se heap existe
	if (heap == NULL) 
	{
		printf(" \n ERRO - Heap de fibonacci ainda nao foi criado \n");
		return;
	}
	
	//Aloca espaco para novoNo
	novoNo = (NOH *)malloc(sizeof(NOH));
	
	//Atribui valores na struct do no
	novoNo->chave = valor;
	novoNo->grau = 0;
	novoNo->marcado = false;
	novoNo->pai = NULL;
	novoNo->filho = NULL;
	novoNo->vizEsq = novoNo;
	novoNo->vizDir = novoNo;
	
	//Se heap nao tiver MIN (vazio) - Atribui noh na posicao
	if (heap->min == NULL) 
	{
		heap->min = novoNo;
	} 
	//Se seu min nao estiver vazio, coloca como vizinho esquerdo do atual Min
	else 
	{
		heap->min->vizEsq->vizDir = novoNo;
		novoNo->vizDir = heap->min;
		novoNo->vizEsq = heap->min->vizEsq;
		heap->min->vizEsq = novoNo;
	
		//Se for menor que a chave do autal minimo, o novo noh passara a ser o minimo
		if (novoNo->chave < heap->min->chave) 
		{
			heap->min = novoNo;
		}
	}
	
	//Adiciona mais um noh ao qtd de noh do heap
	heap->num++;
	
	return;
}

// Une dois heaps em um novo heap
//FIB-HEAP-UNION (CORMEM)
FIB_HEAP *unirHeaps(FIB_HEAP *heapEsq, FIB_HEAP *heapDir) 
{
	//Declara e cria o novo heap
	FIB_HEAP *heapNovo;
	heapNovo = criaHeap();
		
	//Realiza o merge entre os heaps
	NOH *nohAux1, *nohAux2;
	
	heapNovo->min = heapEsq->min;
	
	
	nohAux1 = heapNovo->min->vizDir;
	nohAux2 = heapDir->min->vizEsq;
	
	heapNovo->min->vizDir->vizEsq = heapDir->min->vizEsq;
	heapNovo->min->vizDir = heapDir->min;
		
	heapDir->min->vizEsq = heapNovo->min;
	
	nohAux2->vizDir = nohAux1;
	
	//Verifica se o minimo do heap da esquerda é nulo ou seu valor eh menor que o min do heap da direita
	if ((heapEsq->min == NULL) || (heapDir->min != NULL && heapDir->min->chave < heapEsq->min->chave))
	{
		//Nesse caso o min do novo heap sera o min do heap da direita
		heapNovo->min = heapDir->min;
	}
	else
	{
		//Atribui ao min o valor minimo do heap da esquerda
		heapNovo->min = heapEsq->min;
	}
	
		
	//Somando o tamanho dos heaps	
	heapNovo->num = heapEsq->num + heapDir->num;
	
	return heapNovo;
}


// Retorna noh min
NOH *encontraMin(FIB_HEAP *heap) 
{
	if (heap == NULL) 
	{
		printf(" \n ERRO - Heap de fibonacci ainda nao foi criado \n");
		return NULL;
	} 
	else
		return heap->min;
}


// Funcao consolidar
//CONSOLIDATE (CORMEM)
void consolidar(FIB_HEAP *heap) 
{
	int num, qtdRoot, i, grauNohRoot;
		
	num = heap->num;
	
	qtdRoot = (int) log2(num);
			
	NOH *rootList[qtdRoot], *minH, *nohLink;
	
	//Zera os nohs da rootlist
	for (i = 0; i <= qtdRoot; i++) 
	{
		rootList[i] = NULL;
	}
	
	minH = heap->min;
    //Para cada noh na rootList de heap, sempre andando pro seu vizinho da direita
    
    do
	{
	
		grauNohRoot = minH->grau;
		
		while (rootList[grauNohRoot] != NULL) 
		{
			nohLink = rootList[grauNohRoot];
			//Troca o min do heap se chave for menor
			if (minH->chave > nohLink->chave) 
			{
				NOH *nohAux;
				nohAux = minH;
				minH = nohLink;
				nohLink = nohAux;
			}
			
			if (nohLink == heap->min)
			{
				heap->min = minH;
			}
			
			//Move e linka no root
			heapLink(heap, nohLink, minH);
			
			
			if (nohLink->vizDir == minH)
			{
				heap->min = minH;
			}
			
			rootList[grauNohRoot] = NULL;
			
			grauNohRoot++;
		}
		rootList[grauNohRoot] = minH;
	}while(minH != heap->min);
    
	heap->min = NULL;
	
	//Reconstroi  o heap a partir dos nohs que estao na rootList
	for (i = 0; i < qtdRoot; i++) 
	{
		if (rootList[i] != NULL) 
		{
			rootList[i]->vizEsq = rootList[i];
			rootList[i]->vizDir = rootList[i];
			
			if (heap->min == NULL) 
			{
				heap->min = rootList[i];
			} 
			else 
			{
				heap->min->vizEsq->vizDir = rootList[i];
				rootList[i]->vizDir = heap->min;
				rootList[i]->vizEsq = heap->min->vizEsq;
				heap->min->vizEsq = rootList[i];
				
				if (rootList[i]->chave < heap->min->chave) 
				{
				  heap->min = rootList[i];
				  
				}
			}
		} 
		
	}
		
}

// Movendo e linkando noh root
//FIB-HEAP-LINK (CORMEM)
void heapLink(FIB_HEAP *heap, NOH *nohMovido, NOH *nohDestino) 
{
	nohMovido->vizDir->vizEsq = nohMovido->vizEsq;
	nohMovido->vizEsq->vizDir = nohMovido->vizDir;
	
	if (nohDestino->vizDir == nohDestino)
	{
		heap->min = nohDestino;
	}
	
	nohMovido->vizEsq = nohMovido;
	nohMovido->vizDir = nohMovido;
	nohMovido->pai = nohDestino;
	
	if (nohDestino->filho == NULL) 
	{
		nohDestino->filho = nohMovido;
	}
	
	nohMovido->vizDir = nohDestino->filho;
	nohMovido->vizEsq = nohDestino->filho->vizEsq;
	nohDestino->filho->vizEsq->vizDir = nohMovido;
	nohDestino->filho->vizEsq = nohMovido;
	
	if (nohMovido->chave < nohDestino->filho->chave)
	{
		nohDestino->filho = nohMovido;
	}
	
	nohDestino->grau++;
}

// Extract min
//FIB-HEAP-EXTRACT-MIN (CORMEM)
NOH *deleteMin(FIB_HEAP *heap) 
{
	if (heap->min == NULL)
	{
		printf("\n O heap estah vazio\n");	
		return heap->min;
	}
	
	
	NOH *nohDeletar = heap->min;
	NOH *nohAuxiliar;
	nohAuxiliar = nohDeletar;
	NOH *nohFilho = NULL; //Filho
	
	if (nohDeletar->filho != NULL) 
	{
		//Percorre os filhos do noh a deletar
		for(nohFilho = nohDeletar->filho; nohAuxiliar != nohDeletar->filho; nohFilho = nohAuxiliar)
		{
			nohAuxiliar = nohFilho->vizDir;
			
			heap->min->vizEsq->vizDir = nohFilho;
			
			//Coloca referencias de vizinhanca no noh filho
			nohFilho->vizDir = heap->min;
			nohFilho->vizEsq = heap->min->vizEsq;
			
			//Adiciona noh filho na rootList do heap
			heap->min->vizEsq = nohFilho;
			
			if (nohFilho->chave < heap->min->chave)
			{
				heap->min = nohFilho;
			}
				
			nohFilho->pai = NULL;
		}
		
		
	}

	nohDeletar->vizEsq->vizDir = nohDeletar->vizDir;
	nohDeletar->vizDir->vizEsq = nohDeletar->vizEsq;
	
	//Remove o noh da rootList
	heap->min = nohDeletar->vizDir;
	
	//Unico noh na rootList
	if (nohDeletar == nohDeletar->vizDir)
	{
		heap->min = NULL;
	}
	else 
	{
		heap->min = nohDeletar->vizDir;
		consolidar(heap);
	}
	
	heap->num--;
	
	return nohDeletar;
}

//Funcao recursiva que imprime o heap na tela
void printHeap(NOH *nohRoot) 
{
	NOH *nohAux;
	
	for (nohAux = nohRoot; ; nohAux = nohAux->vizDir) 
	{
		if (nohAux->filho == NULL) 
		{
			printf("Noh sem filho (%d) \n", nohAux->chave);
		} 
		else 
		{
			printf("Noh (%d) com filho (%d)\n", nohAux->chave, nohAux->filho->chave);
			printHeap(nohAux->filho);
		}
		
		if(nohAux->vizDir == nohRoot)
		{
			break;
		}
	}
}

//Corta a relacao entre pai e filho, fazendo de filho um root do heap
//CUT (CORMEM)
void cortar(FIB_HEAP *heap, NOH *nohReduzido, NOH *nohPai) 
{
	//Se nao tiver irmaos remove noh da rootList
	if (nohReduzido == nohReduzido->vizDir)
	{
		nohPai->filho = NULL;
	}
	else
	{
		nohReduzido->vizEsq->vizDir = nohReduzido->vizDir;
		nohReduzido->vizDir->vizEsq = nohReduzido->vizEsq;
	
		if (nohReduzido == nohPai->filho)
		{
			nohPai->filho = nohReduzido->vizDir;
		}
	}	
	
	//Diminui o grau de pai		
	nohPai->grau--;
	
	nohReduzido->vizEsq = nohReduzido;
	nohReduzido->vizDir = nohReduzido;
	
	//Adiciona o noh reduzido na rootList do heap
	heap->min->vizEsq->vizDir = nohReduzido;
	heap->min->vizEsq = nohReduzido;
	
	//Aponta os vizinhos do noh reduzido
	nohReduzido->vizDir = heap->min;
	nohReduzido->vizEsq = heap->min->vizEsq;
	
	//Retira a marcacao e o pai
	nohReduzido->pai = NULL;
	nohReduzido->marcado = false;
}

//Marca o pai e realiza a operacao de cortar recursivamente
//CASCADING-CUT (CORMEM)
void cortarCascata(FIB_HEAP *heap, NOH *nohPai) 
{
	NOH *nohAux;
	nohAux = nohPai->pai;
	
	//Se noh tiver pai
	if (nohAux != NULL) 
	{
		//Se o pai nao estiver marcado, marque o noh		
		if (nohPai->marcado == false) 
		{
			nohPai->marcado = true;
		}
		//Se estiver marcado, corte e depois corte em cascata recursivamente 
		else 
		{
			cortar(heap, nohPai, nohAux);
			cortarCascata(heap, nohAux);
		}
	}
	
	return;
}

//Reduz o valor da chave para um valor menor
//FIB-HEAP-DECREASE-KEY (CORMEM)
void diminuiChave(FIB_HEAP *heap, NOH *nohReduzido, int novaChave) 
{
	NOH *nohPai;
	
	if (heap == NULL) 
	{
		printf("\n O heap de Fibonacci nao foi criado \n");
		return;
	}
	
	if (nohReduzido == NULL) 
	{
		printf("\n O noh nao existe \n");
	}
	else 
	{
		if (nohReduzido->chave < novaChave) 
		{
			printf("\n Chave do noh a ser reduzido eh maior que a chave escolhida \n ");
		} 
		else 
		{
			nohReduzido->chave = novaChave;
			nohPai = nohReduzido->pai;
			//Se noh pai eh maior, deve cortar o heap para trocar a posicao da chave
			if ((nohPai != NULL) && (nohReduzido->chave < nohPai->chave)) 
			{
				cortar(heap, nohReduzido, nohPai);
				cortarCascata(heap, nohPai);
			}
			if (nohReduzido->chave < heap->min->chave) 
			{
				heap->min = nohReduzido;
			}
		}
	}
	
	return;
}


//Funcao auxiliar para encontrar o noh a ser reduzido
void *encontraNoh(FIB_HEAP *heap, NOH *nohRoot, int chave, int novaChave) 
{
	NOH *nohVisitado = nohRoot;
	NOH *f = NULL;
	nohVisitado->marcado = true;
	
	if (nohVisitado->chave == chave) 
	{
		nohVisitado->marcado = false;
		f = nohVisitado;
		diminuiChave(heap, f, novaChave);
	}
	if (nohVisitado->filho != NULL) 
	{
		encontraNoh(heap, nohVisitado->filho, chave, novaChave);
	}
	if ((nohVisitado->vizDir->marcado != true)) 
	{
		encontraNoh(heap, nohVisitado->vizDir, chave, novaChave);
	}
	
	nohVisitado->marcado = false;
}


//Deleta o noh
//FIB-HEAP-DELETE
void deleteNoh(FIB_HEAP *heap, int chave) 
{
	NOH *nohRemovido = NULL;
	
	encontraNoh(heap, heap->min, chave, -100000);
	nohRemovido = deleteMin(heap);
	
	if (nohRemovido != NULL)
	{
		printf("\n NOH deleted");
	}
	else
	{
		printf("\n Nao foi possivel remover o noh");
	}
    
}


int main(int argc, char **argv) 
{
	NOH  *nohMin, *minExtraido, *novoNoh, *nohReduzido, *nohAux;
	FIB_HEAP *heap, *heapAux; 
	int opcao, novaChave, chaveReduzir, i, qtdNohs, chave, chaveDeletar;
	
	heap = (FIB_HEAP *)malloc(sizeof(FIB_HEAP));
	heap = NULL;
	
	while (true) 
	{
		printf("\n Operacoes com heaps de Fibonacci \n"); 
		printf("\n 1. Criar Fibonacci heap \n "); 
		printf("\n 2. Inserir nohs \n ");  
		printf("\n 3. Encontrar o min \n "); 
		printf("\n 4. Uniao \n "); 
		printf("\n 5. Extrair o min \n ");
		printf("\n 6. Diminuir a chave \n ");
		printf("\n 7. Imprimir heap \n ");
		printf("\n 8. Sair e deletar o heap\n ");
		printf("\n 9. Deletar um noh\n");
		printf("\n Digite o numero da operacao desejada \n");
		scanf("%d", &opcao);
		
		switch (opcao) 
		{
			//Cria heap
			case 1:
				heap = criaHeap();
			break;
			//Insere nohs
			case 2:
				
				if (heap == NULL) 
				{
					heap = criaHeap();
				}
				
				
				printf(" \n Digite a quantidade de nohs a ser inserido = ");
				scanf("%d", &qtdNohs);
		
				for (i = 1; i <= qtdNohs; i++) 
				{
					printf("\n Digite o valor da chave do noh %d \n", i);
					scanf("%d", &chave);
					insercao(heap, novoNoh, chave);
				}
			
			break;
			//Retorna min
			case 3:
				nohMin = encontraMin(heap);
				if (nohMin == NULL)
				{
					printf("\n Heap vazio, sem valor minimo \n ");
				}
				else
				{
					printf("\n Valor min = %d", nohMin->chave);
				}
				
			break;
			//Unir heaps
			case 4:
				if (heap == NULL) 
				{
					printf("\n O heap de Fibonacci ainda nao foi criado \n ");
					break;
				}
							
				printf(" \n Digite a quantidade de nohs a ser inserido = ");
				scanf("%d", &qtdNohs);
		
				for (i = 1; i <= qtdNohs; i++) 
				{
					NOH *novoNoh;
					printf("\n Digite o valor da chave do noh %d = ", i);
					scanf("%d", &chave);
					insercao(heapAux, novoNoh, chave);
				}
				
				heap = unirHeaps(heap, heapAux);
				//printf("\n Novo heap:\n");
				//printHeap(heap->min);
			
			break;
			//Extrair o min
			case 5:
				if (heap == NULL)
				{
					printf("\n Heap vazio \n");
				}
				else 
				{
					minExtraido = deleteMin(heap);
					printf("\n Valor minimo extraido = %d\n", minExtraido->chave);
					
				}
			break;
			
			case 6:
				if (heap == NULL)
				{
					printf("\nO heap nao existe\n");
				}
				else 
				{
					printf(" \n Noh para ser reduzido = ");
					scanf("%d", &chaveReduzir);
					printf(" \n Digite a nova chave = ");
					scanf("%d", &novaChave);
					nohAux = heap->min;
					encontraNoh(heap, nohAux, chaveReduzir, novaChave);
					printf("\n Chave reduzida:\n");
					//printHeap(heap->min);
				}
			break;
			//Imprimir o heap
			case 7:
				printHeap(heap->min);
			break;
			//Deletar o heap
			case 8:
				free(novoNoh);
				free(heap);
				printf("\nHeap deletado\n");
			exit(0);
			//Deletar um noh
			case 9:
				if (heap == NULL)
				{
					printf("\nO heap nao existe\n");
				}
				else
				{
					printf(" \n Noh para ser removido = ");
					scanf("%d", &chaveDeletar);	
					deleteNoh(heap, chaveDeletar);
				}
			
			default:
			printf("\nEscolha invalida \n");
		}
	}
}