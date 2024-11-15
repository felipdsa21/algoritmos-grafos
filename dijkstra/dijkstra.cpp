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
typedef std::pair<unsigned, unsigned> ParInt;
typedef std::vector<std::vector<ParInt>> Grafo;

typedef struct Resultado {
  std::vector<int> prev;
  std::vector<unsigned> dist;
} Resultado;

/* Funções */

Grafo ler_grafo(std::istream *in) {
  unsigned qtd_vertices, qtd_arestas, u, v, w, i;

  *in >> qtd_vertices >> qtd_arestas;
  Grafo grafo(qtd_vertices, std::vector<ParInt>());

  for (i = 0; i < qtd_arestas; i++) {
    *in >> u >> v >> w;
    u--;
    v--;
    grafo[u].push_back({w, v});
    grafo[v].push_back({w, u});
  }

  return grafo;
}

Resultado dijkstra(const Grafo &grafo, unsigned s) {
  unsigned u, v, w, nova_dist;

  std::vector<unsigned> dist(grafo.size(), UINT_MAX);
  std::vector<int> prev(grafo.size(), -1);
  std::priority_queue<ParInt, std::vector<ParInt>, std::greater<ParInt>> heap;

  dist[s] = 0;
  heap.push({dist[s], s});

  while (!heap.empty()) {
    u = heap.top().second;
    heap.pop();

    for (ParInt aresta : grafo[u]) {
      v = aresta.second;
      w = aresta.first;
      nova_dist = dist[u] + w;

      if (dist[v] > nova_dist) {
        dist[v] = nova_dist;
        prev[v] = (int)u;
        heap.push({dist[v], v});
      }
    }
  }

  return {prev, dist};
}

void imprimir_resultado(std::ostream *saida, const Resultado &resultado) {
  unsigned u;

  const std::vector<unsigned> &dist = resultado.dist;
  for (u = 0; u < dist.size(); u++) {
    *saida << u + 1 << ":" << dist[u] << " ";
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
  unsigned vertice_inicial = 0;
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
      vertice_inicial = (unsigned)strtoul(argv[++i], nullptr, 10) - 1;
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
  Resultado resultado = dijkstra(grafo, vertice_inicial);
  imprimir_resultado(saida, resultado);

  return EXIT_SUCCESS;
}
