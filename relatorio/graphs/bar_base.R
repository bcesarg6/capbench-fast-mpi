names = c("1", "2", "4", "8", "16")
cores = c("yellow", "red", "blue", "green", "purple")

vals <- c(1, 2, 4, 8, 16)

mat <- matrix(vals, nrow=5, ncol=1)
y_lim <- range(0, vals)

pdf("bar_base.pdf")

barplot(mat, main="Gráfico de barras", xlab="N processos", ylab="Dado a ser verifiacado", ylim=y_lim, col=c("yellow", "red", "blue", "green", "purple"),
  names.arg=names, beside=TRUE)
