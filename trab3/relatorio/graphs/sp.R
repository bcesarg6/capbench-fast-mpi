names = c("Ideal", "Execução", "Exec + envio")
ts = c("1 fluxo", "2 fluxos", "4 fluxos", "8 fluxos", "16 fluxos", "32 fluxos")
cores = c("black", "blue", "red")

ideal = c(1, 2, 4, 8, 16, 32)
exec = c(1, 1.871, 3.743, 7.49, 15, 21.48)
exec_env = c(1, 1.82, 1.752, 1.71, 1.7, 1.75)

g_range <- range(0, ideal, exec, exec_env)

pdf("sp.pdf")

plot(ideal, type="o", col="black", ylim=g_range, xaxt='n', yaxt='n', ann=FALSE)
axis(1, at=1:6, lab=c("1", "2", "4", "8", "16", "32"))
axis(2, at=c(1, 2, 4, 8, 16, 32))
lines(exec, type="o", col="blue", pch=22, lty=2)
lines(exec_env, type="o", col="red", pch=21, lty=2)


title(xlab="Número de fluxos", col.lab=rgb(0,0.5,0))
title(ylab="Speedup", col.lab=rgb(0,0.5,0))
title(main="", col.main="red")
legend(1, g_range[2], names, cex=0.8,
   col=cores, pch=21:22, lty=1:2);
