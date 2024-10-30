#include <algorithm>
#include <climits>
#include <clocale>
#include <cstring>
#include <fstream>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>

/* Mensagens*/

const char TEXTO_AJUDA[] =
  "-h           : mostra o help\n"
  "-o <arquivo> : redireciona a saida para o “arquivo”\n"
  "-f <arquivo> : indica o “arquivo” que contém o grafo de entrada\n"
  "-s           : mostra a solução\n";

const char TEXTO_ERRO[] = "Parâmetro desconhecido (use -h para ver os disponíveis)";

/* Tipos */
typedef std::pair<int, unsigned> ParInteiros;
typedef std::tuple<int, unsigned, unsigned> Aresta;

typedef struct Grafo {
  unsigned qtd_vertices;
  std::vector<Aresta> arestas;
} Grafo;

typedef struct Dados {
  std::set<Aresta> T;
} Dados;

/* Funções */

Grafo ler_grafo(std::istream *in) {
  unsigned qtd_vertices, qtd_arestas, u, v, i;
  int w;

  *in >> qtd_vertices >> qtd_arestas;
  std::vector<Aresta> arestas;
  arestas.reserve(qtd_arestas);

  for (i = 0; i < qtd_arestas; i++) {
    *in >> u >> v >> w;
    u--;
    v--;
    arestas.push_back({w, u, v});
  }

  return {qtd_vertices, arestas};
}

void union_(std::vector<unsigned> &pai, std::vector<unsigned> &rank, unsigned x, unsigned y) {
  if (rank[x] >= rank[y]) {
    pai[y] = x;
    if (rank[x] == rank[y]) {
      rank[x]++;
    }
  } else {
    pai[x] = y;
  }
}

unsigned find(std::vector<unsigned> &pai, unsigned x) {
  if (pai[x] != x) {
    pai[x] = find(pai, pai[x]);
  }
  return pai[x];
}

Dados kruskal(Grafo &grafo) {
  unsigned u, v, find_u, find_v;

  std::vector<Aresta> arestas(grafo.arestas);
  std::vector<unsigned> pai(grafo.qtd_vertices);
  std::vector<unsigned> rank(grafo.qtd_vertices, 0);
  std::set<Aresta> T;

  std::sort(arestas.begin(), arestas.end());

  // Make-set
  for (v = 0; v < grafo.qtd_vertices; v++) {
    pai[v] = v;
  }

  for (Aresta aresta : arestas) {
    u = std::get<1>(aresta);
    v = std::get<2>(aresta);

    find_u = find(pai, u);
    find_v = find(pai, v);

    if (find_u != find_v) {
      T.insert(aresta);
      union_(pai, rank, find_u, find_v);
    }
  }

  return {T};
}

void imprimir_saida(std::ostream *saida, Dados &dados, bool mostrar_solucao) {
  int custo_total = 0;
  unsigned u, v;

  if (mostrar_solucao) {
    for (Aresta aresta : dados.T) {
      u = std::get<1>(aresta);
      v = std::get<2>(aresta);
      *saida << "(" << u + 1 << "," << v + 1 << ") ";
    }
  } else {
    for (Aresta aresta : dados.T) {
      custo_total += std::get<0>(aresta);
    }

    *saida << custo_total;
  }

  *saida << std::endl;
}

void configurar_terminal() {
#ifdef _WIN32
  // Evita problemas com texto no terminal no Windows
  // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=msvc-170#utf-8-support
  setlocale(LC_ALL, ".UTF8");
#endif /* WIN32 */
}

int main(int argc, char *argv[]) {
  char *caminho_entrada = nullptr, *caminho_saida = nullptr, *arg;
  bool mostrar_solucao = false;
  int i;

  configurar_terminal();

  for (i = 1; i < argc; i++) {
    arg = argv[i];
    if (!strcmp(arg, "-h")) {
      std::cout << TEXTO_AJUDA << std::flush;
      return EXIT_SUCCESS;
    } else if (!strcmp(arg, "-o")) {
      caminho_saida = argv[++i];
    } else if (!strcmp(arg, "-f")) {
      caminho_entrada = argv[++i];
    } else if (!strcmp(arg, "-s")) {
      mostrar_solucao = true;
    } else {
      std::cerr << TEXTO_ERRO << std::flush;
      return EXIT_FAILURE;
    }
  }

  std::istream *entrada = &std::cin;
  std::ifstream arquivo_entrada;
  if (caminho_entrada) {
    arquivo_entrada.open(caminho_entrada);
    entrada = &arquivo_entrada;
  }

  std::ostream *saida = &std::cout;
  std::ofstream arquivo_saida;
  if (caminho_saida) {
    arquivo_saida.open(caminho_saida);
    saida = &arquivo_saida;
  }

  Grafo grafo = ler_grafo(entrada);
  Dados dados = kruskal(grafo);
  imprimir_saida(saida, dados, mostrar_solucao);

  return EXIT_SUCCESS;
}
