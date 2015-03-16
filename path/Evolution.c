#include <stdlib.h>
#include <time.h>
#include <random>
#include <vector>
#include <algorithm>

#include "Application.h"
#include "Contract.h"
#include "Interface.h"

struct Edge {
	int source, target;
	bool isAdd;

	Edge(int source, int target, bool isAdd): source(source), target(target), isAdd(isAdd) {}
};

Edge randomEdge(std::mt19937 &rng, int n) {
	return Edge(1 + rng() % n, 1 + rng() % n, rng() % 2);
}

long evaluate(std::vector<Edge> const &individual) {
	int maxV = 0;
	int sz = individual.size();
	for (int i = 0; i < sz; ++i) {
		maxV = std::max(maxV, std::max(individual[i].source, individual[i].target));
	}
	RC_Forest f(maxV * 2);
	f.contract();
	for (int i = 0; i < sz; ++i) {
		Edge const &e = individual[i];
		if (e.source < 1 || e.source > maxV) throw 5;
		if (e.target < 1 || e.target > maxV) throw 6;
		node *vs = f.vertex(e.source);
		node *vt = f.vertex(e.target);
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
	return f.getPerformance();
}

void mutate(std::vector<Edge> &individual, int n, std::mt19937 &rng) {
	for (int i = 0, sz = individual.size(); i < sz; ++i) {
		if (rng() % sz == 0) {
			individual[i] = randomEdge(rng, n);
		}
	}
}

int main() {
	const int n = 10;
	const int m = 9;

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
			currF = nextF;
			currI = nextI;
		}
		printf("%d: %ld, max = %ld\n", g, nextF, currF);
	}
	return 0;
}
