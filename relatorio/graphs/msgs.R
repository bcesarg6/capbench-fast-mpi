names = c("1", "2", "4", "8", "16")
cores = c("yellow", "red", "blue", "green", "purple")

vals <- c(2.273573875, 28.007216375, 40.81487875, 47.28618225, 47.406148125)

mat <- matrix(vals, nrow=5, ncol=1)
y_lim <- range(0, vals)

pdf("msgs.pdf")

barplot(mat, main="Tempo de envio e recebimento dos dados", xlab="N processos", ylab="Segundos", ylim=y_lim, col=c("yellow", "red", "blue", "green", "purple"),
  names.arg=names, beside=TRUE)
