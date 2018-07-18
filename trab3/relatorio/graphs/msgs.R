names = c("1", "2", "4", "8", "16", "32")
cores = c("yellow", "red", "blue", "green", "purple", "orange")

vals <- c(3.9009025, 4.121629375, 49.654947875, 72.432384, 84.409318875, 84.387548875)

mat <- matrix(vals, nrow=6, ncol=1)
y_lim <- range(0, vals)

pdf("msgs.pdf")

barplot(mat, main="", xlab="Número de fluxos", ylab="Segundos", ylim=y_lim, col=c("yellow", "red", "blue", "green", "purple", "orange"),
  names.arg=names, beside=TRUE)
