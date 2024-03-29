% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/summary_lm.R
\name{summary_lm}
\alias{summary_lm}
\title{summary_lm}
\usage{
summary_lm(obj, ...)
}
\arguments{
\item{obj}{Object returned by \code{\link{.lm.fit}}.}

\item{...}{ignored}
}
\value{
a p x 4 matrix with columns for the estimated coefficient, its standard error,
t-statistic and corresponding (two-sided) p-value. Aliased coefficients are omitted.
}
\description{
summary method for class ".lm.fit".. It's 200 times faster than traditional \code{lm}.
}
\examples{
set.seed(129)
n <- 100
p <- 2
X <- matrix(rnorm(n * p), n, p) # no intercept!
y <- rnorm(n)

obj <- .lm.fit (x = cbind(1, X), y = y) 
info <- summary_lm(obj)
}
