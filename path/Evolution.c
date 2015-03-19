#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <random>
#include <vector>
#include <algorithm>

#include "Application.h"
#include "Contract.h"
#include "Interface.h"

const int n = 100;
const int m = 500;
const long powX = 10000000;

struct Edge {
	int source, target;
	bool isAdd;

	Edge(int source, int target, bool isAdd): source(source), target(target), isAdd(isAdd) {}
};

Edge randomEdge(std::mt19937 &rng, int n) {
	return Edge(1 + rng() % n, 1 + rng() % n, rng() % 2);
}

long evaluate(std::vector<Edge> const &individual) {
	srand(237357235);
	int maxV = 0;
	int sz = individual.size();
	for (int i = 0; i < sz; ++i) {
		maxV = std::max(maxV, std::max(individual[i].source, individual[i].target));
	}
	RC_Forest f(maxV);
	f.contract();
	int validCount = 0;
	for (int i = 0; i < sz; ++i) {
		Edge const &e = individual[i];
		if (e.source < 1 || e.source > maxV) throw 5;
		if (e.target < 1 || e.target > maxV) throw 6;
		node *vs = f.vertex(e.source);
		node *vt = f.vertex(e.target);
		if (e.source != e.target) {
			if (e.isAdd) {
				bin_data d = pathQuery(vs, vt);
				if (d.weight != 1) {
					f.link(vs, vt, 1);
					f.contract();
					++validCount;
				}
			} else {
				if (f.isEdge(vs, vt)) {
					f.cut(vs, vt);
					f.contract();
					++validCount;
				}
			}
		}
	}
	return f.getPerformance() + (validCount * powX);
}

void mutate(std::vector<Edge> &individual, int n, std::mt19937 &rng) {
	for (int i = 0, sz = individual.size(); i < sz; ++i) {
		if (rng() % sz == 0) {
			individual[i] = randomEdge(rng, n);
		}
	}
}

int evolution() {
	std::mt19937 rng;
	std::vector<Edge> currI;
	for (int i = 0; i < m; ++i) {
		currI.push_back(randomEdge(rng, n));
	}
	long currF = evaluate(currI);
	printf("0: %ld\n", currF);

	for (int g = 1; ; ++g) {
		std::vector<Edge> nextI = currI;
		mutate(nextI, n, rng);
		long nextF = evaluate(nextI);
		if (nextF >= currF) {
			if (nextF > currF) {
				char tmp[1024];
				sprintf(tmp, "evolution-%ld.test", nextF);
				FILE *trgf = fopen(tmp, "wt");
				for (int i = 0; i < m; ++i) {
					Edge &e = nextI[i];
					fprintf(trgf, "%d %d %d\n", e.source, e.target, e.isAdd);
				}
				fclose(trgf);
				printf("%d: %ld => %ld\n", g, currF, nextF);
			}
			currF = nextF;
			currI = nextI;
		}
	}
	return 0;
}

int main(int argc, char **argv) {
	if (argc == 1) {
		return evolution();
	} else {
		FILE *fin = fopen(argv[1], "rt");
		std::vector<Edge> v;
		int src = -1, trg, add;
		while (fscanf(fin, "%d%d%d", &src, &trg, &add), src != -1) {
			v.push_back(Edge(src, trg, add));
			src = -1;
		}
		printf("items: %lu\n", v.size());
		fclose(fin);
		long fitness = evaluate(v);
		printf("fitness = %ld\n", fitness);
		fitness = evaluate(v);
		printf("fitness = %ld\n", fitness);
		return 0;
	}
}
