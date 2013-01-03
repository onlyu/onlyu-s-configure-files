x <- rnorm(100)

myfunction <- function(x) {
  x + rnorm(length(x))
}