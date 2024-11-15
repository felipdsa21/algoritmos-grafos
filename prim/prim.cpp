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
  "-s           : mostra a solução\n"
  "-i           : vértice inicial (para o algoritmo de Prim)\n";

const char TEXTO_ERRO[] = "Parâmetro desconhecido (use -h para ver os disponíveis)";

/* Tipos */
typedef std::pair<int, int> ParInt;
typedef std::vector<std::vector<ParInt>> Grafo;

typedef struct Resultado {
  std::vector<int> prev;
  std::vector<int> custo;
} Resultado;

/* Funções */

Grafo ler_grafo(std::istream *in) {
  int qtd_vertices, qtd_arestas, u, v, w, i;

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

Resultado prim(const Grafo &grafo, int v0) {
  int u, v, w;

  std::vector<int> custo(grafo.size(), INT_MAX);
  std::vector<int> prev(grafo.size(), -1);
  std::vector<bool> visitado(grafo.size(), false);
  std::priority_queue<ParInt, std::vector<ParInt>, std::greater<ParInt>> heap;

  custo[v0] = 0;
  heap.push({custo[v0], v0});

  while (!heap.empty()) {
    v = heap.top().second;
    heap.pop();

    if (visitado[v]) {
      continue;
    } else {
      visitado[v] = true;
    }

    for (ParInt aresta : grafo[v]) {
      u = aresta.second;
      w = aresta.first;

      if (!visitado[u] && custo[u] > w) {
        custo[u] = w;
        prev[u] = v;
        heap.push({custo[u], u});
      }
    }
  }

  return {prev, custo};
}

void imprimir_resultado(std::ostream *saida, const Resultado &resultado, bool mostrar_solucao) {
  int custo_total = 0, v;

  if (mostrar_solucao) {
    const std::vector<int> &prev = resultado.prev;

    for (v = 0; v < (int)prev.size(); v++) {
      if (prev[v] != -1) {
        *saida << "(" << v + 1 << "," << prev[v] + 1 << ") ";
      }
    }
  } else {
    const std::vector<int> &custo = resultado.custo;

    for (v = 0; v < (int)custo.size(); v++) {
      custo_total += custo[v];
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
  int vertice_inicial = 0;
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
    } else if (!strcmp(arg, "-i")) {
      vertice_inicial = strtoul(argv[++i], nullptr, 10) - 1;
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
  Resultado resultado = prim(grafo, vertice_inicial);
  imprimir_resultado(saida, resultado, mostrar_solucao);

  return EXIT_SUCCESS;
}
