#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

int n;
double x[1010], y[1010];
double b, ansa, ansb, ans;

double sqr(double a) { return a * a; }

double calc(double a) {
	b = 0;
	for (int i = 0; i < n; ++ i)
		b += y[i] - a * x[i];
	b /= n;
	double res = 0;
	for (int i = 0; i < n; ++ i)
		res += sqr(y[i] - a * x[i] - b);
	return res;
}

void update(double e, double a, double b) {
	if (e < ans) {
		ans = e; ansa = a; ansb = b;
	}
}

int main() {
	cin >> n;
	for (int i = 0; i < n; ++ i) cin >> x[i] >> y[i];
	double l = 0, r = 1e10;
	ans = 1e100;
	for (int times = 0; times <= 100; ++ times) {
		double L = l + (r - l) / 3, R = r - (r - l) / 3;
		double el = calc(L);
		update(el, L, b);
		double er = calc(R);
		update(er, R, b);
		if (el > er) l = L;
		else r = R;
	}
	printf("%.3f\n%.3f\n", ansa, ansb);
	return 0;
}


