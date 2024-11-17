#include <algorithm>
#include <clocale>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

/* Mensagens*/

const char TEXTO_AJUDA[] =
  "-h           : mostra o help\n"
  "-o <arquivo> : redireciona a saida para o “arquivo”\n"
  "-f <arquivo> : indica o “arquivo” que contém o grafo de entrada\n";

const char TEXTO_ERRO[] = "Parâmetro desconhecido (use -h para ver os disponíveis)";

/* Tipos */
typedef std::vector<std::vector<int>> Grafo;

typedef struct Resultado {
  std::vector<std::vector<int>> componentes;
} Resultado;

/* Funções */

Grafo ler_grafo(std::istream *in) {
  int qtd_vertices, qtd_arestas, u, v, i;

  *in >> qtd_vertices >> qtd_arestas;
  Grafo grafo(qtd_vertices);

  for (i = 0; i < qtd_arestas; i++) {
    *in >> u >> v;
    u--;
    v--;
    grafo[u].push_back(v);
  }

  return grafo;
}

Grafo inverter_grafo(const Grafo &grafo) {
  int u;
  Grafo invertido(grafo.size());

  for (u = 0; u < (int)grafo.size(); u++) {
    for (int v : grafo[u]) {
      invertido[v].push_back(u);
    }
  }

  return invertido;
}

void dfs(const Grafo &grafo, int v, std::vector<bool> &visitado, std::stack<int> &permutacao) {
  visitado[v] = true;

  for (int u : grafo[v]) {
    if (!visitado[u]) {
      dfs(grafo, u, visitado, permutacao);
    }
  }

  permutacao.push(v);
}

void dfs_rev(const Grafo &grafo, int v, std::vector<bool> &visitado, std::vector<int> &componente) {
  visitado[v] = true;
  componente.push_back(v);

  for (int u : grafo[v]) {
    if (!visitado[u]) {
      dfs_rev(grafo, u, visitado, componente);
    }
  }
}

Resultado kosaraju(const Grafo &grafo) {
  int v;

  Grafo invertido = inverter_grafo(grafo);

  std::vector<bool> visitado(grafo.size(), false);
  std::stack<int> permutacao;

  for (int v = 0; v < (int)grafo.size(); ++v) {
    if (!visitado[v]) {
      dfs(invertido, v, visitado, permutacao);
    }
  }

  std::fill(visitado.begin(), visitado.end(), false);
  std::vector<std::vector<int>> componentes;

  while (!permutacao.empty()) {
    v = permutacao.top();
    permutacao.pop();

    if (!visitado[v]) {
      std::vector<int> componente;
      dfs_rev(grafo, v, visitado, componente);
      componentes.push_back(componente);
    }
  }

  return {componentes};
}

void imprimir_resultado(std::ostream *saida, const Resultado &resultado) {
  for (std::vector<int> componente : resultado.componentes) {
    for (int u : componente) {
      *saida << u + 1 << " ";
    }

    *saida << std::endl;
  }
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
  Resultado resultado = kosaraju(grafo);
  imprimir_resultado(saida, resultado);

  return EXIT_SUCCESS;
}
