#include <climits>
#include <clocale>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

/* Mensagens*/

const char TEXTO_AJUDA[] =
  "-h           : mostra o help\n"
  "-o <arquivo> : redireciona a saida para o “arquivo”\n"
  "-f <arquivo> : indica o “arquivo” que contém o grafo de entrada\n"
  "-i           : vértice inicial\n";

const char TEXTO_ERRO[] = "Parâmetro desconhecido (use -h para ver os disponíveis)";

/* Tipos */
typedef std::pair<unsigned, unsigned> UnsignedPair;
typedef std::vector<std::vector<UnsignedPair>> Grafo;

typedef struct Dados {
  std::vector<int> prev;
  std::vector<unsigned> dist;
} Dados;

/* Funções */

Grafo ler_grafo(std::istream *in) {
  unsigned qtd_vertices, qtd_arestas, u, v, w, i;

  *in >> qtd_vertices >> qtd_arestas;
  Grafo grafo(qtd_vertices, std::vector<UnsignedPair>());

  for (i = 0; i < qtd_arestas; i++) {
    *in >> u >> v >> w;
    u--;
    v--;
    grafo[u].push_back({v, w});
    grafo[v].push_back({u, w});
  }

  return grafo;
}

Dados djikstra(Grafo &grafo, unsigned s) {
  unsigned u, v, w, i;

  std::vector<unsigned> dist(grafo.size(), INT_MAX);
  std::vector<int> prev(grafo.size(), -1);
  std::priority_queue<UnsignedPair, std::vector<UnsignedPair>, std::greater<UnsignedPair>> heap;

  dist[s] = 0;

  for (i = 0; i < grafo.size(); i++) {
    heap.push({dist[i], i});
  }

  while (!heap.empty()) {
    u = heap.top().second;
    heap.pop();

    for (i = 0; i < grafo[u].size(); i++) {
      UnsignedPair aresta = grafo[u][i];
      v = aresta.first;
      w = aresta.second;

      if (dist[v] > dist[u] + w) {
        dist[v] = dist[u] + w;
        prev[v] = u;
        heap.push({dist[v], v});
      }
    }
  }

  return {prev, dist};
}

void imprimir_saida(std::ostream *saida, Dados &dados) {
  unsigned u;

  for (u = 0; u < dados.dist.size(); u++) {
    *saida << u + 1 << ":" << dados.dist[u] << " ";
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
  unsigned vertice_inicial = 1;
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
    } else if (!strcmp(arg, "-i")) {
      vertice_inicial = strtoul(argv[++i], nullptr, 10);
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
  Dados dados = djikstra(grafo, vertice_inicial - 1);
  imprimir_saida(saida, dados);

  return EXIT_SUCCESS;
}
