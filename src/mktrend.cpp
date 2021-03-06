#include <Rcpp.h>
using namespace Rcpp;

//' @rdname slope
//' @export
// [[Rcpp::export]]
SEXP slope_sen(const NumericVector& y, Nullable<NumericMatrix> x = R_NilValue) {
    int n = y.size();
    NumericVector xx;
    // isNotNull
    if (x.isNull()) {
        xx = NumericVector(n);
        for (int i = 0; i < n; i++) xx[i] = i;
    } else {
        xx = x;
    }

    int len = (n * n - n) / 2;
    NumericVector V = NumericVector(len, NA_REAL);  // int V[len];

    int k = 0;
    for (int j = 1; j < n; j++) {
        for (int i = 0; i < j; i++) {
            V[k] = (y[j] - y[i]) / (xx[j] - xx[i]);
            k++;
            // Rcout << i << "," << j << std::endl;
            // Rcout << "k = " << k << ": " << V[k] << std::endl;
        }
    }
    // Rcout << V << std::endl;
    double slope = Rcpp::median(V, true);  //rm_na is true
    return wrap(slope);
}

int sign(double x) {
    return x > 0 ? 1 : (x == 0 ? 0 : -1);
}

// [[Rcpp::export]]
int Sf(NumericVector x) {
    int n = x.size();
    int S = 0;

    for (int j = 1; j < n; j++) {
        for (int i = 0; i < j; i++) {
            S += sign(x[j] - x[i]);
        }
    }
    return S;
}

// [[Rcpp::export]]
NumericVector varS(NumericVector x, NumericVector rof, int S) {
    int n = x.size();
    // int S = 0;
    double cte = 2.0 / (n * (n - 1) * (n - 2));
    double ess = 0;
    for (int i = 1; i <= n - 1; i++) {
        ess = ess + (n - i) * (n - i - 1) * (n - i - 2) * rof[i - 1];
    }

    double essf = 1 + ess * cte;
    double var_S = n * (n - 1) * (2 * n + 5) / 18;

    NumericVector aux = unique(x);
    int m = aux.size();
    if (m < n) {
        int tie;
        for (int i = 0; i < m; i++) {
            tie = 0;
            for (int j = 0; j < n; j++) {
                if (x[j] == aux[i]) tie++;
            }
            if (tie > 1)
                var_S = var_S - tie * (tie - 1) * (2 * tie + 5) / 18;
        }
    }

    double VS = var_S * essf;
    double z, z0;
    if (S == 0) {
        z = 0;
        z0 = 0;
    } else if (S > 0) {
        z = (S - 1) / sqrt(VS);
        z0 = (S - 1) / sqrt(var_S);
    } else {
        z = (S + 1) / sqrt(VS);
        z0 = (S + 1) / sqrt(var_S);
    }

    return NumericVector::create(
        _["essf"] = essf,
        _["var.S"] = var_S,
        _["z0"] = z0,
        _["z"] = z);
    // double VS = var_S * essf;
    // return S;
}

/*** R
# x <- rnorm(50)
# library(microbenchmark)
# # library(Rcpp)
# # microbenchmark(slope_sen(x), times=100)
# 
# source("E:/GitHub/Vegetation/TP_phenology/R/mainfunc/main_TREND.R", encoding = "utf-8")
# x <- rnorm(32)
# microbenchmark(mkTrend(x),
#                mkTrend.rcpp(x), times=100)
*/
