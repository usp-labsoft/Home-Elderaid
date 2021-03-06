#Colocar cabe�alhos em "dados.csv", como luminosidade e presenca, luzLed e presLed
df = read.csv("dados.csv",stringsAsFactors = FALSE)
summary(df) #Distribution measures
#hist(df$luminosidade,main="Distribui��o da Luminosidade",xlab="luminosidade",breaks = 20,xlim = c(0,1000))
#hist(df$luzLed,main="Distribui��o da LuzLed",xlab="luzLed",breaks = 20,xlim = c(0,255))
#hist(df$presenca,main="Distribui��o da Presenca",xlab="presenca",breaks = 20,xlim = c(2,11))
#hist(df$presLed,main="Distribui��o de presLed",xlab="presLed",breaks = 20,xlim = c(0,255))
#qqnorm(df$luminosidade) #Pior aprox pela normal
#qqnorm(df$presenca)	#Melhor aproxima��o pela normal
#stripchart(df$luminosidade) #N�o bom para muitos dados
#plot(df$luminosidade,df$luzLed) #Boa an�lise
#plot(df$presenca,df$presLed) #Boa an�lise
#plot(df$luzLed,df$presLed) #Mostra n�o haver rela��o aparente de luzLed e presLed

#Classification
luzclassifier <- function(x){
	prediction <- rep(NA, length(x)) #Initialize prediction vector
	prediction[x>700] <- 255.0
	prediction[x<=700] <- 0.0
	return (prediction) #prediction is 0 or 1
}
pred_luz <- luzclassifier(df$luminosidade)
#plot(pred_luz,df$luzLed)	#Compara luz categorizada de previs�o com a realizada

presclassifier <- function(x){
	prediction <- rep(NA, length(x)) #Initialize prediction vector
	prediction[x>9.5] <- 255.0
	prediction[8.5<x & x<=9.5] <- 127.5
	prediction[7.5<x & x<=8.5] <- 63.75
	prediction[x<=7.5] <- 0.0
	return (prediction) #prediction is 0 or 1
}
pred_pres <- presclassifier(df$presenca)
#plot(pred_pres,df$presLed)	#Compara presen�a categorizada de previs�o com a realizada


#Clustering
luminosidade = read.csv("luminosidade.csv",stringsAsFactors = FALSE)
presenca = read.csv("presenca.csv",stringsAsFactors = FALSE)
#random seed
set.seed(1)
positive <- luminosidade[1]
positive2 <- presenca[1]
kmeans_luminosidade <- kmeans(positive,3) #Interessante mudar o n�m de clusters e ver resultados
kmeans_presenca <- kmeans(positive2,2)
#plot(df$luminosidade ~ df$luzLed,data = luminosidade[1],col = kmeans_luminosidade$cluster)
plot(df$presenca ~ df$presLed,data = presenca[1],col = kmeans_presenca$cluster)