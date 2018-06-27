names = c("Mestre", "Escravos")
ts = c("2 fluxos", "4 fluxos", "8 fluxos", "16 fluxos")
cores = c("black", "blue")

mestre = c(96.94, 96.46, 96.18, 95.67)
escravos = c(37.80, 63.95, 80.42, 89.18)

g_range <- range(0, mestre, escravos)

pdf("msgsp.pdf")

plot(mestre, type="o", col="black", ylim=g_range, xaxt='n', ann=FALSE)
axis(1, at=1:4, lab=c("2", "4", "8", "16"))
lines(escravos, type="o", col="blue", pch=22, lty=2)

title(xlab="N processos", col.lab=rgb(0,0.5,0))
title(ylab="% tempo gasto com MPI", col.lab=rgb(0,0.5,0))
title(main="Tempo relativo gasto com passagem de mensagens por MPI", col.main="red")
legend("bottomright", inset=.05, g_range[2], names, cex=0.8,
   col=cores, pch=21:22, lty=1:2);
