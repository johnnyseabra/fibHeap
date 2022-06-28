typedef struct _NOH 
{
  int chave;
  int grau;
  struct _NOH *vizEsq;
  struct _NOH *vizDir;
  struct _NOH *pai;
  struct _NOH *filho;
  bool marcado;
 
} NOH;

typedef struct fibanocciHeap 
{
  int num; //tamanho
  NOH *min; //ponteiro pro min
} FIB_HEAP;

FIB_HEAP *criaHeap();
void insercao(FIB_HEAP *heap, NOH *novo, int valor);
FIB_HEAP *unirHeaps(FIB_HEAP *heapDir, FIB_HEAP *heapEsq);
NOH *encontraMin(FIB_HEAP *heap);
void consolidar(FIB_HEAP *heap);
void heapLink(FIB_HEAP *heap, NOH *y, NOH *x);
NOH *deleteMin(FIB_HEAP *heap);
void printHeap(NOH *noh);
void cortar(FIB_HEAP *heap, NOH *nohReduzido, NOH *nohPai);
void cortarCascata(FIB_HEAP *heap, NOH *nohPai);
void diminuiChave(FIB_HEAP *heap, NOH *node, int chave);
void *encontraNoh(FIB_HEAP *heap, NOH *nohRoot, int chave, int novaChave);
void deleteNoh(FIB_HEAP *heap, int chave);
int main(int argc, char **argv); 