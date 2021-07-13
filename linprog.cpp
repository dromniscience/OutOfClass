/*
 * My LP Solver:
 * Input Format
 *     n m // n is # of var, m is # of <= constraint
 *     a list of n numbers // objective to be maximized
 *     a m * n matrix // coefficients
 * Output Format:
 *     1. Exception // Infeasible / Unbounded
 *     2. An optimal solution plus the maximal objective value
 */

#include <iostream>
#include <exception>
#include <cstring>
enum class Expt {INFEASIBLE, UNBOUNDED};
const double neginf = -1.0 / 0;

class LinProgExp: public std::exception{
public:
	LinProgExp(Expt st) throw(): stat(st) {}
	const char* what() const throw(){
		if(stat == Expt::INFEASIBLE)
			return "Infeasible Instance!";
		else
			return "Unbounded Objective!";
	}

	Expt stat; 
};

class Table{
public:
	Table(int m, int n): hgh(m), wth(n) {
		tab = new double[m * n];
		basic = new int[n];
		memset(tab, 0, m * n * sizeof(double));
		memset(basic, 0, n * sizeof(int));
	}
	~Table() { delete [] tab; delete [] basic;}

	void RowDot(int r, double coe);
	void RowSub(int dst, int src, double coe, int st = 0);
	void ColSub(int dst, int src, double coe, int st = 0);
	std::pair<int, double> ColMax(int c, int st = 0);
	std::pair<int, double> RowPos(int r, int st = 0);
	std::pair<int, double> RowNonZero(int r, int st = 0);

	void Pivot(int varin, int varout, int r);
	void Simplex();
	void InitSimplex();
	void LinProg();

	void Display();

	const int hgh, wth;
	double *tab;
	int *basic;
};

void Table::RowDot(int r, double coe){
	int off = r * wth;
	for(int i = 0; i < wth; ++i)
		tab[off + i] *= coe;
}
void Table::RowSub(int dst, int src, double coe, int st){
	int srcoff = src * wth, dstoff = dst * wth;
	for(int i = st; i < wth; ++i)
		tab[dstoff + i] -= coe * tab[srcoff + i];
}

void Table::ColSub(int dst, int src, double coe, int st){
	int bound = hgh * wth;
	for(int i = st * wth;i < bound; i += wth)
		tab[i + dst] -= coe * tab[i + src];
}

std::pair<int, double> Table::ColMax(int c, int st){
	int rec = st * wth, bound = hgh * wth;
	double maxi = tab[rec + c];
	for(int i = rec;i < bound; i += wth)
		if(tab[i + c] > maxi){
			rec = i;
			maxi = tab[i + c];
		}
	return std::make_pair(rec / wth, maxi);
}

std::pair<int, double> Table::RowPos(int r, int st){
	int off = r * wth;
	for(int i = st;i < wth; ++i)
		if(tab[off + i] > 0) return std::make_pair(i, tab[off + i]);
	return std::make_pair(-1, 0.0);
}

std::pair<int, double> Table::RowNonZero(int r, int st){
	int off = r * wth;
	for(int i = st;i < wth; ++i)
		if(tab[off + i] != 0) return std::make_pair(i, tab[off + i]);
	return std::make_pair(-1, 0.0);
}

void Table::Pivot(int varin, int varout, int r){
	/* modify basic set of variables */
	basic[varout] = 0;
	basic[varin] = r;
	/* normalizing the r-th row */
	this->RowDot(r, 1 / tab[r * wth + varin]);
	/* Updating other lines */
	for(int i = 0;i < hgh;++i)
		if(i == r) continue;
		else this->RowSub(i, r, tab[i * wth + varin]);
	// printf("pivot: %d %d %d\n", varin, varout, r);
	// Display();
}

void Table::Simplex(){
	while(true){
		std::pair<int, double> tmp = this->RowPos(0, 1);
		const int varin = tmp.first;
		if(varin < 0) return;
		double maxi = neginf;
		int rec = wth;
		for(int r = rec; r < wth * hgh; r += wth)
			if(tab[r + varin] <= 0) continue;
			else{
				double t = tab[r] / tab[r + varin];
				if(maxi < t) {maxi = t; rec = r;}
			}
		if(maxi == neginf) throw LinProgExp(Expt::UNBOUNDED);
		rec /= wth;
		int varout = 1;
		while(basic[varout] != rec) varout++;
		this->Pivot(varin, varout, rec);
	}
}

void Table::InitSimplex(){
	/* The origin is a feasible */
	std::pair<int, double> t = this->ColMax(0, 1);
	if(t.second <= 0) return;
	
	/* Initial Linear Prog */
	Table tmp = Table(hgh, wth + 1);
	for(int i = 1; i < hgh;++i){
		memcpy(tmp.tab + i * tmp.wth, tab + i * wth, wth * sizeof(double));
		tmp.tab[(i + 1) * tmp.wth - 1] = -1;
	}
	tmp.tab[tmp.wth - 1] = -1;
	memcpy(tmp.basic, basic, wth * sizeof(int));
	/* A feasible solution */
	tmp.Pivot(wth, wth - hgh + t.first, t.first);
	tmp.Simplex(); // Optimize
	if(tmp.tab[0] < 0) throw LinProgExp(Expt::INFEASIBLE);

	/* We have got a feasible solution */
	/* In case that wth-th var is basic, swap it out */
	if(tmp.basic[wth]){
		std::pair<int, double> tt = tmp.RowNonZero(tmp.basic[wth], 1);
		tmp.Pivot(tt.first, wth, tmp.basic[wth]);
	}
	/* Restore */
	for(int i = 1; i < hgh;++i)
		memcpy(tab + i * wth, tmp.tab + i * tmp.wth, wth * sizeof(double));
	memcpy(basic, tmp.basic, wth * sizeof(int));
	for(int i = 1;i <= wth - hgh;++i)
		if(basic[i]) this->RowSub(0, basic[i], tab[i]);
}

void Table::LinProg(){
	try{
		this->InitSimplex();
	}
	catch(LinProgExp &e){
		fprintf(stderr, "%s\n", e.what());
		exit(-1);
	}
	try{
		// this->Display();
		this->Simplex();
	}
	catch(LinProgExp &e){
		fprintf(stderr, "%s\n", e.what());
		exit(-1);
	}
}

void Table::Display(){
	printf("[\n");
	for(int i = 0;i < hgh * wth;i += wth){
		printf("[ ");
		for(int j = 0; j < wth;++j)
			printf("%lf ", tab[i + j]);
		printf("]\n");
	}
	printf("\n[ ");
	for(int i = 0;i < wth;i++)
		printf("%d ", basic[i]);
	printf("]\n]\n");
}

int main(){
	int n, m;
	scanf("%d %d", &n, &m);
	double arr[m + 1][n + m + 1];
	int bas[n + m + 1];
	memset(arr, 0, sizeof(arr));
	memset(bas, 0, sizeof(bas));

	for(int i = 1;i <= n;++i)
		scanf("%lf", &arr[0][i]);
	for(int i = 1;i <= m;++i){
		for(int j = 1;j <= n;++j)
			scanf("%lf", &arr[i][j]);
		scanf("%lf", &arr[i][0]);
		arr[i][0] = -arr[i][0];
		arr[i][n + i] = 1;
		bas[n + i] = i;
	}
	Table t(m + 1, n + m + 1);
	memcpy(t.tab, arr, sizeof(arr));
	memcpy(t.basic, bas, sizeof(bas));
	t.LinProg();
	printf("Solution: [ ");
	for(int i = 1;i <= n;++i)
		if(t.basic[i]) printf("%.6f ", -t.tab[t.basic[i] * t.wth]);
		else printf("0.000000 ");
	printf("]\nOptimal Value: %.6f\n", t.tab[0]);
}