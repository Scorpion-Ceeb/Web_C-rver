# Project C-rver

Este proyecto tiene como objetivo crear un servidor web que lea los directorios en __C__


## Dependencias

Este proyecto usa la biblioteca __libcurl__. Para instalarla ejecute este comando en Ubuntu

```
sudo apt-get install libcurl4-openssl-dev
```
## Ejecutar

Para ejecutar el projecto se tiene el __makefile__ donde tiene los comandos a ejecutar
con __make__

### makefile Preview

```
port ?=5000
root ?=$(HOME)

.PHONY: run
run:
		gcc ./*.c -lpthread -lcurl -o main && ./main $(port) $(root)

.PHONY: build
build:
		gcc ./*c -lpthread -lcurl -o main
```

__make run__: Corre el projecto directamente despues de compilarlo con esos valores predeterminados del puerto y la direccion

__make build__: Compila el proyecto, luego para ejecutarlo con los valores del puerto y la direccion deseados

```
./main <port> <root>
```