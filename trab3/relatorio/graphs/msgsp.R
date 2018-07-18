names = c("Gasto")
cores = c("blue")

mestre = c(2.4, 4.7, 54.4, 77.6, 89, 92)

g_range <- range(0, mestre)

pdf("msgsp.pdf")

plot(mestre, type="o", col="blue", ylim=g_range, xaxt='n', ann=FALSE)
axis(1, at=1:6, lab=c("1", "2", "4", "8", "16", "32"))

title(xlab="N processos", col.lab=rgb(0,0.5,0))
title(ylab="% tempo gasto com envio de mensagens", col.lab=rgb(0,0.5,0))
title(main="Tempo relativo gasto com passagem de mensagens", col.main="red")
legend("bottomright", inset=.05, g_range[2], names, cex=0.8,
   col=cores, pch=21:22, lty=1:2);
