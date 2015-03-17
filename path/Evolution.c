#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <random>
#include <vector>
#include <algorithm>

#include "Application.h"
#include "Contract.h"
#include "Interface.h"

const int n = 50;
const int m = 1000;

struct Edge {
	int source, target;
	bool isAdd;

	Edge(int source, int target, bool isAdd): source(source), target(target), isAdd(isAdd) {}
};

Edge randomEdge(std::mt19937 &rng, int n) {
	return Edge(1 + rng() % n, 1 + rng() % n, rng() % 2);
}

long evaluate0(std::vector<Edge> const &individual, char const *arg0) {
	FILE *f = fopen("evolution.in", "wt");
	for (int i = 0; i < m; ++i) {
		fprintf(f, "%d %d %d\n", individual[i].source, individual[i].target, individual[i].isAdd);
	}
	fclose(f);

	pid_t w = fork();
	if (w) {
		wait(NULL);
		f = fopen("evolution.out", "rt");
		long rv;
		fscanf(f, "%ld", &rv);
		fclose(f);
		return rv;
	} else {
		if (execl(arg0, arg0, "f", (char*) (NULL))) {
			throw 89;
		}
	}
}

long evaluate1(std::vector<Edge> const &individual) {
	int maxV = 0;
	int sz = individual.size();
	for (int i = 0; i < sz; ++i) {
		maxV = std::max(maxV, std::max(individual[i].source, individual[i].target));
	}
	RC_Forest f(maxV);
	f.contract();
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
				}
			} else {
				if (f.isEdge(vs, vt)) {
					f.cut(vs, vt);
					f.contract();
				}
			}
		}
	}
	return f.getPerformance();
}

void mutate(std::vector<Edge> &individual, int n, std::mt19937 &rng) {
	for (int i = 0, sz = individual.size(); i < sz; ++i) {
		if (rng() % sz == 0) {
			individual[i] = randomEdge(rng, n);
		}
	}
}

int main0(char const *arg0) {
	std::mt19937 rng;
	std::vector<Edge> currI;
	for (int i = 0; i < m; ++i) {
		currI.push_back(randomEdge(rng, n));
	}
	long currF = evaluate0(currI, arg0);
	printf("0: %ld\n", currF);

	for (int g = 1; ; ++g) {
		std::vector<Edge> nextI = currI;
		mutate(nextI, n, rng);
		long nextF = evaluate0(nextI, arg0);
		if (nextF >= currF) {
			currF = nextF;
			currI = nextI;
		}
		printf("%d: %ld, max = %ld\n", g, nextF, currF);
	}
	return 0;
}

int main1() {
	std::vector<Edge> input;
	FILE *f = fopen("evolution.in", "rt");
	for (int i = 0; i < m; ++i) {
		int src, trg, add;
		fscanf(f, "%d%d%d", &src, &trg, &add);
		input.push_back(Edge(src, trg, add));
	}
	fclose(f);
	long rv = evaluate1(input);
	f = fopen("evolution.out", "wt");
	fprintf(f, "%ld\n", rv);
	fclose(f);
	return 0;
}

int main(int argc, char **argv) {
	if (argc > 1 && !strcmp(argv[1], "f")) {
		return main1();
	} else {
		return main0(argv[0]);
	}
}
