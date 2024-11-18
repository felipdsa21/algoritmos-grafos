#include <clocale>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

/* Tipos */
typedef std::tuple<int, int, int> Aresta;

typedef struct Grafo {
  int qtd_vertices;
  std::vector<Aresta> arestas;
} Grafo;

typedef struct Resultado {
  std::vector<int> dist;
} Resultado;

/* Funções */

Grafo ler_grafo(std::istream *in) {
  int qtd_vertices, qtd_arestas, u, v, w, i;

  *in >> qtd_vertices >> qtd_arestas;
  std::vector<Aresta> arestas;
  arestas.reserve(qtd_arestas);

  for (i = 0; i < qtd_arestas; i++) {
    *in >> u >> v >> w;
    u--;
    v--;
    arestas.push_back({w, u, v});
    arestas.push_back({w, v, u});
  }

  return {qtd_vertices, arestas};
}

Resultado bellman(const Grafo &grafo, int s) {
  int u, v, w, l;

  std::vector<int> d(grafo.qtd_vertices, std::numeric_limits<int>::max());
  d[s] = 0;

  for (l = 0; l < grafo.qtd_vertices - 1; l++) {
    for (Aresta aresta : grafo.arestas) {
      w = std::get<0>(aresta);
      u = std::get<1>(aresta);
      v = std::get<2>(aresta);

      if (d[u] != std::numeric_limits<int>::max() && d[u] + w < d[v]) {
        d[v] = d[u] + w;
      }
    }
  }

  return {d};
}

void imprimir_resultado(std::ostream *saida, const Resultado &resultado) {
  int u, d;

  const std::vector<int> &dist = resultado.dist;
  for (u = 0; u < (int)dist.size(); u++) {
    d = dist[u] == std::numeric_limits<int>::max() ? -1 : dist[u];
    *saida << u + 1 << ":" << d << " ";
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
  int vertice_inicial = 0, i;

  configurar_terminal();

  for (i = 1; i < argc; i++) {
    arg = argv[i];
    if (!strcmp(arg, "-h")) {
      const char msg[] =
        "Parâmetros disponíveis:\n"
        "-h           : mostra o help\n"
        "-o <arquivo> : redireciona a saida para o “arquivo”\n"
        "-f <arquivo> : indica o “arquivo” que contém o grafo de entrada\n"
        "-i           : vértice inicial\n";
      std::cout << msg << std::flush;
      return EXIT_SUCCESS;
    } else if (!strcmp(arg, "-o")) {
      caminho_saida = argv[++i];
    } else if (!strcmp(arg, "-f")) {
      caminho_entrada = argv[++i];
    } else if (!strcmp(arg, "-i")) {
      vertice_inicial = (int)std::stoul(argv[++i]) - 1;
    } else {
      std::cerr << "Parâmetro desconhecido (use -h para ver os disponíveis)" << std::endl;
      return EXIT_FAILURE;
    }
  }

  std::istream *entrada = &std::cin;
  std::ifstream arquivo_entrada;
  if (caminho_entrada) {
    arquivo_entrada.open(caminho_entrada);
    if (arquivo_entrada.fail()) {
      std::cerr << "Não foi possível abrir o arquivo de entrada" << std::endl;
      return EXIT_FAILURE;
    }

    entrada = &arquivo_entrada;
  }

  std::ostream *saida = &std::cout;
  std::ofstream arquivo_saida;
  if (caminho_saida) {
    arquivo_saida.open(caminho_saida);
    if (arquivo_saida.fail()) {
      std::cerr << "Não foi possível abrir o arquivo de saída" << std::endl;
      return EXIT_FAILURE;
    }

    saida = &arquivo_saida;
  }

  Grafo grafo = ler_grafo(entrada);
  Resultado resultado = bellman(grafo, vertice_inicial);
  imprimir_resultado(saida, resultado);

  return EXIT_SUCCESS;
}
